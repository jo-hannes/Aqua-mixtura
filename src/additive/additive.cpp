// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additive.h"

Additive::Additive() {
  for (int i = 0; i < static_cast<int>(Value::Size); i++) {
    amount[i] = 0;
  }
}

Additive::Additive(const QJsonObject& json) {
  Additive();
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
    emit dataModified();
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
    amount[i] = additives[jsonKey].toDouble(0);
  }
  return true;
}

QJsonObject Additive::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (int i = 0; i < static_cast<int>(Value::Size); i++) {
    QString jsonKey = strings[i][static_cast<uint>(StringIdx::JsonKey)];
    inner[jsonKey] = amount[i];
  }
  QJsonObject outer;
  outer["WaterAdditives"] = inner;
  return outer;
}
