// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "settings.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

Settings::Settings(QObject* parent) : QAbstractTableModel{parent} {
  // init all values
  for (int i = 0; i < static_cast<int>(Water::Value::Size); i++) {
    for (int j = 0; j < 2; j++) {
      limits[i][j] = 0;
    }
    negative[i] = false;
    logarithmic[i] = false;
  }
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
  const bool ret = Meta::fromJson(jSettings);
  beginResetModel();
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(Water::Value::Size); i++) {
    // get sub object
    const QJsonValue setting = jSettings[Water::waterStrings[i][static_cast<int>(Water::Idx::JsonKey)]];
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
  for (int i = 1; i < static_cast<int>(Water::Value::Size); i++) {
    // create settings object
    QJsonObject setting;
    setting["Min"] = limits[i][0];
    setting["Max"] = limits[i][1];
    setting["AllowNegative"] = negative[i];
    setting["LogarithmicScale"] = logarithmic[i];
    // add object to main json
    inner[Water::waterStrings[i][static_cast<int>(Water::Idx::JsonKey)]] = setting;
  }
  QJsonObject outer;
  outer["Settings"] = inner;
  return outer;
}

float Settings::getMin(Water::Value what) const {
  if (what < Water::Value::Size) {
    return limits[static_cast<uint>(what)][0];
  }
  return -1;
}

float Settings::getMax(Water::Value what) const {
  if (what < Water::Value::Size) {
    return limits[static_cast<uint>(what)][1];
  }
  return -1;
}

void Settings::setMin(Water::Value what, float value) {
  if (what < Water::Value::Size) {
    limits[static_cast<uint>(what)][0] = value;
    setChanged(true);
  }
}

void Settings::setMax(Water::Value what, float value) {
  if (what < Water::Value::Size) {
    limits[static_cast<uint>(what)][1] = value;
    setChanged(true);
  }
}

bool Settings::isNegativeAllowed(Water::Value what) const {
  if (what < Water::Value::Size) {
    return negative[static_cast<uint>(what)];
  }
  return false;
}

void Settings::setNegativeAllowed(Water::Value what, bool value) {
  if (what < Water::Value::Size) {
    negative[static_cast<uint>(what)] = value;
    setChanged(true);
  }
}

bool Settings::isLogarithmicScale(Water::Value what) const {
  if (what < Water::Value::Size) {
    return logarithmic[static_cast<uint>(what)];
  }
  return false;
}

void Settings::setLogarithmicScale(Water::Value what, bool value) {
  if (what < Water::Value::Size) {
    logarithmic[static_cast<uint>(what)] = value;
    setChanged(true);
  }
}

bool Settings::isChanged() const {
  return changed;
}

int Settings::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return static_cast<int>(Water::Value::Size) - 1;
}

int Settings::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // Min, Max, negative, logarithimc
  return 3;  // TODO do not hide logarithmic setting
}

QVariant Settings::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return {};
  }
  if (role != Qt::DisplayRole) {
    return {};
  }
  const qsizetype row = index.row() + 1;  // Skip volume
  if (row < 0 || row >= static_cast<int>(Water::Value::Size)) {
    return {};
  }
  const qsizetype col = index.column();
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
      return {};
      break;
  }
  return {};
}

QVariant Settings::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return {};
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
        return {};
    }
  } else {
    const int idx = section + 1;  // skip volume
    if (idx > 0 && idx < static_cast<int>(Water::Value::Size)) {
      if (!Water::waterStrings[idx][static_cast<int>(Water::Idx::Unit)].isEmpty()) {
        return Water::translatableStrings[idx] + " (" + Water::waterStrings[idx][static_cast<int>(Water::Idx::Unit)] +
               ")";
      }
      return Water::translatableStrings[idx];
    }
  }
  return {};
}

bool Settings::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (!index.isValid()) {
    return false;
  }
  if (role != Qt::EditRole) {
    return false;
  }
  const qsizetype row = index.row() + 1;  // Skip volume
  if (row < 0 || row >= static_cast<int>(Water::Value::Size)) {
    return false;
  }

  const qsizetype col = index.column();
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
  return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void Settings::load() {
  const QString fileName = "settings.json";
  Download::loadDefaults(fileName);
  const QString file = Paths::dataDir() + "/" + fileName;
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Settings::save() {
  const QString file = Paths::dataDir() + "/settings.json";
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
