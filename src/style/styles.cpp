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
  unsavedChanges = false;
}

Styles::Styles(const QJsonObject& json) {
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
  setUnsaved(false);
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
  setUnsaved(true);
  return true;
}

bool Styles::exportStyle(const QString& path, qsizetype i) {
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
  setUnsaved(true);
}

void Styles::deleteStyle(qsizetype i) {
  if (i >= 0 && i < styles.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    delete styles[i];
    styles.removeAt(i);
    endRemoveRows();
    setUnsaved(true);
  }
}

void Styles::setSaved() {
  setUnsaved(false);
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

bool Styles::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (!index.isValid()) {
    return false;
  }
  if (index.row() < 0 || index.row() >= styles.size()) {
    return false;
  }
  if (role != Qt::EditRole) {
    return false;
  }
  styles[index.row()]->setName(value.toString());
  setUnsaved(true);
  return true;
}

Qt::ItemFlags Styles::flags(const QModelIndex &index) const
{
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
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
}

void Styles::clear() {
  for (const auto style : styles) {
    delete style;
  }
  styles.clear();
}

void Styles::setUnsaved(bool unsaved) {
  if (this->unsavedChanges != unsaved) {
    this->unsavedChanges = unsaved;
    emit unsavedStyles(unsaved);
  }
  emit dataModified();
}
