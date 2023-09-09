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

float WaterProfile::getGesamthaerte() {
  return getCaHaerte() + getMgHaerte();
}

float WaterProfile::getCaHaerte() {
  return 0.14 * calzium;
}

float WaterProfile::getMgHaerte() {
  return 0.23 * magnesium;
}

float WaterProfile::getCarbonhaerte() {
  return hydrogencarbonat / 61.017 * 2.8;
}

float WaterProfile::getNichtCarbonhaerte() {
  return getGesamthaerte() - getCarbonhaerte();
}

float WaterProfile::getSO4ClVerhaeltnis() {
  if (chlorid != 0)
    return (float)sulfat / chlorid;
  else
    return HUGE_VAL;
}

float WaterProfile::getRestalkalitaet() {
  return getCarbonhaerte() - getCaHaerte() / 3.5 - getMgHaerte() / 7;
}
