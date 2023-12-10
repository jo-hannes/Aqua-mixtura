// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "water.h"

#include <limits>

Water::Water() {}

Water::Water(QString name, float volume, float calzium, float magnesium, float natrium, float hydrogencarbonat,
             float chlorid, float sulfat, float phosphat, float lactat) {
  this->name = name;
  set(Type::Volume, volume);
  set(Type::Calcium, calzium);
  set(Type::Magnesium, magnesium);
  set(Type::Natrium, natrium);
  set(Type::Hydrogencarbonat, hydrogencarbonat);
  set(Type::Chlorid, chlorid);
  set(Type::Sulfat, sulfat);
  set(Type::Phosphat, phosphat);
  set(Type::Lactat, lactat);
}

QString Water::getName() const {
  return name;
}

void Water::setName(const QString& newName) {
  name = newName;
}

float Water::get(Type what) const {
  // stored values
  if (what <= Type::LastAnion) {
    return values[static_cast<uint>(what)];
  }
  // calculated values
  switch (what) {
    case Type::Restalkalitaet:
      return calculateRestalkalitaet();
    case Type::Gesamthaerte:
      return calculateGesamthaerte();
    case Type::Carbonhaerte:
      return calculateCarbonhaerte();
    case Type::NichtCarbonhaerte:
      return calculateNichtCarbonhaerte();
    case Type::CaHaerte:
      return calculateCaHaerte();
    case Type::MgHaerte:
      return calculateMgHaerte();
    case Type::SO4ClVerhaeltnis:
      return calculateSO4ClVerhaeltnis();
    default:
      return -1;
  }
}

void Water::set(Type what, float value) {
  // only stored values
  if (what <= Type::LastAnion) {
    values[static_cast<uint>(what)] = value;
  }
};

Water Water::fromJson(const QJsonObject& json) {
  Water ret;
  ret.name = json["Name"].toString("");
  for (int i = 0; i < static_cast<int>(Type::TypeSize); i++) {
    const QString& key = ret.strings[i][1];
    if (!key.isEmpty()) {
      ret.values[i] = json[key].toDouble(0);
    }
  }
  return ret;
}

QJsonObject Water::toJson() const {
  QJsonObject json;
  json["Name"] = name;
  for (int i = 0; i < static_cast<int>(Type::TypeSize); i++) {
    const QString& key = strings[i][1];
    if (!key.isEmpty()) {
      json[key] = values[i];
    }
  }
  return json;
}

QJsonObject Water::profileToJson() const {
  QJsonObject json = toJson();
  json.remove("Volume");  // no volue in profile
  return json;
}

Water& Water::operator+=(const Water& rhs) {
  float volThis = this->values[static_cast<uint>(Type::Volume)];
  float volRhs = rhs.values[static_cast<uint>(Type::Volume)];
  float volSum = volThis + volRhs;
  for (int i = static_cast<uint>(Type::Volume) + 1; i <= static_cast<int>(Type::LastCation); i++) {
    this->values[i] = (this->values[i] * volThis + rhs.values[i] * volRhs) / volSum;
  }
  this->values[static_cast<uint>(Type::Volume)] = volSum;
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
  sum.setName(this->name + "+" + rhs.name);
  return sum;
}

float Water::calculateGesamthaerte() const {
  return calculateCaHaerte() + calculateMgHaerte();
}

float Water::calculateCaHaerte() const {
  return 0.14 * get(Type::Calcium);
}

float Water::calculateMgHaerte() const {
  return 0.23 * get(Type::Magnesium);
}

float Water::calculateCarbonhaerte() const {
  return get(Type::Hydrogencarbonat) / 61.017 * 2.8;
}

float Water::calculateNichtCarbonhaerte() const {
  return calculateGesamthaerte() - calculateCarbonhaerte();
}

float Water::calculateSO4ClVerhaeltnis() const {
  if (get(Type::Chlorid) != 0)
    return get(Type::Sulfat) / get(Type::Chlorid);
  else
    return HUGE_VAL;
}

float Water::calculateRestalkalitaet() const {
  return calculateCarbonhaerte() - calculateCaHaerte() / 3.5 - calculateMgHaerte() / 7;
}
