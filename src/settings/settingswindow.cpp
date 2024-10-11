// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "settingswindow.h"

#include "../common/buttons.h"
#include "../common/dialogs.h"
#include "../common/jsonhelper.h"

#include <QFileDialog>
#include <QHeaderView>
#include <QStandardPaths>
#include <QVBoxLayout>

SettingsWindow::SettingsWindow(Settings& model, QWidget* parent) : QWidget{parent}, settings(model) {
  // Window title
  setWindowTitle("Aqua-mixtura - " + tr("Einstellungen"));

  // main layout
  QVBoxLayout* layout = new QVBoxLayout(this);

  // Table view with limits
  limitsView = new QTableView(this);
  limitsView->setModel(&settings);
  // TODO add delegate
  limitsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  limitsView->verticalHeader()->setDefaultAlignment(Qt::AlignRight);
  layout->addWidget(limitsView);

  // Buttons
  Buttons* buttons = new Buttons("", "", "", tr("Einstellungen importieren"), tr("Einstellungen exportieren"),
                                 tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnImport, &QPushButton::clicked, this, &SettingsWindow::settingsImport);
  QObject::connect(buttons->btnExport, &QPushButton::clicked, this, &SettingsWindow::settingsExport);
  QObject::connect(buttons->btnSave, &QPushButton::clicked, &settings, &Settings::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, &settings, &Settings::load);
  layout->addWidget(buttons);

  setLayout(layout);
}

void SettingsWindow::closeEvent(QCloseEvent* event) {
  if (settings.isChanged()) {
    int ret = Dialogs::saveChanges(tr("Änderungen speichern?"), tr("Einstellungen wurden nicht gespeicherte"));
    switch (ret) {
      case QMessageBox::Save:
        settings.save();  // save and close window
        break;
      case QMessageBox::Discard:
        settings.load();
        break;
      case QMessageBox::Cancel:
        event->ignore();  // ignore event to keep window open
        return;
        break;
    }
  }
  event->accept();
}

void SettingsWindow::settingsImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Einstellungen importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!settings.fromJson(JsonHelper::loadFile(path))) {
    Dialogs::info(tr("Fehler beim Importieren"), tr("Konnte Einstellungen nicht im JSON finden"));
  }
}

void SettingsWindow::settingsExport() {
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + "settings.json";
  QString path =
      QFileDialog::getSaveFileName(this, tr("Einstellungen exportieren"), suggestedFileName, "JSON (*.json)");
  if (path.isEmpty()) {
    return;
  }
  if (!JsonHelper::saveFile(path, settings.toJson())) {
    Dialogs::info(tr("Fehler beim Exportieren"), tr("Konnte Einstellungen nicht speichern"));
  }
}
