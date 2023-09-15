// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "waterprofile.h"

#include <limits>

WaterProfile::WaterProfile() {}

WaterProfile::WaterProfile(QString name, float calzium, float magnesium, float natrium, float hydrogencarbonat,
                           float chlorid, float sulfat, float phosphat, float lactat)
    : name(name) {
  setCalzium(calzium);
  setMagnesium(magnesium);
  setNatrium(natrium);
  setHydrogencarbonat(hydrogencarbonat);
  setChlorid(chlorid);
  setSulfat(sulfat);
  setPhosphat(phosphat);
  setLactat(lactat);
}

QString WaterProfile::getName() const {
  return name;
}

void WaterProfile::setName(const QString& newName) {
  name = newName;
};

float WaterProfile::getCalzium() const {
  return calzium;
}

void WaterProfile::setCalzium(float newCalzium) {
  calzium = newCalzium;
}

float WaterProfile::getMagnesium() const {
  return magnesium;
}

void WaterProfile::setMagnesium(float newMagnesium) {
  magnesium = newMagnesium;
}

float WaterProfile::getNatrium() const {
  return natrium;
}

void WaterProfile::setNatrium(float newNatrium) {
  natrium = newNatrium;
}

float WaterProfile::getHydrogencarbonat() const {
  return hydrogencarbonat;
}

void WaterProfile::setHydrogencarbonat(float newHydrogencarbonat) {
  hydrogencarbonat = newHydrogencarbonat;
}

float WaterProfile::getChlorid() const {
  return chlorid;
}

void WaterProfile::setChlorid(float newChlorid) {
  chlorid = newChlorid;
}

float WaterProfile::getSulfat() const {
  return sulfat;
}

void WaterProfile::setSulfat(float newSulfat) {
  sulfat = newSulfat;
}

float WaterProfile::getPhosphat() const {
  return phosphat;
}

void WaterProfile::setPhosphat(float newPhosphat) {
  phosphat = newPhosphat;
}

float WaterProfile::getLactat() const {
  return lactat;
}

void WaterProfile::setLactat(float newLactat) {
  lactat = newLactat;
}

float WaterProfile::getGesamthaerte() const {
  return getCaHaerte() + getMgHaerte();
}

float WaterProfile::getCaHaerte() const {
  return 0.14 * calzium;
}

float WaterProfile::getMgHaerte() const {
  return 0.23 * magnesium;
}

float WaterProfile::getCarbonhaerte() const {
  return hydrogencarbonat / 61.017 * 2.8;
}

float WaterProfile::getNichtCarbonhaerte() const {
  return getGesamthaerte() - getCarbonhaerte();
}

float WaterProfile::getSO4ClVerhaeltnis() const {
  if (chlorid != 0)
    return (float)sulfat / chlorid;
  else
    return HUGE_VAL;
}

float WaterProfile::getRestalkalitaet() const {
  return getCarbonhaerte() - getCaHaerte() / 3.5 - getMgHaerte() / 7;
}

WaterProfile WaterProfile::fromJson(const QJsonObject& json) {
  WaterProfile ret;
  ret.name = json["Name"].toString("");
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

QJsonObject WaterProfile::toJson() const
{
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
