// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "water.h"

#include <cassert>

Water::Water() {
  if (strTranslate[0].isEmpty()) {
    // NOLINTBEGIN(*-magic-numbers)
    strTranslate[0] = QObject::tr("Volumen");
    strTranslate[1] = QObject::tr("Calcium");
    strTranslate[2] = QObject::tr("Magnesium");
    strTranslate[3] = QObject::tr("Natrium");
    strTranslate[4] = QObject::tr("Hydrogencarbonat");
    strTranslate[5] = QObject::tr("Chlorid");
    strTranslate[6] = QObject::tr("Sulfat");
    strTranslate[7] = QObject::tr("Phosphat");
    strTranslate[8] = QObject::tr("Lactat");
    strTranslate[9] = QObject::tr("Restalkalität");
    strTranslate[10] = QObject::tr("Gesamthärte");
    strTranslate[11] = QObject::tr("Carbonhärte");
    strTranslate[12] = QObject::tr("Nichtcarbonhärte");
    strTranslate[13] = QObject::tr("Ca-Härte");
    strTranslate[14] = QObject::tr("Mg-Härte");
    strTranslate[15] = QObject::tr("SO₄/Cl-Verhältnis");
    strTranslate[16] = QObject::tr("Maische pH");
    // NOLINTEND(*-magic-numbers)
  }
}

Water::Water(QString name, double volume, double calzium, double magnesium, double natrium, double hydrogencarbonat,
             double chlorid, double sulfat, double phosphat, double lactat)
    : Water() {
  this->setName(name);
  values[static_cast<uint>(Value::Volume)] = volume;
  values[static_cast<uint>(Value::Calcium)] = calzium;
  values[static_cast<uint>(Value::Magnesium)] = magnesium;
  values[static_cast<uint>(Value::Natrium)] = natrium;
  values[static_cast<uint>(Value::Hydrogencarbonat)] = hydrogencarbonat;
  values[static_cast<uint>(Value::Chlorid)] = chlorid;
  values[static_cast<uint>(Value::Sulfat)] = sulfat;
  values[static_cast<uint>(Value::Phosphat)] = phosphat;
  values[static_cast<uint>(Value::Lactat)] = lactat;
}

Water::Water(const QJsonObject& json) : Water() {
  fromJson(json);
}

// NOLINTBEGIN(misc-no-recursion)
// get is called by calculate*() functions but this is no real recursion
double Water::get(Value what) const {
  // stored values
  const auto idx = static_cast<std::size_t>(what);
  if (idx < values.size()) {
    return values.at(idx);
  }
  // calculated values
  switch (what) {
    case Value::Restalkalitaet:
      return calculateRestalkalitaet();
    case Value::Gesamthaerte:
      return calculateGesamthaerte();
    case Value::Carbonhaerte:
      return calculateCarbonhaerte();
    case Value::NichtCarbonhaerte:
      return calculateNichtCarbonhaerte();
    case Value::CaHaerte:
      return calculateCaHaerte();
    case Value::MgHaerte:
      return calculateMgHaerte();
    case Value::SO4ClVerhaeltnis:
      return calculateSO4ClVerhaeltnis();
    default:
      return -1;
  }
}

void Water::set(Value what, double value) {
  // only stored values
  const auto idx = static_cast<std::size_t>(what);
  if (idx < values.size()) {
    values.at(idx) = value;
    updateEditTime();
  }
};

bool Water::fromJson(const QJsonObject& json) {
  const bool ret = Meta::fromJson(json);
  assert(strJsonKey.size() >= values.size());
  for (uint i = 0; i < values.size(); i++) {
    const QString& key = strJsonKey.at(i);
    values.at(i) = json[key].toDouble(0);
  }
  return ret;
}

QJsonObject Water::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  for (uint i = 0; i < values.size(); i++) {
    const QString& key = strJsonKey.at(i);
    json[key] = values.at(i);
  }
  return json;
}

QJsonObject Water::profileToJson() const {
  QJsonObject json = toJson();
  json.remove("Volume");  // no volume in profile
  return json;
}

Water& Water::operator+=(const Water& rhs) {
  const double volThis = this->values[static_cast<uint>(Value::Volume)];
  const double volRhs = rhs.values[static_cast<uint>(Value::Volume)];
  const double volSum = volThis + volRhs;
  if (volSum == 0) {  // avoid zero division
    return *this;
  }
  for (uint i = static_cast<uint>(Value::Volume) + 1; i < this->values.size(); i++) {
    this->values.at(i) = (this->values.at(i) * volThis + rhs.values.at(i) * volRhs) / volSum;
  }
  this->values[static_cast<uint>(Value::Volume)] = volSum;
  return *this;
}

Water Water::operator+(const Water& rhs) {
  //  QString tmpname = this->name + "+" + rhs.name;
  //  double wSum = this->menge + rhs.menge;
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

double Water::calculateGesamthaerte() const {
  return calculateCaHaerte() + calculateMgHaerte();
}

double Water::calculateCaHaerte() const {
  return 0.14 * get(Value::Calcium);  // NOLINT(*-magic-numbers)
}

double Water::calculateMgHaerte() const {
  return 0.23 * get(Value::Magnesium);  // NOLINT(*-magic-numbers)
}

double Water::calculateCarbonhaerte() const {
  return get(Value::Hydrogencarbonat) / 61.017 * 2.8;  // NOLINT(*-magic-numbers)
}

double Water::calculateNichtCarbonhaerte() const {
  return calculateGesamthaerte() - calculateCarbonhaerte();
}

double Water::calculateSO4ClVerhaeltnis() const {
  if (get(Value::Chlorid) != 0) {
    return get(Value::Sulfat) / get(Value::Chlorid);
  }
  return HUGE_VAL;
}

double Water::calculateRestalkalitaet() const {
  return calculateCarbonhaerte() - calculateCaHaerte() / 3.5 - calculateMgHaerte() / 7;  // NOLINT(*-magic-numbers)
}
// NOLINTEND(misc-no-recursion)
