// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additive.h"

Additive::Additive() {}

Additive::Additive(const QJsonObject& json) {
  fromJson(json);
}

float Additive::get(Value what) const {
  if (what < Value::Size) {
    return amount[static_cast<uint>(what)];
  }
  return -1;
}

void Additive::set(Value what, float value) {
  if (what < Value::Size) {
    amount[static_cast<uint>(what)] = value;
    updateEditTime();
  }
}

float Additive::getConcentration(Value what) const {
  // only liqids have a concentration
  if (what <= Value::lastLiquid) {
    return concentration[static_cast<uint>(what)];
  } else {
    return 100;  // not a liquid => concentration = 100%
  }
}

void Additive::setConcentration(Value what, float value) {
  if (what <= Value::lastLiquid) {
    if (value < 0) {
      value = 0;
    } else if (value > 100) {
      value = 100;
    }
    concentration[static_cast<uint>(what)] = value;
    updateEditTime();
  }
}

bool Additive::isEnabled(Value what) const {
  if (what < Value::Size) {
    return enabled[static_cast<uint>(what)];
  }
  return false;
}

void Additive::enable(Value what, bool enable) {
  if (what < Value::Size) {
    enabled[static_cast<uint>(what)] = enable;
    updateEditTime();
  }
}

bool Additive::fromJson(const QJsonObject& json) {
  if (!json.contains("WaterAdditives")) {
    qWarning("No valid additives in JSON found");
    return false;
  }
  if (!json["WaterAdditives"].isObject()) {
    qWarning("No valid additives in JSON found");
    return false;
  }
  QJsonObject additives = json["WaterAdditives"].toObject();

  Meta::fromJson(additives);

  for (int i = 0; i < static_cast<int>(Value::Size); i++) {
    QString jsonKey = strings[i][static_cast<uint>(StringIdx::JsonKey)];
    enabled[i] = additives.contains(jsonKey);
    if (i <= static_cast<int>(Value::lastLiquid)) {
      QJsonValue jVal = additives[jsonKey];
      amount[i] = jVal["amount"].toDouble(0);
      concentration[i] = jVal["concentration"].toDouble(0);
    } else {
      amount[i] = additives[jsonKey].toDouble(0);
    }
  }
  return true;
}

QJsonObject Additive::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (int i = 0; i < static_cast<int>(Value::Size); i++) {
    if (enabled[i]) {
      QString jsonKey = strings[i][static_cast<uint>(StringIdx::JsonKey)];
      if (i <= static_cast<int>(Value::lastLiquid)) {
        QJsonObject jObj;
        jObj["amount"] = amount[i];
        jObj["concentration"] = concentration[i];
        inner[jsonKey] = jObj;
      } else {
        inner[jsonKey] = amount[i];
      }
    }
  }
  QJsonObject outer;
  outer["WaterAdditives"] = inner;
  return outer;
}
