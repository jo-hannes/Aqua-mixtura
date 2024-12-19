// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "styles.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Styles::Styles(const QJsonObject& json) : Styles() {
  fromJson(json);
}

Styles::~Styles() {
  clear();
}

bool Styles::fromJson(const QJsonObject& json) {
  if (!json.contains("BeerStyles")) {
    qWarning("No valid styles in JSON found");
    return false;
  }
  const QJsonValue jsonStyles = json["BeerStyles"];
  if (!jsonStyles.isArray()) {
    qWarning("No valid styles in JSON found");
    return false;
  }
  beginResetModel();
  clear();
  for (const auto& style : jsonStyles.toArray()) {
    styles.append(new Style(style.toObject()));
  }
  setChanged(false);
  endResetModel();
  return true;
}

QJsonObject Styles::toJson() const {
  QJsonArray jsonArray;
  for (const auto& style : styles) {
    jsonArray.append(style->toJson());
  }
  QJsonObject json;
  json["BeerStyles"] = jsonArray;
  return json;
}

bool Styles::importStyle(const QString& path) {
  if (path.isEmpty()) {
    return false;
  }
  QJsonObject json = JsonHelper::loadFile(path);
  if (!json.contains("BeerStyle")) {
    return false;
  }
  addStyle(new Style(json["BeerStyle"].toObject()));
  setChanged(true);
  return true;
}

bool Styles::exportStyle(const QString& path, qsizetype i) const {
  if (path.isEmpty()) {
    return false;
  }
  if (i < 0 || i >= styles.size()) {
    return false;
  }
  QJsonObject json;
  json["BeerStyle"] = styles.at(i)->toJson();
  return JsonHelper::saveFile(path, json);
}

Style* Styles::getStyle(qsizetype i) {
  if (i >= 0 && i < styles.size()) {
    return styles[i];
  }
  return &noStyle;
}

void Styles::addStyle(Style* style) {
  const int i = styles.size();  // NOLINT(*-narrowing-conversions): beginInsertRows requires int
  beginInsertRows(QModelIndex(), i, i);
  styles.append(style);
  endInsertRows();
  setChanged(true);
}

void Styles::deleteStyle(int i) {
  if (i >= 0 && i < styles.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    delete styles[i];
    styles.removeAt(i);
    endRemoveRows();
    setChanged(true);
  }
}

bool Styles::isChanged() const {
  return changed;
}

int Styles::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return styles.size();  // NOLINT(*-narrowing-conversions): using int because of QAbstractListModel
}

QVariant Styles::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return {};
  }
  if (role != Qt::DisplayRole) {
    return {};
  }
  const qsizetype row = index.row();
  if (row < 0 || row >= styles.size()) {
    return {};
  }
  return styles.at(row)->getName();
}

void Styles::load() {
  const QString fileName = "styles.json";
  Download::loadDefaults(fileName);
  const QString file = Paths::dataDir() + "/" + fileName;
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Styles::save() {
  const QString file = Paths::dataDir() + "/styles.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
  for (auto* const style : styles) {
    style->setChanged(false);
  }
}

void Styles::setChanged(bool changed) {
  this->changed = changed;
  emit dataModified();
}

void Styles::clear() {
  for (auto* const style : styles) {
    delete style;
  }
  styles.clear();
}
