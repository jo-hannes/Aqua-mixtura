// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "settings.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

Settings::Settings(QObject* parent) : QAbstractTableModel{parent} {
  // All arrays need to have the same size
  assert(limits.size() == negative.size());
  assert(limits.size() == logarithmic.size());
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
  for (uint i = 1; i < limits.size(); i++) {
    // get sub object
    const QJsonValue setting = jSettings[Water::strJsonKey.at(i)];
    limits.at(i).at(0) = setting["Min"].toDouble(0);
    limits.at(i).at(1) = setting["Max"].toDouble(0);
    negative.at(i) = setting["AllowNegative"].toBool(false);
    logarithmic.at(i) = setting["LogarithmicScale"].toBool(false);
  }
  endResetModel();
  setChanged(false);
  return ret;
}

QJsonObject Settings::toJson() const {
  QJsonObject inner;
  Meta::toJson(inner);
  for (uint i = 1; i < limits.size(); i++) {
    // create settings object
    QJsonObject setting;
    setting["Min"] = limits.at(i).at(0);
    setting["Max"] = limits.at(i).at(1);
    setting["AllowNegative"] = negative.at(i);
    setting["LogarithmicScale"] = logarithmic.at(i);
    // add object to main json
    inner[Water::strJsonKey.at(i)] = setting;
  }
  QJsonObject outer;
  outer["Settings"] = inner;
  return outer;
}

double Settings::getMin(Water::Value what) const {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < limits.size()) {
    return limits.at(idx).at(0);
  }
  return -1;
}

double Settings::getMax(Water::Value what) const {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < limits.size()) {
    return limits.at(idx).at(1);
  }
  return -1;
}

void Settings::setMin(Water::Value what, double value) {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < limits.size()) {
    limits.at(idx).at(0) = value;
    setChanged(true);
  }
}

void Settings::setMax(Water::Value what, double value) {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < limits.size()) {
    limits.at(idx).at(1) = value;
    setChanged(true);
  }
}

bool Settings::isNegativeAllowed(Water::Value what) const {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < negative.size()) {
    return negative.at(idx);
  }
  return false;
}

void Settings::setNegativeAllowed(Water::Value what, bool value) {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < negative.size()) {
    negative.at(idx) = value;
    setChanged(true);
  }
}

bool Settings::isLogarithmicScale(Water::Value what) const {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < logarithmic.size()) {
    return logarithmic.at(idx);
  }
  return false;
}

void Settings::setLogarithmicScale(Water::Value what, bool value) {
  const auto idx = static_cast<std::size_t>(what);
  if (idx < logarithmic.size()) {
    logarithmic.at(idx) = value;
    setChanged(true);
  }
}

bool Settings::isChanged() const {
  return changed;
}

int Settings::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return limits.size() - 1;  // NOLINT(*-narrowing-conversions): QAbstractTableModel requires int
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
  if (row < 0 || row >= limits.size()) {
    return {};
  }
  const qsizetype col = index.column();
  switch (col) {
    case 0:
    case 1:
      return limits.at(row).at(col);
      break;
    case 2:
      return negative.at(row);
      break;
    case 3:
      return logarithmic.at(row);
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
    if (idx > 0 && idx < limits.size()) {
      if (!Water::strUnit.at(idx).isEmpty()) {
        return Water::strTranslate.at(idx) + " (" + Water::strUnit.at(idx) + ")";
      }
      return Water::strTranslate.at(idx);
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
  if (row < 0 || row >= limits.size()) {
    return false;
  }

  const qsizetype col = index.column();
  switch (col) {
    case 0:
    case 1:
      limits.at(row).at(col) = value.toDouble();
      setChanged(true);
      return true;
    case 2:
      negative.at(row) = value.toBool();
      setChanged(true);
      return true;
    case 3:
      return logarithmic.at(row) = value.toBool();
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
