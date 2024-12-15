// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "water.h"

Water::Water() {
  if (translatableStrings[0].isEmpty()) {
    // NOLINTBEGIN(*-magic-numbers)
    translatableStrings[0] = QObject::tr("Volumen");
    translatableStrings[1] = QObject::tr("Calcium");
    translatableStrings[2] = QObject::tr("Magnesium");
    translatableStrings[3] = QObject::tr("Natrium");
    translatableStrings[4] = QObject::tr("Hydrogencarbonat");
    translatableStrings[5] = QObject::tr("Chlorid");
    translatableStrings[6] = QObject::tr("Sulfat");
    translatableStrings[7] = QObject::tr("Phosphat");
    translatableStrings[8] = QObject::tr("Lactat");
    translatableStrings[9] = QObject::tr("Restalkalität");
    translatableStrings[10] = QObject::tr("Gesamthärte");
    translatableStrings[11] = QObject::tr("Carbonhärte");
    translatableStrings[12] = QObject::tr("Nichtcarbonhärte");
    translatableStrings[13] = QObject::tr("Ca-Härte");
    translatableStrings[14] = QObject::tr("Mg-Härte");
    translatableStrings[15] = QObject::tr("SO₄/Cl-Verhältnis");
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

double Water::get(Value what) const {
  // stored values
  if (what <= Value::LastAnion) {
    return values[static_cast<uint>(what)];
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
  if (what <= Value::LastAnion) {
    values[static_cast<uint>(what)] = value;
    updateEditTime();
  }
};

bool Water::fromJson(const QJsonObject& json) {
  const bool ret = Meta::fromJson(json);
  for (int i = 0; i <= static_cast<int>(Value::LastAnion); i++) {
    const QString& key = waterStrings[i][static_cast<int>(Idx::JsonKey)];
    values[i] = json[key].toDouble(0);
  }
  return ret;
}

QJsonObject Water::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  for (int i = 0; i <= static_cast<int>(Value::LastAnion); i++) {
    const QString& key = waterStrings[i][static_cast<int>(Idx::JsonKey)];
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
  const double volThis = this->values[static_cast<uint>(Value::Volume)];
  const double volRhs = rhs.values[static_cast<uint>(Value::Volume)];
  const double volSum = volThis + volRhs;
  if (volSum == 0) {  // avoid zero division
    return *this;
  }
  for (int i = static_cast<uint>(Value::Volume) + 1; i < static_cast<int>(Value::LastAnion); i++) {
    this->values[i] = (this->values[i] * volThis + rhs.values[i] * volRhs) / volSum;
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
