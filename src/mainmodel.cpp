// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainmodel.h"

#include "common/jsonhelper.h"

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

  malts = new Malts();
  maltsFile = configDir + "/malts.json";
  if (QFile::exists(maltsFile)) {
    malts->fromJson(JsonHelper::loadFile(maltsFile));
  }

  styles = new Styles();
  stylesFile = configDir + "/styles.json";
  loadStyles();

  mixtures = new Mixtures();
  mixturesFile = configDir + "/mixtures.json";
  loadMixtures();
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

void MainModel::saveMalts() {
  JsonHelper::saveFile(maltsFile, malts->toJson());
}

void MainModel::saveMalts(const QString& path) {
  JsonHelper::saveFile(path, malts->toJson());
}

void MainModel::loadStyles() {
  if (QFile::exists(stylesFile)) {
    styles->fromJson(JsonHelper::loadFile(stylesFile));
  }
}

void MainModel::saveStyles() {
  JsonHelper::saveFile(stylesFile, styles->toJson());
}

void MainModel::saveStyles(const QString& path) {
  JsonHelper::saveFile(path, styles->toJson());
}

void MainModel::loadMixtures() {
  if (QFile::exists(mixturesFile)) {
    mixtures->fromJson(JsonHelper::loadFile(mixturesFile));
  }
}

void MainModel::saveMixtures() {
  JsonHelper::saveFile(mixturesFile, mixtures->toJson());
}
