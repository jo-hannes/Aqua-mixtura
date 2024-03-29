// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "limitswindow.h"

#include "../common/buttons.h"
#include "../common/jsonhelper.h"

#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVBoxLayout>

LimitsWindow::LimitsWindow(Limits* model, QWidget* parent) : QWidget{parent} {
  this->limits = model;

  // Window title
  setWindowTitle("Aqua mixtura - " + tr("Beschränkungen"));

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
  Buttons* buttons = new Buttons("", "", "", tr("Beschränkungen importieren"), tr("Beschränkungen exportieren"),
                                 tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnImport, &QPushButton::clicked, this, &LimitsWindow::limitsImport);
  QObject::connect(buttons->btnExport, &QPushButton::clicked, this, &LimitsWindow::limitsExport);
  QObject::connect(buttons->btnSave, &QPushButton::clicked, this, &LimitsWindow::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &LimitsWindow::load);
  layout->addWidget(buttons);

  setLayout(layout);
}

void LimitsWindow::limitsImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Beschränkungen Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!limits->fromJson(JsonHelper::loadFile(path))) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Beschränkungen nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void LimitsWindow::limitsExport() {
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + "limits.json";
  QString path =
      QFileDialog::getSaveFileName(this, tr("Beschränkungen Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!JsonHelper::saveFile(path, limits->toJson())) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Beschränkungen nicht exportieren"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}
