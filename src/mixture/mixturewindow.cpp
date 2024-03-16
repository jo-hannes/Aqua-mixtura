// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixturewindow.h"

#include "mixmaltwidget.h"
#include "mixwaterwidget.h"

#include <QVBoxLayout>

MixtureWindow::MixtureWindow(Mixture& mixture, WaterSources* waterDb, Additive* additiveDb, Malts* maltDb,
                             Styles* styleDb, QWidget* parent)
    : QWidget{parent}, mix{mixture} {
  this->setWindowFlags(Qt::Window);
  updateName();

  QVBoxLayout* layout = new QVBoxLayout(this);
  this->setLayout(layout);
  // QLabel* tst = new QLabel("Test");
  // layout->addWidget(tst);

  // Water widget
  MixWaterWidget* mww = new MixWaterWidget(mix.waters, waterDb, this);
  layout->addWidget(mww);

  // Malt widget
  MixMaltWidget* mmw = new MixMaltWidget(mix.malts, maltDb, this);
  layout->addWidget(mmw);

  // TODO
  // Need widgets for:
  // * Additive
  // * style
  // * Results
}

void MixtureWindow::updateName() {
  this->setWindowTitle(tr("Aufbereitung: ") + mix.getName());
}
