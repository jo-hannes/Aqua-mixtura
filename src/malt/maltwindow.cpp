// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "maltwindow.h"

#include "malttabledelegate.h"

#include "../common/buttons.h"
#include "../common/dialogs.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

MaltWindow::MaltWindow(Malts& model, QWidget* parent) : QWidget{parent}, malts{model} {
  // Window tittle
  title = "Aqua mixtura - " + tr("Malze");
  this->setWindowTitle(title);
  QObject::connect(&malts, &Malts::dataModified, this, &MaltWindow::unsavedMalts);

  // main layout
  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  // Table View
  maltsView = new QTableView();
  maltsView->setModel(&malts);
  maltsView->verticalHeader()->setVisible(false);
  MaltTableDelegate* delegate = new MaltTableDelegate(this);
  maltsView->setItemDelegate(delegate);
  // maltsView->setEditTriggers(QAbstractItemView::AllEditTriggers);
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
  QObject::connect(buttons->btnSave, &QPushButton::clicked, &malts, &Malts::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, &malts, &Malts::load);  // just load on cancel
  mainLayout->addWidget(buttons);
}

MaltWindow::~MaltWindow() {}

void MaltWindow::closeEvent(QCloseEvent* event) {
  if (malts.isChanged()) {
    int ret = Dialogs::saveChanges(tr("Änderungen speichern?"), tr("Malze haben ungespeicherte Änderungen"));
    switch (ret) {
      case QMessageBox::Save:
        malts.save();  // save and close window
        break;
      case QMessageBox::Discard:
        malts.load();
        break;
      case QMessageBox::Cancel:
        event->ignore();  // ignore event to keep window open
        return;
        break;
    }
  }
  event->accept();
}

void MaltWindow::maltAdd() {
  Malt m;
  malts.addMalt(m);
}

void MaltWindow::maltCopy() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  Malt m = malts.getMalt(idx.row());
  m.updateCreationTime();
  m.newUuid();
  m.setName("Copy of " + m.getName());
  malts.addMalt(m);
}

void MaltWindow::maltDelete() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  malts.deleteMalt(idx.row());
}

void MaltWindow::maltImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Malz Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!malts.importMalt(path)) {
    Dialogs::info(tr("Fehler beim Importieren"), tr("Konnte Malz nicht im JSON finden"));
  }
}

void MaltWindow::maltExport() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              malts.getMalt(idx.row()).getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Malz Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!malts.exportMalt(path, idx.row())) {
    Dialogs::info(tr("Fehler beim Exportieren"), tr("Konnte Malz nicht speichern"));
  }
}

void MaltWindow::unsavedMalts() {
  if (malts.isChanged()) {
    setWindowTitle("* " + title);
  } else {
    setWindowTitle(title);
  }
  emit maltWindowUnsavedChanges(malts.isChanged());
}
