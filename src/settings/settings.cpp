// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "settings.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

Settings::Settings(QObject* parent) : QObject{parent} {}

Settings::Settings(const QJsonObject& json) {
  fromJson(json);
}

bool Settings::fromJson(const QJsonObject& json) {
  if (!json.contains("Settings")) {
    qWarning("No valid settings in JSON found");
    return false;
  }
  if (!json["Settings"].isObject()) {
    qWarning("No valid settings in JSON found");
    return false;
  }

  QJsonObject jSettings = json["Settings"].toObject();

  bool ret = Meta::fromJson(jSettings);
  ret &= waterSettings.fromJson(jSettings);

  return ret;
}

QJsonObject Settings::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  JsonHelper::mergeJson(inner, waterSettings.toJson());
  QJsonObject outer;
  outer["Settings"] = inner;
  return outer;
}

void Settings::load() {
  QString file = Paths::dataDir() + "/settings.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Settings::save() {
  QString file = Paths::dataDir() + "/settings.json";
  JsonHelper::saveFile(file, this->toJson());
}
