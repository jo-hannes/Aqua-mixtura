// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersourcewindow.h"

#include "../model/jsonhelper.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

WatersourceWindow::WatersourceWindow(MainModel* model, QWidget* parent) : QWidget(parent) {
  this->model = model;
  selected = -1;
  // Überschriften
  QLabel* txtQuellen = new QLabel(tr("Wasserquellen"));
  txtQuellen->setStyleSheet("font-weight: bold");

  QLabel* txtProfil = new QLabel(tr("Wasserprofil"));
  txtProfil->setStyleSheet("font-weight: bold");

  // Quellen
  QVBoxLayout* layoutQuellen = new QVBoxLayout();

  sourcesView = new QListView();
  sourcesView->setModel(model->sources);
  layoutQuellen->addWidget(sourcesView);

  // buttons für quellen
  QPushButton* btnAdd = new QPushButton();
  btnAdd->setToolTip(tr("Wasserquelle hinzufügen"));
  btnAdd->setIcon(QIcon(":/icons/document-plus.svg"));
  QObject::connect(btnAdd, &QPushButton::clicked, this, &WatersourceWindow::profileAdd);
  QPushButton* btnCopy = new QPushButton();
  btnCopy->setToolTip(tr("Wasserquelle kopieren"));
  btnCopy->setIcon(QIcon(":/icons/document-duplicate.svg"));
  QObject::connect(btnCopy, &QPushButton::clicked, this, &WatersourceWindow::profileCopy);
  QPushButton* btnDelete = new QPushButton();
  btnDelete->setToolTip(tr("Wasserquelle löschen"));
  btnDelete->setIcon(QIcon(":/icons/document-minus.svg"));
  QObject::connect(btnDelete, &QPushButton::clicked, this, &WatersourceWindow::profileDelete);
  QPushButton* btnImport = new QPushButton();
  btnImport->setToolTip(tr("Wasserquelle importieren"));
  btnImport->setIcon(QIcon(":/icons/document-arrow-down.svg"));
  QObject::connect(btnImport, &QPushButton::clicked, this, &WatersourceWindow::profileImport);
  QPushButton* btnExport = new QPushButton();
  btnExport->setToolTip(tr("Wasserquelle exportieren"));
  btnExport->setIcon(QIcon(":/icons/document-arrow-up.svg"));
  QObject::connect(btnExport, &QPushButton::clicked, this, &WatersourceWindow::profileExport);
  QDialogButtonBox* buttonsQuellen = new QDialogButtonBox();
  buttonsQuellen->addButton(btnAdd, QDialogButtonBox::ActionRole);
  buttonsQuellen->addButton(btnCopy, QDialogButtonBox::ActionRole);
  buttonsQuellen->addButton(btnDelete, QDialogButtonBox::ActionRole);
  buttonsQuellen->addButton(btnImport, QDialogButtonBox::ActionRole);
  buttonsQuellen->addButton(btnExport, QDialogButtonBox::ActionRole);
  layoutQuellen->addWidget(buttonsQuellen);

  // Profil Editor
  waterEdit = new WaterProfileEdit();

  // sticking it together
  QGridLayout* layoutMain = new QGridLayout();
  layoutMain->addWidget(txtQuellen, 0, 0, Qt::AlignLeft);
  layoutMain->addWidget(txtProfil, 0, 1, Qt::AlignLeft);
  layoutMain->addLayout(layoutQuellen, 1, 0);
  layoutMain->addWidget(waterEdit, 1, 1);
  setLayout(layoutMain);

  QObject::connect(sourcesView, &QListView::pressed, this, &WatersourceWindow::selectSource);
  QObject::connect(waterEdit, &WaterProfileEdit::saveProfile, this, &WatersourceWindow::saveProfile);
}

void WatersourceWindow::selectSource(const QModelIndex& index) {
  if (selected == index.row()) {  // same row selected again => do noting
    return;
  }
  if (waterEdit->isChanged()) {
    QMessageBox msgBox;
    msgBox.setText(tr("Änderungen speichern?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Save:
        // Save, then siwtch source
        waterEdit->save();
        selected = index.row();
        waterEdit->setProfile(model->sources->getProfile(selected));
        break;
      case QMessageBox::Discard:
        // Just switch to new selection
        selected = index.row();
        waterEdit->setProfile(model->sources->getProfile(selected));
        break;
      case QMessageBox::Cancel:
        // Select previously selected item
        sourcesView->setCurrentIndex(sourcesView->model()->index(selected, 0));
        break;
      default:
        // should never be reached
        break;
    }
  } else {
    selected = index.row();
    waterEdit->setProfile(model->sources->getProfile(selected));
  }
}

void WatersourceWindow::saveProfile(Water& profile) {
  if (selected >= 0 && selected < model->sources->rowCount()) {
    model->sources->updateProfile(profile, selected);
    model->saveSources();
  }
}

void WatersourceWindow::profileAdd() {
  Water newProfile("New");
  model->sources->addProfile(newProfile);
}

void WatersourceWindow::profileCopy() {
  Water newProfile = model->sources->getProfile(selected);
  newProfile.setName("Copy of " + newProfile.getName());
  model->sources->addProfile(newProfile);
}

void WatersourceWindow::profileDelete() {
  model->sources->deleteProfile(selected);
}

void WatersourceWindow::profileImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Wasserquelle Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  QJsonObject jsonSource = JsonHelper::loadFile(path);
  if (jsonSource.contains("WaterSource")) {
    Water wp = Water::fromJson(jsonSource["WaterSource"].toObject());
    model->sources->addProfile(wp);
  } else {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Wasserquelle nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void WatersourceWindow::profileExport() {
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              model->sources->getProfile(selected).getName() + ".json";
  QString path =
      QFileDialog::getSaveFileName(this, tr("Wasserquelle Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  QJsonObject jsonSource;
  jsonSource["WaterSource"] = model->sources->getProfile(selected).toJson();
  bool success = JsonHelper::saveFile(path, jsonSource);
  if (!success) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Wasserquelle nicht speichern"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}
