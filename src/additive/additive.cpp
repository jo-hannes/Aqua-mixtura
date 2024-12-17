// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additive.h"

Additive::Additive() : changed{false} {
  if (strTranslate[0].isEmpty()) {
    // NOLINTBEGIN(*-magic-numbers)
    strTranslate[0] = tr("Milchsäure");
    strTranslate[1] = tr("Salzsäure");
    strTranslate[2] = tr("Schwefelsäure");
    strTranslate[3] = tr("Phosphorsäure");
    strTranslate[4] = tr("Calciumsulfat (Braugips)");
    strTranslate[5] = tr("Calciumchlorid");
    strTranslate[6] = tr("Natriumchlorid (Kochsalz)");
    strTranslate[7] = tr("Natriumhydrogencarbonat (Natron)");
    strTranslate[8] = tr("Magnesiumchlorid");
    strTranslate[9] = tr("Magnesiumsulfat");
    strTranslate[10] = tr("Calciumcarbonat");
    // NOLINTEND(*-magic-numbers)
  }
}

Additive::Additive(const QJsonObject& json) : Additive() {
  fromJson(json);
}

double Additive::get(Value what) const {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < amount.size()) {
    return amount.at(idx);
  }
  return -1;
}

void Additive::set(Value what, double value) {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < amount.size()) {
    amount.at(idx) = value;
    setChanged(true);
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

  for (uint i = 0; i < amount.size(); i++) {
    // i is inside range of array
    const QString jsonKey = strJsonKey.at(i);
    amount.at(i) = additives[jsonKey].toDouble(0);
  }
  setChanged(false);
  return true;
}

QJsonObject Additive::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (uint i = 0; i < amount.size(); i++) {
    const QString jsonKey = strJsonKey.at(i);
    inner[jsonKey] = amount.at(i);
  }
  QJsonObject outer;
  outer["WaterAdditives"] = inner;
  return outer;
}

Water Additive::operator+(const Water& rhs) const {
  // first just copy values
  std::array<double, calculationMatrix[0].size()> values{};
  // double values[static_cast<int>(Water::Value::LastAnion) + 1];
  for (uint w = 0; w < values.size(); w++) {
    values.at(w) = rhs.get(static_cast<Water::Value>(w));
  }

  // sotre volume for later calculations
  const double volume = rhs.get(Water::Value::Volume);

  // loop over all water values
  for (uint w = 0; w < calculationMatrix[0].size(); w++) {
    double mg = 0;  // mg added or removed
    // loop over all additive
    for (uint a = 0; a < calculationMatrix.size(); a++) {
      mg += amount.at(a) * calculationMatrix.at(a).at(w);
    }
    // add it to the water
    values.at(w) += mg / volume;
  }

  // store calculated values in new water object
  Water result(rhs.getName());
  for (int w = 0; w <= static_cast<int>(Water::Value::LastAnion); w++) {
    result.set(static_cast<Water::Value>(w), values.at(w));
  }
  return result;
}

bool Additive::isChanged() const {
  return changed;
}

void Additive::setChanged(bool changed) {
  this->changed = changed;
  if (changed) {
    updateEditTime();
  }
  emit dataModified();
}
