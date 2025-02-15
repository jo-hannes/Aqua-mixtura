// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivesettings.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

AdditiveSettings::AdditiveSettings() {
  for (auto& c : concentration) {
    // NOLINTNEXTLINE(*-magic-numbers)
    c = 10;  // default concentration = 10%
  }
}

AdditiveSettings::AdditiveSettings(const QJsonObject& json) : AdditiveSettings() {
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
  for (uint i = 0; i < concentration.size(); i++) {
    const QString jsonKey = Additive::strJsonKey.at(i);
    concentration.at(i) = asettings[jsonKey].toDouble(0);
  }
  setChanged(false);
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
    const QString jsonKey = Additive::strJsonKey.at(i);
    inner[jsonKey] = concentration.at(i);
  }
  json["AdditiveSettings"] = inner;
}

double AdditiveSettings::getConcentration(Additive::Value what) const {
  // only liqids have a concentration
  const auto idx = static_cast<uint>(what);
  if (idx < concentration.size()) {
    return concentration.at(idx);
  }
  // NOLINTNEXTLINE(*-magic-numbers)
  return 100;  // not a liquid => concentration = 100%
}

void AdditiveSettings::setConcentration(Additive::Value what, double value) {
  const auto idx = static_cast<uint>(what);
  if (idx < concentration.size()) {
    if (value < 1) {
      value = 1;
    } else if (value > 100) {
      value = 100;
    }
    concentration.at(idx) = value;
    setChanged(true);
  }
}

AdditiveSettings::LiquidUnit AdditiveSettings::getLiquidUnit() const {
  return unit;
}

void AdditiveSettings::setLiquidUnit(LiquidUnit newUnit) {
  unit = newUnit;
  setChanged(true);
}

double AdditiveSettings::getDensity(Additive::Value what) const {
  // only liqids have a density
  if ((what <= Additive::Value::lastLiquid) && (unit == LiquidUnit::milliLiter)) {
    const auto idx = static_cast<uint>(what);
    // first check for linear
    if (densityCoefficients.at(idx)[2] == 0) {
      // linear
      return densityCoefficients.at(idx)[0] + densityCoefficients.at(idx)[1] * concentration.at(idx);
    } else {  // NOLINT(readability-else-after-return)
      // qubic
      return densityCoefficients.at(idx)[0] + densityCoefficients.at(idx)[1] * concentration.at(idx) +
             densityCoefficients.at(idx)[2] * concentration.at(idx) * concentration.at(idx);
    }
  } else {
    return 1;  // not a liquid or unit is not ml => just return 1
  }
}

bool AdditiveSettings::isChanged() const {
  return changed;
}

void AdditiveSettings::load() {
  const QString file = Paths::dataDir() + "/additive.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void AdditiveSettings::save() {
  const QString file = Paths::dataDir() + "/additive.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
}

void AdditiveSettings::setChanged(bool changed) {
  this->changed = changed;
  if (changed) {
    updateEditTime();
  }
  emit dataModified();
}
