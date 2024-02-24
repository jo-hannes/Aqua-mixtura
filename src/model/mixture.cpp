// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixture.h"

Mixture::Mixture(QString name) : Meta(name) {}

Mixture Mixture::fromJson(const QJsonObject& json) {
  Mixture ret;
  ret.Meta::fromJson(json);
  // TODO
  return ret;
}

QJsonObject Mixture::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  // TODO
  return json;
}

void Mixture::AddWater(Water water) {
  waters.append(water);
}

void Mixture::ClearWater() {
  waters.clear();
}

void Mixture::RemoveWaterAt(qsizetype i) {
  waters.removeAt(i);
}

Water Mixture::calc() {
  // start with empty water
  Water result("Result");

  // mix water sources
  for (Water& w : waters) {
    result += w;
  }
  // Add additives
  // TODO

  return result;
}
