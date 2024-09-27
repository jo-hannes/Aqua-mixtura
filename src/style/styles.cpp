// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "styles.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Styles::Styles() {
  noStyle = new Style("No Style");
  changed = false;
}

Styles::Styles(const QJsonObject& json) {
  Styles();
  fromJson(json);
}

Styles::~Styles() {
  clear();
  delete noStyle;
}

bool Styles::fromJson(const QJsonObject& json) {
  if (!json.contains("BeerStyles")) {
    qWarning("No valid styles in JSON found");
    return false;
  }
  QJsonValue jsonStyles = json["BeerStyles"];
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
  } else {
    return noStyle;
  }
}

void Styles::addStyle(Style* style) {
  qsizetype i = styles.size();
  beginInsertRows(QModelIndex(), i, i);
  styles.append(style);
  endInsertRows();
  setChanged(true);
}

void Styles::deleteStyle(qsizetype i) {
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
  return styles.size();
}

QVariant Styles::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  qsizetype row = index.row();
  if (row < 0 || row >= styles.size()) {
    return QVariant();
  }
  return styles.at(row)->getName();
}

void Styles::load()
{
  QString file = Paths::dataDir() + "/styles.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Styles::save()
{
  QString file = Paths::dataDir() + "/styles.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
  for (const auto style : styles) {
    style->saved();
  }
}

void Styles::setChanged(bool changed) {
  this->changed = changed;
  emit dataModified();
}

void Styles::clear() {
  for (const auto style : styles) {
    delete style;
  }
  styles.clear();
}
