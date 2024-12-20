// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixwaterwidget.h"

#include "../common/buttons.h"
#include "../water/watersourcestabledelegate.h"

#include <QAction>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

MixWaterWidget::MixWaterWidget(WaterSources& mixtureWaters, WaterSources& waterDb, QWidget* parent)
    : QFrame{parent}, wMix{mixtureWaters}, wDb{waterDb} {
  // Build ui
  auto* layout = new QVBoxLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // heading
  auto* heading = new QHBoxLayout();
  heading->addWidget(new QLabel(tr("Wasser")), 0, Qt::AlignLeft);
  heading->addWidget(new QLabel(tr("Menge:")), 1, Qt::AlignRight);
  wTotal = new QDoubleSpinBox();
  wTotal->setMinimum(0);
  wTotal->setMaximum(9999);
  wTotal->setDecimals(2);
  wTotal->setSingleStep(0.1);
  wTotal->setValue(wMix.getTotalVolume());
  QObject::connect(wTotal, &QDoubleSpinBox::valueChanged, &wMix, &WaterSources::setTotalVolume);
  heading->addWidget(wTotal, 0, Qt::AlignRight);
  heading->addWidget(new QLabel("L"), 0, Qt::AlignLeft);
  // layout->addWidget(heading);
  layout->addLayout(heading);

  // View with waters
  waterView = new QTableView(this);
  waterView->setModel(&wMix);
  waterView->verticalHeader()->setVisible(false);
  waterView->setItemDelegate(new WaterSourcesTableDelegate(this));
  waterView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  layout->addWidget(waterView);

  // Buttons
  auto* buttons = new Buttons(tr("Wasser hinzufügen"), "", tr("Wasser löschen"), "", "", "", "");
  QObject::connect(buttons->btnAdd, &QPushButton::clicked, this, &MixWaterWidget::add);
  QObject::connect(buttons->btnDelete, &QPushButton::clicked, this, &MixWaterWidget::remove);
  layout->addWidget(buttons);

  // build button menu
  waterMenu = new QMenu(this);
  updateWaterDb();
  QObject::connect(&wDb, &WaterSources::dataChanged, this, &MixWaterWidget::updateWaterDb);
  buttons->btnAdd->setMenu(waterMenu);
}

void MixWaterWidget::add(int i) {
  wMix.addProfile(wDb.getProfile(i));
}

void MixWaterWidget::remove() {
  const QModelIndex idx = waterView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  wMix.deleteProfile(idx.row());
}

void MixWaterWidget::updateWaterDb() {
  waterMenu->clear();
  for (int i = 0; i < wDb.rowCount(); i++) {
    auto* act = new QAction(wDb.getProfile(i).getName(), waterMenu);
    QObject::connect(act, &QAction::triggered, this, [this, i]() {
      add(i);
    });
    waterMenu->addAction(act);
  }
}
