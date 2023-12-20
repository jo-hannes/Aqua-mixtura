// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "style.h"

#include "QJsonValue"

Style::Style(QString name) {
  this->name = name;
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    limited[i] = false;
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limits[i][j] = 0;
    }
  }
}

QString Style::getName() const {
  return name;
}

void Style::setName(const QString& newName) {
  name = newName;
}

float Style::get(AM::WaterValue what, Limit limit) const {
  if (what < AM::WaterValue::Size && limit < Limit::Size) {
    return limits[static_cast<uint>(what)][static_cast<uint>(limit)];
  }
  return -1;
}

void Style::set(AM::WaterValue what, Limit limit, float value) {
  if (what < AM::WaterValue::Size && limit < Limit::Size) {
    limits[static_cast<uint>(what)][static_cast<uint>(limit)] = value;
  }
}

bool Style::isLimited(AM::WaterValue what)
{
  if (what < AM::WaterValue::Size) {
    return limited[static_cast<uint>(what)];
  }
  return false;
}

void Style::limit(AM::WaterValue what, bool limit)
{
  if (what < AM::WaterValue::Size) {
    limited[static_cast<uint>(what)] = limit;
  }
}

Style Style::fromJson(const QJsonObject& json) {
  Style ret;
  ret.name = json["Name"].toString("");

  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // get sub object
    QJsonValue limit = json[AM::waterStrings[i][AM::JsonKey]];
    ret.limited[i] = limit.isObject();
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      ret.limits[i][j] = limit[jsonKeys[j]].toDouble(0);
    }
  }
  return ret;
}

QJsonObject Style::toJson() const {
  QJsonObject json;
  json["Name"] = name;
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // skip values not limited
    if (!limited[i]) {
      continue;
    }
    // create limit object
    QJsonObject limit;
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limit[jsonKeys[j]] = limits[i][j];
    }
    // add object to main json
    json[AM::waterStrings[i][AM::JsonKey]] = limit;
  }
  return json;
}
