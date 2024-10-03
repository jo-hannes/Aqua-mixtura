// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersourcewindow.h"

#include "../common/buttons.h"
#include "../common/dialogs.h"
#include "../common/jsonhelper.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

WatersourceWindow::WatersourceWindow(WaterSources& model, QWidget* parent) : QWidget(parent), sources{model} {
  selected = -1;
  // Überschriften
  setWindowTitle("Aqua mixtura - " + tr("Wasserquellen"));
  QLabel* txtQuellen = new QLabel(tr("Wasserquellen"));
  txtQuellen->setStyleSheet("font-weight: bold");

  QLabel* txtProfil = new QLabel(tr("Wasserprofil"));
  txtProfil->setStyleSheet("font-weight: bold");

  // Quellen
  QVBoxLayout* layoutQuellen = new QVBoxLayout();

  sourcesView = new QListView();
  sourcesView->setModel(&sources);
  layoutQuellen->addWidget(sourcesView);

  // buttons für quellen
  Buttons* buttonsQuellen = new Buttons(tr("Wasser hinzufügen"), tr("Wasser kopieren"), tr("Wasser löschen"),
                                        tr("Wasser importieren"), tr("Wasser exportieren"));
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

  // select first water
  if (sources.rowCount(QModelIndex()) >= 1) {
    sourcesView->setCurrentIndex(sourcesView->model()->index(0, 0));
    selectSource(sourcesView->model()->index(0, 0));
  }
}

void WatersourceWindow::closeEvent(QCloseEvent* event) {
  // check for changes
  if (saveChangesDialog() == QMessageBox::Cancel) {
    event->ignore();
  } else {
    event->accept();
  }
}

void WatersourceWindow::selectSource(const QModelIndex& index) {
  if (selected == index.row()) {  // same row selected again => do noting
    return;
  }
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    // Select previously selected item
    sourcesView->setCurrentIndex(sourcesView->model()->index(selected, 0));
  } else {
    // Just switch to new selection
    selected = index.row();
    waterEdit->setProfile(sources.getProfile(selected));
  }
}

void WatersourceWindow::saveProfile(Water& profile) {
  if (selected >= 0 && selected < sources.rowCount()) {
    sources.updateProfile(profile, selected);
    sources.save();
  }
}

void WatersourceWindow::profileAdd() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  Water newProfile("New");
  sources.addProfile(newProfile);
  sources.save();
  // select new profile
  QModelIndex idx = sourcesView->model()->index(sources.rowCount(QModelIndex()) - 1, 0);
  sourcesView->setCurrentIndex(idx);
  selectSource(idx);
}

void WatersourceWindow::profileCopy() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  Water newProfile = sources.getProfile(selected);
  newProfile.updateCreationTime();
  newProfile.newUuid();
  newProfile.setName("Copy of " + newProfile.getName());
  sources.addProfile(newProfile);
  sources.save();
  // select new profile
  QModelIndex idx = sourcesView->model()->index(sources.rowCount(QModelIndex()) - 1, 0);
  sourcesView->setCurrentIndex(idx);
  selectSource(idx);
}

void WatersourceWindow::profileDelete() {
  int ret = Dialogs::yesNo(tr("Wasser wirklich löschen?"), sources.getProfile(selected).getName());
  if (ret == QMessageBox::Yes) {
    // Delete profile, save cahnges and select another water
    waterEdit->cancel();  // reset changed flag before delition
    sources.deleteProfile(selected);
    sources.save();
    sourcesView->setCurrentIndex(sourcesView->model()->index(std::max(0, selected - 1), 0));
    selectSource(sourcesView->model()->index(std::max(0, selected - 1), 0));
  }
}

void WatersourceWindow::profileImport() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  QString path = QFileDialog::getOpenFileName(this, tr("Wasser importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  QJsonObject jsonSource = JsonHelper::loadFile(path);
  if (jsonSource.contains("WaterSource")) {
    Water wp(jsonSource["WaterSource"].toObject());
    sources.addProfile(wp);
    sources.save();
    // select imported profile
    QModelIndex idx = sourcesView->model()->index(sources.rowCount(QModelIndex()) - 1, 0);
    sourcesView->setCurrentIndex(idx);
    selectSource(idx);
  } else {
    Dialogs::info(tr("Fehler beim Importieren"), tr("Konnte Wasser nicht im JSON finden"));
  }
}

void WatersourceWindow::profileExport() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              sources.getProfile(selected).getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Wasser exportieren"), suggestedFileName, "JSON (*.json)");
  if (path.isEmpty()) {
    return;
  }
  QJsonObject jsonSource;
  jsonSource["WaterSource"] = sources.getProfile(selected).profileToJson();
  bool success = JsonHelper::saveFile(path, jsonSource);
  if (!success) {
    Dialogs::info(tr("Fehler beim Exportieren"), tr("Konnte Wasser nicht speichern"));
  }
}

int WatersourceWindow::saveChangesDialog() {
  if (waterEdit->isChanged()) {
    int ret = Dialogs::saveChanges(
        tr("Änderungen speichern?"),
        tr("Wasser \"%1\" hat ungespeicherte Änderungen").arg(sources.getProfile(selected).getName()));
    //  save or discard
    switch (ret) {
      case QMessageBox::Save:
        waterEdit->save();
        break;
      case QMessageBox::Discard:
        waterEdit->cancel();  // discard changes
        break;
    }
    return ret;
  } else {
    return 0;
  }
}
