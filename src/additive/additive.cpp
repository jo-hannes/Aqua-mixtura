// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "additive.h"

Additive::Additive() {}

float Additive::getCaso4() const {
  return caso4;
}

void Additive::setCaso4(float newCaso4) {
  caso4 = newCaso4;
}

float Additive::getCacl2() const {
  return cacl2;
}

void Additive::setCacl2(float newCacl2) {
  cacl2 = newCacl2;
}

float Additive::getNacl() const {
  return nacl;
}

void Additive::setNacl(float newNacl) {
  nacl = newNacl;
}

float Additive::getNahco3() const {
  return nahco3;
}

void Additive::setNahco3(float newNahco3) {
  nahco3 = newNahco3;
}

float Additive::getMgcl2() const {
  return mgcl2;
}

void Additive::setMgcl2(float newMgcl2) {
  mgcl2 = newMgcl2;
}

float Additive::getMgso4() const {
  return mgso4;
}

void Additive::setMgso4(float newMgso4) {
  mgso4 = newMgso4;
}

float Additive::getCaco3() const {
  return caco3;
}

void Additive::setCaco3(float newCaco3) {
  caco3 = newCaco3;
}

float Additive::getC3h6o3Volume() const {
  return c3h6o3Volume;
}

void Additive::setC3h6o3Volume(float newC3h6o3Volume) {
  c3h6o3Volume = newC3h6o3Volume;
}

float Additive::getC3h6o3Concentration() const {
  return c3h6o3Concentration;
}

void Additive::setC3h6o3Concentration(float newC3h6o3Concentration) {
  if (newC3h6o3Concentration < 0)
    c3h6o3Concentration = 0;
  else if (newC3h6o3Concentration > 100)
    c3h6o3Concentration = 100;
  else
    c3h6o3Concentration = newC3h6o3Concentration;
}

float Additive::getHclVolume() const {
  return hclVolume;
}

void Additive::setHclVolume(float newHclVolume) {
  hclVolume = newHclVolume;
}

float Additive::getHclConcentration() const {
  return hclConcentration;
}

void Additive::setHclConcentration(float newHclConcentration) {
  if (newHclConcentration < 0)
    hclConcentration = 0;
  else if (newHclConcentration > 100)
    hclConcentration = 100;
  else
    hclConcentration = newHclConcentration;
}

float Additive::getH2so4Volume() const {
  return h2so4Volume;
}

void Additive::setH2so4Volume(float newH2so4Volume) {
  h2so4Volume = newH2so4Volume;
}

float Additive::getH2so4Concentration() const {
  return h2so4Concentration;
}

void Additive::setH2so4Concentration(float newH2so4Concentration) {
  if (newH2so4Concentration < 0)
    h2so4Concentration = 0;
  else if (newH2so4Concentration > 100)
    h2so4Concentration = 100;
  else
    h2so4Concentration = newH2so4Concentration;
}

float Additive::getH3po4Volume() const {
  return h3po4Volume;
}

void Additive::setH3po4Volume(float newH3po4Volume) {
  h3po4Volume = newH3po4Volume;
}

float Additive::getH3po4Concentration() const {
  return h3po4Concentration;
}

void Additive::setH3po4Concentration(float newH3po4Concentration) {
  if (newH3po4Concentration < 0)
    h3po4Concentration = 0;
  else if (newH3po4Concentration > 100)
    h3po4Concentration = 100;
  else
    h3po4Concentration = newH3po4Concentration;
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
  QJsonObject j = json["WaterAdditives"].toObject();
  setCaso4(j["caso4"].toDouble(-1));
  setCacl2(j["cacl2"].toDouble(-1));
  setNacl(j["nacl"].toDouble(-1));
  setNahco3(j["nahco3"].toDouble(-1));
  setMgcl2(j["mgcl2"].toDouble(-1));
  setMgso4(j["mgso4"].toDouble(-1));
  setCaco3(j["caco3"].toDouble(-1));
  setC3h6o3Volume(j["c3h6o3Volume"].toDouble(-1));
  setC3h6o3Concentration(j["c3h6o3Concentration"].toDouble(80));
  setHclVolume(j["hclVolume"].toDouble(-1));
  setHclConcentration(j["hclConcentration"].toDouble(10));
  setH2so4Volume(j["h2so4Volume"].toDouble(-1));
  setH2so4Concentration(j["h2so4Concentration"].toDouble(10));
  setH3po4Volume(j["h3po4Volume"].toDouble(-1));
  setH3po4Concentration(j["h3po4Concentration"].toDouble(10));
  return true;
}

QJsonObject Additive::toJson() const {
  QJsonObject inner;
  inner["caso4"] = caso4;
  inner["cacl2"] = cacl2;
  inner["nacl"] = nacl;
  inner["nahco3"] = nahco3;
  inner["mgcl2"] = mgcl2;
  inner["mgso4"] = mgso4;
  inner["caco3"] = caco3;
  inner["c3h6o3Volume"] = c3h6o3Volume;
  inner["c3h6o3Concentration"] = c3h6o3Concentration;
  inner["hclVolume"] = hclVolume;
  inner["hclConcentration"] = hclConcentration;
  inner["h2so4Volume"] = h2so4Volume;
  inner["h2so4Concentration"] = h2so4Concentration;
  inner["h3po4Volume"] = h3po4Volume;
  inner["h3po4Concentration"] = h3po4Concentration;
  QJsonObject outer;
  outer["WaterAdditives"] = inner;
  return outer;
}
