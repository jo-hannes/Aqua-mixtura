// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixturewindow.h"

#include "mixwaterwidget.h"

#include <QVBoxLayout>

MixtureWindow::MixtureWindow(Mixture& mixture, WaterSources* waterDb, QWidget* parent) : QWidget{parent}, mix{mixture} {
  this->setWindowFlags(Qt::Window);
  updateName();

  QVBoxLayout* layout = new QVBoxLayout(this);
  this->setLayout(layout);
  // QLabel* tst = new QLabel("Test");
  // layout->addWidget(tst);
  MixWaterWidget* mww = new MixWaterWidget(mix.waters, waterDb);
  layout->addWidget(mww);

  // TODO
  // Need widgets for:
  // * Waters => Table View? => KISS => Yes Table View
  // * Additive
  // * malts
  // * style
  // * Results
}

void MixtureWindow::updateName() {
  this->setWindowTitle(tr("Aufbereitung: ") + mix.getName());
}
