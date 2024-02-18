// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "water.h"

Water::Water() {}

Water::Water(QString name, float volume, float calzium, float magnesium, float natrium, float hydrogencarbonat,
             float chlorid, float sulfat, float phosphat, float lactat)
    : Meta(name) {
  values[static_cast<uint>(AM::WaterValue::Volume)] = volume;
  values[static_cast<uint>(AM::WaterValue::Calcium)] = calzium;
  values[static_cast<uint>(AM::WaterValue::Magnesium)] = magnesium;
  values[static_cast<uint>(AM::WaterValue::Natrium)] = natrium;
  values[static_cast<uint>(AM::WaterValue::Hydrogencarbonat)] = hydrogencarbonat;
  values[static_cast<uint>(AM::WaterValue::Chlorid)] = chlorid;
  values[static_cast<uint>(AM::WaterValue::Sulfat)] = sulfat;
  values[static_cast<uint>(AM::WaterValue::Phosphat)] = phosphat;
  values[static_cast<uint>(AM::WaterValue::Lactat)] = lactat;
}

float Water::get(AM::WaterValue what) const {
  // stored values
  if (what <= AM::WaterValue::LastAnion) {
    return values[static_cast<uint>(what)];
  }
  // calculated values
  switch (what) {
    case AM::WaterValue::Restalkalitaet:
      return calculateRestalkalitaet();
    case AM::WaterValue::Gesamthaerte:
      return calculateGesamthaerte();
    case AM::WaterValue::Carbonhaerte:
      return calculateCarbonhaerte();
    case AM::WaterValue::NichtCarbonhaerte:
      return calculateNichtCarbonhaerte();
    case AM::WaterValue::CaHaerte:
      return calculateCaHaerte();
    case AM::WaterValue::MgHaerte:
      return calculateMgHaerte();
    case AM::WaterValue::SO4ClVerhaeltnis:
      return calculateSO4ClVerhaeltnis();
    default:
      return -1;
  }
}

void Water::set(AM::WaterValue what, float value) {
  // only stored values
  if (what <= AM::WaterValue::LastAnion) {
    values[static_cast<uint>(what)] = value;
    edited();
  }
};

Water Water::fromJson(const QJsonObject& json) {
  Water ret;
  ret.Meta::fromJson(json);
  for (int i = 0; i <= static_cast<int>(AM::WaterValue::LastAnion); i++) {
    const QString& key = AM::waterStrings[i][AM::JsonKey];
    ret.values[i] = json[key].toDouble(0);
  }
  return ret;
}

QJsonObject Water::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  for (int i = 0; i <= static_cast<int>(AM::WaterValue::LastAnion); i++) {
    const QString& key = AM::waterStrings[i][AM::JsonKey];
    json[key] = values[i];
  }
  return json;
}

QJsonObject Water::profileToJson() const {
  QJsonObject json = toJson();
  json.remove("Volume");  // no volue in profile
  return json;
}

Water& Water::operator+=(const Water& rhs) {
  float volThis = this->values[static_cast<uint>(AM::WaterValue::Volume)];
  float volRhs = rhs.values[static_cast<uint>(AM::WaterValue::Volume)];
  float volSum = volThis + volRhs;
  for (int i = static_cast<uint>(AM::WaterValue::Volume) + 1; i <= static_cast<int>(AM::WaterValue::LastCation); i++) {
    this->values[i] = (this->values[i] * volThis + rhs.values[i] * volRhs) / volSum;
  }
  this->values[static_cast<uint>(AM::WaterValue::Volume)] = volSum;
  return *this;
}

Water Water::operator+(const Water& rhs) {
  //  QString tmpname = this->name + "+" + rhs.name;
  //  float wSum = this->menge + rhs.menge;
  //  Water sum(tmpname, sum, (this->calzium * this->menge + rhs.calzium * rhs.menge) / wSum,
  //            (this->magnesium * this->menge + rhs.magnesium * rhs.menge) / wSum,
  //            (this->natrium * this->menge + rhs.natrium * rhs.menge) / wSum,
  //            (this->hydrogencarbonat * this->menge + rhs.hydrogencarbonat * rhs.menge) / wSum,
  //            (this->chlorid * this->menge + rhs.chlorid * rhs.menge) / wSum,
  //            (this->sulfat * this->menge + rhs.sulfat * rhs.menge) / wSum,
  //            (this->phosphat * this->menge + rhs.phosphat * rhs.menge) / wSum,
  //            (this->lactat * this->menge + rhs.lactat * rhs.menge) / wSum);
  Water sum(*this);
  sum += rhs;
  sum.setName(this->getName() + "+" + rhs.getName());
  return sum;
}

float Water::calculateGesamthaerte() const {
  return calculateCaHaerte() + calculateMgHaerte();
}

float Water::calculateCaHaerte() const {
  return 0.14 * get(AM::WaterValue::Calcium);
}

float Water::calculateMgHaerte() const {
  return 0.23 * get(AM::WaterValue::Magnesium);
}

float Water::calculateCarbonhaerte() const {
  return get(AM::WaterValue::Hydrogencarbonat) / 61.017 * 2.8;
}

float Water::calculateNichtCarbonhaerte() const {
  return calculateGesamthaerte() - calculateCarbonhaerte();
}

float Water::calculateSO4ClVerhaeltnis() const {
  if (get(AM::WaterValue::Chlorid) != 0)
    return get(AM::WaterValue::Sulfat) / get(AM::WaterValue::Chlorid);
  else
    return HUGE_VAL;
}

float Water::calculateRestalkalitaet() const {
  return calculateCarbonhaerte() - calculateCaHaerte() / 3.5 - calculateMgHaerte() / 7;
}
