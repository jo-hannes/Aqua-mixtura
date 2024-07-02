// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivesettings.h"

AdditiveSettings::AdditiveSettings() {
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    enabled[i] = false;
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      concentration[i] = 10;  // default concentration = 10%
    }
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
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    QString jsonKey = Additive::strings[i][static_cast<uint>(Additive::StringIdx::JsonKey)];
    QJsonValue jVal = asettings[jsonKey];
    enabled[i] = jVal["enabled"].toBool();
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      concentration[i] = jVal["concentration"].toDouble(0);
    }
  }
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
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    QString jsonKey = Additive::strings[i][static_cast<uint>(Additive::StringIdx::JsonKey)];
    QJsonObject jObj;
    jObj["enabled"] = enabled[i];
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      jObj["concentration"] = concentration[i];
    }
    inner[jsonKey] = jObj;
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
    // emit dataModified();
  }
}

bool AdditiveSettings::isEnabled(Additive::Value what) const {
  if (what < Additive::Value::Size) {
    return enabled[static_cast<uint>(what)];
  }
  return false;
}

void AdditiveSettings::enable(Additive::Value what, bool enable) {
  if (what < Additive::Value::Size) {
    enabled[static_cast<uint>(what)] = enable;
    updateEditTime();
    // emit dataModified();
  }
}

AdditiveSettings::LiquidUnit AdditiveSettings::getLiquidUnit() const {
  return unit;
}

void AdditiveSettings::setLiquidUnit(LiquidUnit newUnit) {
  unit = newUnit;
  // emit dataModified();
}
