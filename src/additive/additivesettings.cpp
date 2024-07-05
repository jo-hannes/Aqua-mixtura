// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivesettings.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

AdditiveSettings::AdditiveSettings() {
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    concentration[i] = 10;  // default concentration = 10%
  }
  unit = LiquidUnit::gramm;
}

AdditiveSettings::AdditiveSettings(const QJsonObject& json) {
  AdditiveSettings();
  fromJson(json);
}

bool AdditiveSettings::fromJson(const QJsonObject& json) {
  if (!json.contains("AdditiveSettings")) {
    qWarning("No valid additive settings in JSON found");
    return false;
  }
  if (!json["AdditiveSettings"].isObject()) {
    qWarning("No valid additive settings in JSON found");
    return false;
  }
  QJsonObject asettings = json["AdditiveSettings"].toObject();

  Meta::fromJson(asettings);

  if (asettings["liquidUnit"].toString("") == "milliLiter") {
    unit = LiquidUnit::milliLiter;
  } else {
    unit = LiquidUnit::gramm;
  }
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    QString jsonKey = Additive::strings[i][static_cast<uint>(Additive::StringIdx::JsonKey)];
    concentration[i] = asettings[jsonKey].toDouble(0);
  }
  emit dataModified();
  return true;
}

QJsonObject AdditiveSettings::toJson() const {
  QJsonObject outer;
  toJson(outer);
  return outer;
}

void AdditiveSettings::toJson(QJsonObject& json) const {
  QJsonObject inner;
  Meta::toJson(inner);
  if (unit == LiquidUnit::milliLiter) {
    inner["liquidUnit"] = "milliLiter";
  } else {
    inner["liquidUnit"] = "gramm";
  }
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    QString jsonKey = Additive::strings[i][static_cast<uint>(Additive::StringIdx::JsonKey)];
    inner[jsonKey] = concentration[i];
  }
  json["AdditiveSettings"] = inner;
}

float AdditiveSettings::getConcentration(Additive::Value what) const {
  // only liqids have a concentration
  if (what <= Additive::Value::lastLiquid) {
    return concentration[static_cast<uint>(what)];
  } else {
    return 100;  // not a liquid => concentration = 100%
  }
}

void AdditiveSettings::setConcentration(Additive::Value what, float value) {
  if (what <= Additive::Value::lastLiquid) {
    if (value < 1) {
      value = 1;
    } else if (value > 100) {
      value = 100;
    }
    concentration[static_cast<uint>(what)] = value;
    updateEditTime();
    emit dataModified();
  }
}

AdditiveSettings::LiquidUnit AdditiveSettings::getLiquidUnit() const {
  return unit;
}

void AdditiveSettings::setLiquidUnit(LiquidUnit newUnit) {
  unit = newUnit;
  emit dataModified();
}

void AdditiveSettings::load() {
  QString file = Paths::dataDir() + "/additive.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void AdditiveSettings::save() {
  QString file = Paths::dataDir() + "/additive.json";
  JsonHelper::saveFile(file, this->toJson());
}
