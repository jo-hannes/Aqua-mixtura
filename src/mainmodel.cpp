// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainmodel.h"

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
    sources->fromJson(loadJson(sourcesFile));
  }
}

void MainModel::saveSources() {
  saveJson(sourcesFile, sources->toJson());
}

void MainModel::saveSources(const QString& path) {
  saveJson(path, sources->toJson());
}

QJsonObject MainModel::loadJson(const QString& path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Unable to open JSON file");
    return QJsonObject();
  }
  QJsonDocument jsonDoc(QJsonDocument::fromJson(file.readAll()));
  file.close();
  return jsonDoc.object();
}

bool MainModel::saveJson(const QString& path, const QJsonObject& json) {
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("Unable to save JSON file");
    return false;
  }
  file.write(QJsonDocument(json).toJson());
  file.close();
  return true;
}
