// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersourcewindow.h"

#include "../common/buttons.h"
#include "../common/jsonhelper.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

WatersourceWindow::WatersourceWindow(WaterSources* model, QWidget* parent) : QWidget(parent) {
  this->sources = model;
  selected = -1;
  // Überschriften
  QLabel* txtQuellen = new QLabel(tr("Wasserquellen"));
  txtQuellen->setStyleSheet("font-weight: bold");

  QLabel* txtProfil = new QLabel(tr("Wasserprofil"));
  txtProfil->setStyleSheet("font-weight: bold");

  // Quellen
  QVBoxLayout* layoutQuellen = new QVBoxLayout();

  sourcesView = new QListView();
  sourcesView->setModel(sources);
  layoutQuellen->addWidget(sourcesView);

  // buttons für quellen
  Buttons* buttonsQuellen =
      new Buttons(tr("Wasserquelle hinzufügen"), tr("Wasserquelle kopieren"), tr("Wasserquelle löschen"),
                  tr("Wasserquelle importieren"), tr("Wasserquelle exportieren"));
  QObject::connect(buttonsQuellen->btnAdd, &QPushButton::clicked, this, &WatersourceWindow::profileAdd);
  QObject::connect(buttonsQuellen->btnCopy, &QPushButton::clicked, this, &WatersourceWindow::profileCopy);
  QObject::connect(buttonsQuellen->btnDelete, &QPushButton::clicked, this, &WatersourceWindow::profileDelete);
  QObject::connect(buttonsQuellen->btnImport, &QPushButton::clicked, this, &WatersourceWindow::profileImport);
  QObject::connect(buttonsQuellen->btnExport, &QPushButton::clicked, this, &WatersourceWindow::profileExport);
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
        waterEdit->setProfile(sources->getProfile(selected));
        break;
      case QMessageBox::Discard:
        // Just switch to new selection
        selected = index.row();
        waterEdit->setProfile(sources->getProfile(selected));
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
    waterEdit->setProfile(sources->getProfile(selected));
  }
}

void WatersourceWindow::saveProfile(Water& profile) {
  if (selected >= 0 && selected < sources->rowCount()) {
    sources->updateProfile(profile, selected);
    emit save();
  }
}

void WatersourceWindow::profileAdd() {
  Water newProfile("New");
  sources->addProfile(newProfile);
}

void WatersourceWindow::profileCopy() {
  Water newProfile = sources->getProfile(selected);
  newProfile.updateCreationTime();
  newProfile.setName("Copy of " + newProfile.getName());
  sources->addProfile(newProfile);
}

void WatersourceWindow::profileDelete() {
  sources->deleteProfile(selected);
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
    Water wp(jsonSource["WaterSource"].toObject());
    sources->addProfile(wp);
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
                              sources->getProfile(selected).getName() + ".json";
  QString path =
      QFileDialog::getSaveFileName(this, tr("Wasserquelle Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  QJsonObject jsonSource;
  jsonSource["WaterSource"] = sources->getProfile(selected).profileToJson();
  bool success = JsonHelper::saveFile(path, jsonSource);
  if (!success) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Wasserquelle nicht speichern"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}
