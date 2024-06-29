// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "settingswindow.h"

#include "../common/buttons.h"
#include "../common/jsonhelper.h"

#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVBoxLayout>

SettingsWindow::SettingsWindow(Limits* model, QWidget* parent) : QWidget{parent} {
  this->limits = model;

  // Window title
  setWindowTitle("Aqua mixtura - " + tr("Einstellungen"));

  // main layout
  QVBoxLayout* layout = new QVBoxLayout(this);

  // Table view with limits
  limitsView = new QTableView(this);
  limitsView->setModel(limits);
  // TODO add delegate
  limitsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  limitsView->verticalHeader()->setDefaultAlignment(Qt::AlignRight);
  layout->addWidget(limitsView);

  // Buttons
  Buttons* buttons = new Buttons("", "", "", tr("Einstellungen importieren"), tr("Einstellungen exportieren"),
                                 tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnImport, &QPushButton::clicked, this, &SettingsWindow::settingsImport);
  QObject::connect(buttons->btnExport, &QPushButton::clicked, this, &SettingsWindow::settingsExport);
  QObject::connect(buttons->btnSave, &QPushButton::clicked, limits, &Limits::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, limits, &Limits::load);
  layout->addWidget(buttons);

  setLayout(layout);
}

void SettingsWindow::settingsImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Einstellungen Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!limits->fromJson(JsonHelper::loadFile(path))) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Einstellungen nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void SettingsWindow::settingsExport() {
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + "limits.json";
  QString path =
      QFileDialog::getSaveFileName(this, tr("Einstellungen Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!JsonHelper::saveFile(path, limits->toJson())) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Einstellungen nicht exportieren"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}
