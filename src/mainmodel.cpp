// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainmodel.h"

#include "common/jsonhelper.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

MainModel::MainModel() {
  QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
  if (configDir.isEmpty()) {
    qFatal("Unable to get storage location");
  }
  QDir().mkpath(configDir);

  sourcesFile = configDir + "/sources.json";
  additiveFile = configDir + "/additive.json";
  maltsFile = configDir + "/malts.json";
  stylesFile = configDir + "/styles.json";
  mixturesFile = configDir + "/mixtures.json";

  sources = new WaterSources();
  additive = new Additive();
  malts = new Malts();
  styles = new Styles();
  mixtures = new Mixtures();

  load();
}

void MainModel::load() {
  loadSources();
  loadAdditive();
  loadMalts();
  loadStyles();
  loadMixtures();
}

void MainModel::save() {
  saveSources();
  saveAdditive();
  saveMalts();
  saveStyles();
  saveMixtures();
}

void MainModel::loadSources() {
  if (QFile::exists(sourcesFile)) {
    sources->fromJson(JsonHelper::loadFile(sourcesFile));
  }
}

void MainModel::saveSources() {
  JsonHelper::saveFile(sourcesFile, sources->toJson());
}

void MainModel::loadAdditive() {
  if (QFile::exists(additiveFile)) {
    additive->fromJson(JsonHelper::loadFile(additiveFile));
  }
}

void MainModel::saveAdditive() {
  JsonHelper::saveFile(additiveFile, additive->toJson());
}

void MainModel::loadMalts() {
  if (QFile::exists(maltsFile)) {
    malts->fromJson(JsonHelper::loadFile(maltsFile));
  }
}

void MainModel::saveMalts() {
  JsonHelper::saveFile(maltsFile, malts->toJson());
}

void MainModel::loadStyles() {
  if (QFile::exists(stylesFile)) {
    styles->fromJson(JsonHelper::loadFile(stylesFile));
  }
}

void MainModel::saveStyles() {
  JsonHelper::saveFile(stylesFile, styles->toJson());
}

void MainModel::loadMixtures() {
  if (QFile::exists(mixturesFile)) {
    mixtures->fromJson(JsonHelper::loadFile(mixturesFile));
  }
}

void MainModel::saveMixtures() {
  JsonHelper::saveFile(mixturesFile, mixtures->toJson());
}
