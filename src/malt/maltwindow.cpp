// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "maltwindow.h"

#include "malttabledelegate.h"

#include "../common/buttons.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

MaltWindow::MaltWindow(MainModel* model, QWidget* parent) : QWidget{parent} {
  this->model = model;
  volatileMalts = new Malts();
  volatileMalts->setMalts(model->malts->getMalts());
  // Window tittle
  title = tr("Malze");
  this->setWindowTitle(title);
  QObject::connect(volatileMalts, &Malts::unsavedMalts, this, &MaltWindow::unsavedMalts);

  // main layout
  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  // Table View
  maltsView = new QTableView();
  maltsView->setModel(volatileMalts);
  maltsView->verticalHeader()->setVisible(false);
  MaltTableDelegate* delegate = new MaltTableDelegate();
  maltsView->setItemDelegate(delegate);
  // maltsView->hideColumn(1);  // Hide mass column, not needed here
  mainLayout->addWidget(maltsView);

  // Buttons
  Buttons* buttons = new Buttons(tr("Malz hinzufügen"), tr("Malz kopieren"), tr("Malz löschen"), tr("Malz importieren"),
                                 tr("Malz exportieren"), tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnAdd, &QPushButton::clicked, this, &MaltWindow::maltAdd);
  QObject::connect(buttons->btnCopy, &QPushButton::clicked, this, &MaltWindow::maltCopy);
  QObject::connect(buttons->btnDelete, &QPushButton::clicked, this, &MaltWindow::maltDelete);
  QObject::connect(buttons->btnImport, &QPushButton::clicked, this, &MaltWindow::maltImport);
  QObject::connect(buttons->btnExport, &QPushButton::clicked, this, &MaltWindow::maltExport);
  QObject::connect(buttons->btnSave, &QPushButton::clicked, this, &MaltWindow::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &MaltWindow::cancel);
  mainLayout->addWidget(buttons);
}

void MaltWindow::maltAdd() {
  Malt m;
  volatileMalts->addMalt(m);
}

void MaltWindow::maltCopy() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  Malt m = volatileMalts->getMalt(idx.row());
  m.updateCreationTime();
  m.setName("Copy of " + m.getName());
  volatileMalts->addMalt(m);
}

void MaltWindow::maltDelete() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  volatileMalts->deleteMalt(idx.row());
}

void MaltWindow::maltImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Malz Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!volatileMalts->importMalt(path)) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Malz nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void MaltWindow::maltExport() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              volatileMalts->getMalt(idx.row()).getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Malz Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!volatileMalts->exportMalt(path, idx.row())) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Malz nicht speichern"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void MaltWindow::save() {
  model->malts->setMalts(volatileMalts->getMalts());
  volatileMalts->setSaved();
  model->saveMalts();
}

void MaltWindow::cancel() {
  volatileMalts->setMalts(model->malts->getMalts());
}

void MaltWindow::unsavedMalts(bool unsaved) {
  if (unsaved) {
    setWindowTitle("* " + title);
  } else {
    setWindowTitle(title);
  }
  emit maltWindowUnsavedChanges(unsaved);
}
