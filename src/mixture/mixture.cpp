// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixture.h"

#include <QJsonArray>
#include <QJsonValue>

Mixture::Mixture(QString name) : Meta(name) {}

Mixture::Mixture(const QJsonObject& json) {
  fromJson(json);
}

bool Mixture::fromJson(const QJsonObject& json) {
  bool ret = Meta::fromJson(json);

  // TODO
  // read waters
  QJsonValue jWaters = json["Waters"];
  waters.clear();
  if (jWaters.isArray()) {
    for (const auto& water : jWaters.toArray()) {
      waters.append(Water(water.toObject()));
    }
  }
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
