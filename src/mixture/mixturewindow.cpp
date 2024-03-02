// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixturewindow.h"

MixtureWindow::MixtureWindow(Mixture& mixture, QWidget* parent) : QWidget{parent}, mix{mixture} {
  this->setWindowFlags(Qt::Window);
  updateName();
}

void MixtureWindow::updateName() {
  this->setWindowTitle(tr("Aufbereitung: ") + mix.getName());
}
