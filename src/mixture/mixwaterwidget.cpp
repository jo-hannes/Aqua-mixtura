// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixwaterwidget.h"

#include "../common/buttons.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

MixWaterWidget::MixWaterWidget(Mixture& mixture, QWidget* parent) : QFrame{parent}, mix{mixture} {
  // Build ui
  QVBoxLayout* layout = new QVBoxLayout();
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  waterView = new QTableView();
  waterView->setModel(mix.waters);
  waterView->verticalHeader()->setVisible(false);
  layout->addWidget(waterView);

  // Buttons
  Buttons* buttons = new Buttons(tr("Wasser hinzufügen"), "", tr("Wasser löschen"), "", "", "", "");
  QObject::connect(buttons->btnAdd, &QPushButton::clicked, this, &MixWaterWidget::add);
  QObject::connect(buttons->btnDelete, &QPushButton::clicked, this, &MixWaterWidget::remove);
  layout->addWidget(buttons);
}

void MixWaterWidget::add() {
  // get list from waters TODO
}

void MixWaterWidget::remove() {
  QModelIndex idx = waterView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  mix.waters->deleteProfile(idx.row());
}
