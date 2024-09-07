// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "limits.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

Limits::Limits(QObject* parent) : QAbstractTableModel{parent} {
  // init all values
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    for (int j = 0; j < 2; j++) {
      limits[i][j] = 0;
    }
  }
  changed = false;
}

Limits::Limits(const QJsonObject& json) {
  fromJson(json);
}

bool Limits::fromJson(const QJsonObject& json) {
  if (!json.contains("Limits")) {
    qWarning("No valid limits in JSON found");
    return false;
  }
  if (!json["Limits"].isObject()) {
    qWarning("No valid limits in JSON found");
    return false;
  }

  beginResetModel();
  QJsonObject jLimits = json["Limits"].toObject();

  bool ret = Meta::fromJson(jLimits);
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // get sub object
    QJsonValue limit = jLimits[AM::waterStrings[i][AM::JsonKey]];
    limits[i][0] = limit["Min"].toDouble(0);
    limits[i][1] = limit["Max"].toDouble(0);
  }
  endResetModel();
  setChanged(false);
  return ret;
}

QJsonObject Limits::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // create limit object
    QJsonObject limit;
    limit["Min"] = limits[i][0];
    limit["Max"] = limits[i][1];
    // add object to main json
    inner[AM::waterStrings[i][AM::JsonKey]] = limit;
  }
  QJsonObject outer;
  outer["Limits"] = inner;
  return outer;
}

float Limits::getMin(AM::WaterValue what) const {
  if (what < AM::WaterValue::Size) {
    return limits[static_cast<uint>(what)][0];
  }
  return -1;
}

float Limits::getMax(AM::WaterValue what) const {
  if (what < AM::WaterValue::Size) {
    return limits[static_cast<uint>(what)][1];
  }
  return -1;
}

void Limits::setMin(AM::WaterValue what, float value) {
  if (what < AM::WaterValue::Size) {
    limits[static_cast<uint>(what)][0] = value;
    setChanged(true);
  }
}

void Limits::setMax(AM::WaterValue what, float value) {
  if (what < AM::WaterValue::Size) {
    limits[static_cast<uint>(what)][1] = value;
    setChanged(true);
  }
}

bool Limits::isChanged() const {
  return changed;
}

int Limits::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return static_cast<int>(AM::WaterValue::Size) - 1;
}

int Limits::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // only Min + Max column
  return 2;
}

QVariant Limits::data(const QModelIndex& index, int role) const {
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

QVariant Limits::headerData(int section, Qt::Orientation orientation, int role) const {
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

bool Limits::setData(const QModelIndex& index, const QVariant& value, int role) {
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

Qt::ItemFlags Limits::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void Limits::load() {
  QString file = Paths::dataDir() + "/limits.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Limits::save() {
  QString file = Paths::dataDir() + "/limits.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
}

void Limits::setChanged(bool changed) {
  this->changed = changed;
  if (changed) {
    updateEditTime();
  }
  emit dataModified();
}
