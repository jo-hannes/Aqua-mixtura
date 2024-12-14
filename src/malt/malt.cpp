// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "malt.h"

#include <cmath>

Malt::Malt(QString name, float mass, float ebc, float ph) : Meta(name), mass(mass), ebc(ebc), ph(ph) {}

Malt::Malt(const QJsonObject& json) {  // NOLINT(*-member-init)
  fromJson(json);
}

float Malt::getMass() const {
  return mass;
}

void Malt::setMass(float newMass) {
  mass = newMass;
  updateEditTime();
}

float Malt::getEbc() const {
  return ebc;
}

void Malt::setEbc(float newEbc) {
  ebc = newEbc;
  updateEditTime();
}

float Malt::getPh() const {
  return ph;
}

void Malt::setPh(float newPh) {
  ph = newPh;
  updateEditTime();
}

float Malt::calculatePh(float ebc, type type) {
  switch (type) {
    case upto200ebc:
      return Malt::calcualtePhUpTo200Ebc(ebc);
      break;
    case caramalz:
      return Malt::calcualtePhCaramalz(ebc);
      break;
    case roestmalz:
      return Malt::calcualtePhRoestmalz();
      break;
    default:
      return 0;
      break;
  }
}

float Malt::calcualtePhUpTo200Ebc(float ebc) {
  return -0.255 * log(ebc) + 6.156;  // NOLINT(*-magic-numbers)
}

float Malt::calcualtePhCaramalz(float ebc) {
  return -0.289 * log(ebc) + 6.069;  // NOLINT(*-magic-numbers)
}

float Malt::calcualtePhRoestmalz() {
  return 4.65;  // NOLINT(*-magic-numbers)
}

bool Malt::fromJson(const QJsonObject& json) {
  const bool ret = Meta::fromJson(json);
  mass = json["mass"].toDouble(0);
  ebc = json["EBC"].toDouble(0);
  ph = json["pH"].toDouble(7);  // NOLINT(*-magic-numbers)
  return ret;
}

QJsonObject Malt::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  json["mass"] = mass;
  json["EBC"] = ebc;
  json["pH"] = ph;
  return json;
}
