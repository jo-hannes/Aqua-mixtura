// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additive.h"

Additive::Additive() : changed{false} {
  if (translatableStrings[0].isEmpty()) {
    // NOLINTBEGIN(*-magic-numbers)
    translatableStrings[0] = tr("Milchsäure");
    translatableStrings[1] = tr("Salzsäure");
    translatableStrings[2] = tr("Schwefelsäure");
    translatableStrings[3] = tr("Phosphorsäure");
    translatableStrings[4] = tr("Calciumsulfat (Braugips)");
    translatableStrings[5] = tr("Calciumchlorid");
    translatableStrings[6] = tr("Natriumchlorid (Kochsalz)");
    translatableStrings[7] = tr("Natriumhydrogencarbonat (Natron)");
    translatableStrings[8] = tr("Magnesiumchlorid");
    translatableStrings[9] = tr("Magnesiumsulfat");
    translatableStrings[10] = tr("Calciumcarbonat");
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
    amount[idx] = value;  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index): idx is checked
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
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
    const QString jsonKey = strings[i][static_cast<uint>(StringIdx::JsonKey)];
    amount[i] = additives[jsonKey].toDouble(0);
    // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
  }
  setChanged(false);
  return true;
}

QJsonObject Additive::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (uint i = 0; i < amount.size(); i++) {
    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
    const QString jsonKey = strings[i][static_cast<uint>(StringIdx::JsonKey)];
    inner[jsonKey] = amount[i];
    // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
  }
  QJsonObject outer;
  outer["WaterAdditives"] = inner;
  return outer;
}

Water Additive::operator+(const Water& rhs) const {
  // first just copy values
  double values[static_cast<int>(Water::Value::LastAnion) + 1];
  for (int w = 0; w <= static_cast<int>(Water::Value::LastAnion); w++) {
    values[w] = rhs.get(static_cast<Water::Value>(w));
  }

  // sotre volume for later calculations
  const double volume = rhs.get(Water::Value::Volume);

  // loop over all water values
  for (int w = 0; w <= static_cast<int>(Water::Value::LastAnion); w++) {
    double mg = 0;  // mg added or removed
    // loop over all additive
    for (int a = 0; a < static_cast<int>(Value::Size); a++) {
      mg += amount[a] * calculationMatrix[a][w];
    }
    // add it to the water
    values[w] += mg / volume;
  }

  // store calculated values in new water object
  Water result(rhs.getName());
  for (int w = 0; w <= static_cast<int>(Water::Value::LastAnion); w++) {
    result.set(static_cast<Water::Value>(w), values[w]);
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
