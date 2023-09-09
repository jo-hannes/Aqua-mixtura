// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "water.h"

#include <limits>

Water::Water() {}

Water::Water(WaterProfile& profile, float menge) : WaterProfile(profile) {
  this->menge = menge;
}

Water::Water(QString name, float menge, float calzium, float magnesium, float natrium, float hydrogencarbonat,
             float chlorid, float sulfat, float phosphat, float lactat) {
  this->name = name;
  this->menge = menge;
  setCalzium(calzium);
  setMagnesium(magnesium);
  setNatrium(natrium);
  setHydrogencarbonat(hydrogencarbonat);
  setChlorid(chlorid);
  setSulfat(sulfat);
  setPhosphat(phosphat);
  setLactat(lactat);
}

float Water::getMenge() const {
  return menge;
}

void Water::setMenge(float newMenge) {
  menge = newMenge;
}

Water& Water::operator+=(const Water& rhs) {
  float wSum = this->menge + rhs.menge;
  this->calzium = (this->calzium * this->menge + rhs.calzium * rhs.menge) / wSum;
  this->magnesium = (this->magnesium * this->menge + rhs.magnesium * rhs.menge) / wSum;
  this->natrium = (this->natrium * this->menge + rhs.natrium * rhs.menge) / wSum;
  this->hydrogencarbonat = (this->hydrogencarbonat * this->menge + rhs.hydrogencarbonat * rhs.menge) / wSum;
  this->chlorid = (this->chlorid * this->menge + rhs.chlorid * rhs.menge) / wSum;
  this->sulfat = (this->sulfat * this->menge + rhs.sulfat * rhs.menge) / wSum;
  this->phosphat = (this->phosphat * this->menge + rhs.phosphat * rhs.menge) / wSum;
  this->lactat = (this->lactat * this->menge + rhs.lactat * rhs.menge) / wSum;
  this->menge = wSum;
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
