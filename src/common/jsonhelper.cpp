// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "jsonhelper.h"

#include <QFile>
#include <QJsonDocument>

QJsonObject JsonHelper::loadFile(const QString& path) {
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("Unable to open JSON file");
    return QJsonObject();
  }
  QJsonDocument jsonDoc(QJsonDocument::fromJson(file.readAll()));
  file.close();
  return jsonDoc.object();
}

bool JsonHelper::saveFile(const QString& path, const QJsonObject& json) {
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning("Unable to save JSON file");
    return false;
  }
  file.write(QJsonDocument(json).toJson());
  file.close();
  return true;
}

void JsonHelper::mergeJson(QJsonObject& into, const QJsonObject from) {
  for (auto key : from.keys()) {
    into[key] = from[key];
  }
}
