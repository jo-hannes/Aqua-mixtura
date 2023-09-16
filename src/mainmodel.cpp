// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainmodel.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

MainModel::MainModel() {
  configDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
  if (configDir.isEmpty()) {
    qFatal("Unable to get storage location");
  }
  QDir().mkpath(configDir);

  sources = new WaterSources();
  sourcesFile = configDir + "/sources.json";
  if (QFile::exists(sourcesFile)) {
    sources->load(sourcesFile);
  }

  // sources.save("save.json");
}

void MainModel::saveSources() {
  sources->save(sourcesFile);
}

void MainModel::saveSources(const QString& path) {
  sources->save(path);
}
