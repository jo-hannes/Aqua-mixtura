// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixture.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QJsonArray>
#include <QJsonValue>

Mixture::Mixture() {}

Mixture::Mixture(QString path) {
  this->path = path;
  load();
}

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

void Mixture::resetPath() {
  path.clear();
}

QString Mixture::getPath() const {
  if (path.isEmpty()) {
    return Paths::dataDir() + "/" + this->getUuid() + ".json";
  } else {
    return path;
  }
}

void Mixture::load() {
  path = getPath();
  if (QFile::exists(path)) {
    this->fromJson(JsonHelper::loadFile(path));
  }
}

void Mixture::save() const {
  JsonHelper::saveFile(this->getPath(), this->toJson());
}

Water Mixture::calc() {
  return *additive + waters->getMix();
}
