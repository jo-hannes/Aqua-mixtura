// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixture.h"

#include "../common/jsonhelper.h"

#include <QJsonArray>
#include <QJsonValue>

Mixture::Mixture(QString name) : Meta(name) {}

Mixture::Mixture(const QJsonObject& json) {
  fromJson(json);
}

bool Mixture::fromJson(const QJsonObject& json) {
  bool ret = Meta::fromJson(json);
  // read waters
  ret = waters->fromJson(json) && ret;
  // read additive
  ret = additive->fromJson(json) && ret;
  // read malts
  ret = malts->fromJson(json) && ret;
  // read style
  ret = style->fromJson(json["Style"].toObject()) && ret;
  return ret;
}

QJsonObject Mixture::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  JsonHelper::mergeJson(json, waters->toJson());
  JsonHelper::mergeJson(json, additive->toJson());
  JsonHelper::mergeJson(json, malts->toJson());
  json["Style"] = style->toJson();
  return json;
}

void Mixture::AddWater(Water water) {
  waters->addProfile(water);
}

void Mixture::ClearWater() {
  // waters.clear();
}

void Mixture::RemoveWaterAt(qsizetype i) {
  waters->deleteProfile(i);
}

Water Mixture::calc() {
  Water result = waters->getMix();
  // Add additives
  // TODO

  return result;
}
