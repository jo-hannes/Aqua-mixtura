// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "settings.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

Settings::Settings(QObject* parent) : QAbstractTableModel{parent} {
  // init all values
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    for (int j = 0; j < 2; j++) {
      limits[i][j] = 0;
    }
  }
  changed = false;
}

Settings::Settings(const QJsonObject& json) {
  fromJson(json);
}

bool Settings::fromJson(const QJsonObject& json) {
  if (!json.contains("Settings")) {
    qWarning("No valid settings in JSON found");
    return false;
  }
  if (!json["Settings"].isObject()) {
    qWarning("No valid settings in JSON found");
    return false;
  }
  QJsonObject jSettings = json["Settings"].toObject();
  bool ret = Meta::fromJson(jSettings);
  beginResetModel();
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // get sub object
    QJsonValue limit = jSettings[AM::waterStrings[i][AM::JsonKey]];
    limits[i][0] = limit["Min"].toDouble(0);
    limits[i][1] = limit["Max"].toDouble(0);
  }
  endResetModel();
  setChanged(false);
  return ret;
}

QJsonObject Settings::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // create limit object
    QJsonObject limit;
    limit["Min"] = limits[i][0];
    limit["Max"] = limits[i][1];
    // add object to main json
    inner[AM::waterStrings[i][AM::JsonKey]] = limit;
  }
  QJsonObject outer;
  outer["Settings"] = inner;
  return outer;
}

float Settings::getMin(AM::WaterValue what) const {
  if (what < AM::WaterValue::Size) {
    return limits[static_cast<uint>(what)][0];
  }
  return -1;
}

float Settings::getMax(AM::WaterValue what) const {
  if (what < AM::WaterValue::Size) {
    return limits[static_cast<uint>(what)][1];
  }
  return -1;
}

void Settings::setMin(AM::WaterValue what, float value) {
  if (what < AM::WaterValue::Size) {
    limits[static_cast<uint>(what)][0] = value;
    setChanged(true);
  }
}

void Settings::setMax(AM::WaterValue what, float value) {
  if (what < AM::WaterValue::Size) {
    limits[static_cast<uint>(what)][1] = value;
    setChanged(true);
  }
}

bool Settings::isChanged() const {
  return changed;
}

int Settings::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return static_cast<int>(AM::WaterValue::Size) - 1;
}

int Settings::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // only Min + Max column
  return 2;
}

QVariant Settings::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  qsizetype row = index.row() + 1;  // Skip volume
  if (row < 0 || row >= static_cast<int>(AM::WaterValue::Size)) {
    return QVariant();
  }
  qsizetype col = index.column();
  if (col < 0 || col >= 2) {
    return QVariant();
  }
  return limits[row][col];
}

QVariant Settings::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    // Min Max
    switch (section) {
      case 0:
        return tr("Minimum");
      case 1:
        return tr("Maximum");
      default:
        return QVariant();
    }
  } else {
    int idx = section + 1;  // skip volume
    if (idx > 0 && idx < static_cast<int>(AM::WaterValue::Size)) {
      // TODO use display strings instead of JSON keys
      if (!AM::waterStrings[idx][AM::Unit].isEmpty()) {
        return AM::waterStrings[idx][AM::JsonKey] + " (" + AM::waterStrings[idx][AM::Unit] + ")";
      } else {
        return AM::waterStrings[idx][AM::JsonKey];
      }
    }
  }
  return QVariant();
}

bool Settings::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (!index.isValid()) {
    return false;
  }
  if (role != Qt::EditRole) {
    return false;
  }
  qsizetype row = index.row() + 1;  // Skip volume
  if (row < 0 || row >= static_cast<int>(AM::WaterValue::Size)) {
    return false;
  }
  qsizetype col = index.column();
  if (col < 0 || col >= 2) {
    return false;
  }
  limits[row][col] = value.toFloat();
  setChanged(true);
  return true;
}

Qt::ItemFlags Settings::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void Settings::load() {
  QString file = Paths::dataDir() + "/settings.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Settings::save() {
  QString file = Paths::dataDir() + "/settings.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
}

void Settings::setChanged(bool changed) {
  this->changed = changed;
  if (changed) {
    updateEditTime();
  }
  emit dataModified();
}
