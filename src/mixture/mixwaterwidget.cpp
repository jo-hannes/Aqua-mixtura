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
  // Total amount
  heading->addWidget(new QLabel(tr("Menge:")), 1, Qt::AlignRight);
  wTotal = new QDoubleSpinBox();
  wTotal->setMinimum(0.1);
  wTotal->setMaximum(9999);  // NOLINT(*-magic-numbers)
  wTotal->setDecimals(2);
  wTotal->setSingleStep(0.1);
  wTotal->setSuffix("L");
  wTotal->setValue(wMix.getTotalVolume());
  QObject::connect(wTotal, &QDoubleSpinBox::valueChanged, &wMix, &WaterSources::setTotalVolume);
  QObject::connect(&wMix, &WaterSources::totalVolumeChanged, wTotal, &QDoubleSpinBox::setValue);
  heading->addWidget(wTotal, 0, Qt::AlignLeft);
  // Strike water
  heading->addWidget(new QLabel(tr("HG:")), 0, Qt::AlignRight);
  wStrike = new QDoubleSpinBox();
  wStrike->setMinimum(0);
  wStrike->setMaximum(wMix.getTotalVolume());
  wStrike->setDecimals(2);
  wStrike->setSingleStep(0.1);
  wStrike->setSuffix("L");
  wStrike->setValue(wMix.getStrikeWater());
  QObject::connect(wStrike, &QDoubleSpinBox::valueChanged, &wMix, &WaterSources::setStrikeWater);
  QObject::connect(&wMix, &WaterSources::strikeVolumeChanged, wStrike, &QDoubleSpinBox::setValue);
  QObject::connect(&wMix, &WaterSources::maxStrikeSpargingChanged, wStrike, &QDoubleSpinBox::setMaximum);
  heading->addWidget(wStrike, 0, Qt::AlignLeft);
  // Sparging water
  heading->addWidget(new QLabel(tr("NG:")), 0, Qt::AlignRight);
  wSparging = new QDoubleSpinBox();
  wSparging->setMinimum(0);
  wSparging->setMaximum(wMix.getTotalVolume());
  wSparging->setDecimals(2);
  wSparging->setSingleStep(0.1);
  wSparging->setSuffix("L");
  wSparging->setValue(wMix.getSpargingWater());
  QObject::connect(wSparging, &QDoubleSpinBox::valueChanged, &wMix, &WaterSources::setSpargingWater);
  QObject::connect(&wMix, &WaterSources::spargingVolumeChanged, wSparging, &QDoubleSpinBox::setValue);
  QObject::connect(&wMix, &WaterSources::maxStrikeSpargingChanged, wSparging, &QDoubleSpinBox::setMaximum);
  heading->addWidget(wSparging, 0, Qt::AlignLeft);

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
