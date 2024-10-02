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
    negative[i] = false;
    logarithmic[i] = false;
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
    QJsonValue setting = jSettings[AM::waterStrings[i][AM::JsonKey]];
    limits[i][0] = setting["Min"].toDouble(0);
    limits[i][1] = setting["Max"].toDouble(0);
    negative[i] = setting["AllowNegative"].toBool(false);
    logarithmic[i] = setting["LogarithmicScale"].toBool(false);
  }
  endResetModel();
  setChanged(false);
  return ret;
}

QJsonObject Settings::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (int i = 1; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // create settings object
    QJsonObject setting;
    setting["Min"] = limits[i][0];
    setting["Max"] = limits[i][1];
    setting["AllowNegative"] = negative[i];
    setting["LogarithmicScale"] = logarithmic[i];
    // add object to main json
    inner[AM::waterStrings[i][AM::JsonKey]] = setting;
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

bool Settings::isNegativeAllowed(AM::WaterValue what) const {
  if (what < AM::WaterValue::Size) {
    return negative[static_cast<uint>(what)];
  }
  return false;
}

void Settings::setNegativeAllowed(AM::WaterValue what, bool value) {
  if (what < AM::WaterValue::Size) {
    negative[static_cast<uint>(what)] = value;
    setChanged(true);
  }
}

bool Settings::isLogarithmicScale(AM::WaterValue what) const {
  if (what < AM::WaterValue::Size) {
    return logarithmic[static_cast<uint>(what)];
  }
  return false;
}

void Settings::setLogarithmicScale(AM::WaterValue what, bool value) {
  if (what < AM::WaterValue::Size) {
    logarithmic[static_cast<uint>(what)] = value;
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
  // Min, Max, negative, logarithimc
  return 3;  // TODO do not hide logarithmic setting
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
  switch (col) {
    case 0:
    case 1:
      return limits[row][col];
      break;
    case 2:
      return negative[row];
      break;
    case 3:
      return logarithmic[row];
      break;
    default:
      return QVariant();
      break;
  }
  return QVariant();
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
      case 2:
        return tr("Negative Werte");
      case 3:
        return tr("Logarithmische Skala");
      default:
        return QVariant();
    }
  } else {
    int idx = section + 1;  // skip volume
    if (idx > 0 && idx < static_cast<int>(AM::WaterValue::Size)) {
      if (!AM::waterStrings[idx][AM::Unit].isEmpty()) {
        return AM::waterStrings[idx][AM::Description] + " (" + AM::waterStrings[idx][AM::Unit] + ")";
      } else {
        return AM::waterStrings[idx][AM::Description];
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
  switch (col) {
    case 0:
    case 1:
      limits[row][col] = value.toFloat();
      setChanged(true);
      return true;
    case 2:
      negative[row] = value.toBool();
      setChanged(true);
      return true;
    case 3:
      return logarithmic[row] = value.toBool();
      setChanged(true);
      return true;
    default:
      return false;
  }
  return false;
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
