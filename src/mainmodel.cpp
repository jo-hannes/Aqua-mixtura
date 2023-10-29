// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainmodel.h"

#include "model/jsonhelper.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
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
    sources->fromJson(JsonHelper::loadFile(sourcesFile));
  }

  additiveFile = configDir + "/additive.json";
  if (QFile::exists(sourcesFile)) {
    additive = new Additive(Additive::fromJson(JsonHelper::loadFile(additiveFile)));
  } else {
    additive = new Additive();
  }
}

void MainModel::saveSources() {
  JsonHelper::saveFile(sourcesFile, sources->toJson());
}

void MainModel::saveSources(const QString& path) {
  JsonHelper::saveFile(path, sources->toJson());
}

void MainModel::saveAdditive() {
  JsonHelper::saveFile(additiveFile, additive->toJson());
}

void MainModel::saveAdditive(const QString& path) {
  JsonHelper::saveFile(path, additive->toJson());
}
