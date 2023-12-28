// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "style.h"

#include "QJsonValue"

Style::Style(QString name) {
  this->name = name;
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    limited[i] = false;
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limits[i][j] = 0;
    }
  }
}

Style::Style(const QJsonObject& json) {
  name = json["Name"].toString("");
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // get sub object
    QJsonValue limit = json[AM::waterStrings[i][AM::JsonKey]];
    limited[i] = limit.isObject();
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limits[i][j] = limit[jsonKeys[j]].toDouble(0);
    }
  }
}

QJsonObject Style::toJson() const {
  QJsonObject json;
  json["Name"] = name;
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // skip values not limited
    if (!limited[i]) {
      continue;
    }
    // create limit object
    QJsonObject limit;
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limit[jsonKeys[j]] = limits[i][j];
    }
    // add object to main json
    json[AM::waterStrings[i][AM::JsonKey]] = limit;
  }
  return json;
}

Style* Style::copy() const {
  Style* copy = new Style(this->getName());
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    copy->limited[i] = this->limited[i];
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      copy->limits[i][j] = this->limits[i][j];
    }
  }
  return copy;
}

QString Style::getName() const {
  return name;
}

void Style::setName(const QString& newName) {
  name = newName;
}

float Style::get(AM::WaterValue what, Limit limit) const {
  if (what < AM::WaterValue::Size && limit < Limit::Size) {
    return limits[static_cast<uint>(what)][static_cast<uint>(limit)];
  }
  return -1;
}

void Style::set(AM::WaterValue what, Limit limit, float value) {
  if (what < AM::WaterValue::Size && limit < Limit::Size) {
    limits[static_cast<uint>(what)][static_cast<uint>(limit)] = value;
  }
}

bool Style::isLimited(AM::WaterValue what)
{
  if (what < AM::WaterValue::Size) {
    return limited[static_cast<uint>(what)];
  }
  return false;
}

void Style::limit(AM::WaterValue what, bool limit)
{
  if (what < AM::WaterValue::Size) {
    limited[static_cast<uint>(what)] = limit;
  }
}

int Style::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // limits without volume
  return static_cast<int>(AM::WaterValue::Size) - 1;
}

int Style::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // Enable disable + Limits
  return 1 + static_cast<int>(Limit::Size);
}

QVariant Style::data(const QModelIndex& index, int role) const {
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
  // enable disable
  if (col == 0) {
    return limited[row];
  }
  // values
  if (col > 0 && col < static_cast<int>(Limit::Size) + 1) {
    return limits[row][col - 1];
  }
  return QVariant();
}

QVariant Style::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    // enable disable
    switch (section) {
      case 0:
        return tr("Limitieren");
      case 1:
        return tr("Untergrenze");
      case 2:
        return tr("Untere Empfehlung");
      case 3:
        return tr("Obere Empfehlung");
      case 4:
        return tr("Obergrenze");
      default:
        return QVariant();
    }
  } else {
    int idx = section + 1;  // skip volume
    if (idx > 0 && idx < static_cast<int>(AM::WaterValue::Size)) {
      // TODO use display steings instead of JSON keys
      if (!AM::waterStrings[idx][AM::Unit].isEmpty()) {
        return AM::waterStrings[idx][AM::JsonKey] + " (" + AM::waterStrings[idx][AM::Unit] + ")";
      } else {
        return AM::waterStrings[idx][AM::JsonKey];
      }
    }
  }
  return QVariant();
}

bool Style::setData(const QModelIndex& index, const QVariant& value, int role) {
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
  // enable disable
  if (col == 0) {
    limited[row] = value.toBool();
    return true;
  }
  // values
  if (col > 0 && col < static_cast<int>(Limit::Size) + 1) {
    limits[row][col - 1] = value.toFloat();
    return true;
  }
  // mark as unsaved!!
  // setUnsaved(true);
  return false;
}

Qt::ItemFlags Style::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
