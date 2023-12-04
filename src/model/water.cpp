// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "water.h"

#include <limits>

Water::Water() {}

Water::Water(QString name, float menge, float calzium, float magnesium, float natrium, float hydrogencarbonat,
             float chlorid, float sulfat, float phosphat, float lactat) {
  this->name = name;
  this->volume = menge;
  setCalzium(calzium);
  setMagnesium(magnesium);
  setNatrium(natrium);
  setHydrogencarbonat(hydrogencarbonat);
  setChlorid(chlorid);
  setSulfat(sulfat);
  setPhosphat(phosphat);
  setLactat(lactat);
}

QString Water::getName() const {
  return name;
}

void Water::setName(const QString& newName) {
  name = newName;
};

float Water::getVolume() const {
  return volume;
}

void Water::setVolume(float newMenge) {
  volume = newMenge;
}

float Water::getCalzium() const {
  return calzium;
}

void Water::setCalzium(float newCalzium) {
  calzium = newCalzium;
}

float Water::getMagnesium() const {
  return magnesium;
}

void Water::setMagnesium(float newMagnesium) {
  magnesium = newMagnesium;
}

float Water::getNatrium() const {
  return natrium;
}

void Water::setNatrium(float newNatrium) {
  natrium = newNatrium;
}

float Water::getHydrogencarbonat() const {
  return hydrogencarbonat;
}

void Water::setHydrogencarbonat(float newHydrogencarbonat) {
  hydrogencarbonat = newHydrogencarbonat;
}

float Water::getChlorid() const {
  return chlorid;
}

void Water::setChlorid(float newChlorid) {
  chlorid = newChlorid;
}

float Water::getSulfat() const {
  return sulfat;
}

void Water::setSulfat(float newSulfat) {
  sulfat = newSulfat;
}

float Water::getPhosphat() const {
  return phosphat;
}

void Water::setPhosphat(float newPhosphat) {
  phosphat = newPhosphat;
}

float Water::getLactat() const {
  return lactat;
}

void Water::setLactat(float newLactat) {
  lactat = newLactat;
}

float Water::getGesamthaerte() const {
  return getCaHaerte() + getMgHaerte();
}

float Water::getCaHaerte() const {
  return 0.14 * calzium;
}

float Water::getMgHaerte() const {
  return 0.23 * magnesium;
}

float Water::getCarbonhaerte() const {
  return hydrogencarbonat / 61.017 * 2.8;
}

float Water::getNichtCarbonhaerte() const {
  return getGesamthaerte() - getCarbonhaerte();
}

float Water::getSO4ClVerhaeltnis() const {
  if (chlorid != 0)
    return sulfat / chlorid;
  else
    return HUGE_VAL;
}

float Water::getRestalkalitaet() const {
  return getCarbonhaerte() - getCaHaerte() / 3.5 - getMgHaerte() / 7;
}

Water Water::fromJson(const QJsonObject& json) {
  Water ret;
  ret.name = json["Name"].toString("");
  ret.volume = json["volume"].toDouble(0);
  ret.calzium = json["Calzium"].toDouble(0);
  ret.magnesium = json["Magnesium"].toDouble(0);
  ret.natrium = json["Natrium"].toDouble(0);
  ret.hydrogencarbonat = json["Hydrogencarbonat"].toDouble(0);
  ret.chlorid = json["Chlorid"].toDouble(0);
  ret.sulfat = json["Sulfat"].toDouble(0);
  ret.phosphat = json["Phosphat"].toDouble(0);
  ret.lactat = json["Lactat"].toDouble(0);
  return ret;
}

QJsonObject Water::toJson() const {
  QJsonObject json = profileToJson();
  json["volume"] = volume;
  return json;
}

QJsonObject Water::profileToJson() const {
  QJsonObject json;
  json["Name"] = name;
  json["Calzium"] = calzium;
  json["Magnesium"] = magnesium;
  json["Natrium"] = natrium;
  json["Hydrogencarbonat"] = hydrogencarbonat;
  json["Chlorid"] = chlorid;
  json["Sulfat"] = sulfat;
  json["Phosphat"] = phosphat;
  json["Lactat"] = lactat;
  return json;
}

Water& Water::operator+=(const Water& rhs) {
  float wSum = this->volume + rhs.volume;
  this->calzium = (this->calzium * this->volume + rhs.calzium * rhs.volume) / wSum;
  this->magnesium = (this->magnesium * this->volume + rhs.magnesium * rhs.volume) / wSum;
  this->natrium = (this->natrium * this->volume + rhs.natrium * rhs.volume) / wSum;
  this->hydrogencarbonat = (this->hydrogencarbonat * this->volume + rhs.hydrogencarbonat * rhs.volume) / wSum;
  this->chlorid = (this->chlorid * this->volume + rhs.chlorid * rhs.volume) / wSum;
  this->sulfat = (this->sulfat * this->volume + rhs.sulfat * rhs.volume) / wSum;
  this->phosphat = (this->phosphat * this->volume + rhs.phosphat * rhs.volume) / wSum;
  this->lactat = (this->lactat * this->volume + rhs.lactat * rhs.volume) / wSum;
  this->volume = wSum;
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
