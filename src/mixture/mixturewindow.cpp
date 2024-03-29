// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixturewindow.h"

#include "mixadditivewidget.h"
#include "mixmaltwidget.h"
#include "mixwaterwidget.h"

#include <QVBoxLayout>

MixtureWindow::MixtureWindow(Mixture& mixture, WaterSources* waterDb, Additive* additiveDb, Malts* maltDb,
                             Styles* styleDb, Limits* limits, QWidget* parent)
    : QWidget{parent}, mix{mixture} {
  this->setWindowFlags(Qt::Window);
  updateName();

  QHBoxLayout* layout = new QHBoxLayout(this);
  this->setLayout(layout);

  QVBoxLayout* row1 = new QVBoxLayout(this);
  layout->addLayout(row1);

  // Water widget
  MixWaterWidget* mww = new MixWaterWidget(mix.waters, waterDb, this);
  row1->addWidget(mww);

  // Malt widget
  MixMaltWidget* mmw = new MixMaltWidget(mix.malts, maltDb, this);
  row1->addWidget(mmw);

  // Additive widget
  MixAdditiveWidget* maw = new MixAdditiveWidget(mix.additive, additiveDb);
  layout->addWidget(maw);

  // TODO
  // Need widgets for:
  // * style
  // * Results
}

void MixtureWindow::updateName() {
  this->setWindowTitle("Aqua mixtura - " + tr("Aufbereitung: ") + mix.getName());
}
