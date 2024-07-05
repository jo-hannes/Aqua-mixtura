// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainmodel.h"

#include "common/paths.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

MainModel::MainModel() {
  QString configDir = Paths::dataDir();
  QDir().mkpath(configDir);

  mixtures = new Mixtures();

  load();
}

void MainModel::load() {
  mixtures->settings.load();
  mixtures->waterDb->load();
  mixtures->additiveCfg.load();
  mixtures->maltDb->load();
  mixtures->styleDb->load();
  mixtures->load();
}

void MainModel::save() {
  mixtures->settings.save();
  mixtures->waterDb->save();
  mixtures->additiveCfg.save();
  mixtures->maltDb->save();
  mixtures->styleDb->save();
  mixtures->save();
}
