// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixwaterwidget.h"

#include "../common/buttons.h"

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

MixWaterWidget::MixWaterWidget(WaterSources* mixtureWaters, WaterSources* waterDb, QWidget* parent) : QFrame{parent} {
  wMix = mixtureWaters;
  wDb = waterDb;
  // Build ui
  QVBoxLayout* layout = new QVBoxLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // heading
  QLabel* heading = new QLabel(tr("Wasser"));
  layout->addWidget(heading);

  // View with waters
  waterView = new QTableView(this);
  waterView->setModel(wMix);
  waterView->verticalHeader()->setVisible(false);
  waterView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  layout->addWidget(waterView);

  // Buttons
  Buttons* buttons = new Buttons(tr("Wasser hinzufügen"), "", tr("Wasser löschen"), "", "", "", "");
  QObject::connect(buttons->btnAdd, &QPushButton::clicked, this, &MixWaterWidget::add);
  QObject::connect(buttons->btnDelete, &QPushButton::clicked, this, &MixWaterWidget::remove);
  layout->addWidget(buttons);

  // build button menu
  waterMenu = new QMenu(this);
  updateWaterDb();
  QObject::connect(wDb, &WaterSources::dataChanged, this, &MixWaterWidget::updateWaterDb);
  buttons->btnAdd->setMenu(waterMenu);
}

void MixWaterWidget::add(int i) {
  wMix->addProfile(wDb->getProfile(i));
}

void MixWaterWidget::remove() {
  QModelIndex idx = waterView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  wMix->deleteProfile(idx.row());
}

void MixWaterWidget::updateWaterDb() {
  waterMenu->clear();
  for (int i = 0; i < wDb->rowCount(); i++) {
    QAction* act = new QAction(wDb->getProfile(i).getName(), waterMenu);
    QObject::connect(act, &QAction::triggered, this, [=]() {
      add(i);
    });
    waterMenu->addAction(act);
  }
}
