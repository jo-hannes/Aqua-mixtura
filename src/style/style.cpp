// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "style.h"

#include "QJsonValue"

Style::Style(QString name) : Meta(name) {
  for (int i = 0; i < static_cast<int>(Water::Value::Size); i++) {
    limited[i] = false;
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limits[i][j] = 0;
    }
  }
}

Style::Style(const QJsonObject& json) {
  fromJson(json);
}

bool Style::fromJson(const QJsonObject& json) {
  const bool ret = Meta::fromJson(json);
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(Water::Value::Size); i++) {
    // get sub object
    const QJsonValue limit = json[Water::waterStrings[i][static_cast<int>(Water::Idx::JsonKey)]];
    limited[i] = limit.isObject();
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      limits[i][j] = limit[jsonKeys[j]].toDouble(0);
    }
  }
  changed = false;
  return ret;
}

QJsonObject Style::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(Water::Value::Size); i++) {
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
    json[Water::waterStrings[i][static_cast<int>(Water::Idx::JsonKey)]] = limit;
  }
  return json;
}

Style* Style::copy() const {
  auto* copy = new Style(this->getName());
  for (int i = 0; i < static_cast<int>(Water::Value::Size); i++) {
    copy->limited[i] = this->limited[i];
    for (int j = 0; j < static_cast<int>(Limit::Size); j++) {
      copy->limits[i][j] = this->limits[i][j];
    }
  }
  return copy;
}

float Style::get(Water::Value what, Limit limit) const {
  if (what < Water::Value::Size && limit < Limit::Size) {
    return limits[static_cast<uint>(what)][static_cast<uint>(limit)];
  }
  return -1;
}

void Style::set(Water::Value what, Limit limit, float value) {
  if (what < Water::Value::Size && limit < Limit::Size) {
    limits[static_cast<uint>(what)][static_cast<uint>(limit)] = value;
    updateEditTime();
    changed = true;
  }
}

bool Style::isLimited(Water::Value what) const {
  if (what < Water::Value::Size) {
    return limited[static_cast<uint>(what)];
  }
  return false;
}

void Style::limit(Water::Value what, bool limit) {
  if (what < Water::Value::Size) {
    limited[static_cast<uint>(what)] = limit;
    updateEditTime();
    changed = true;
  }
}

bool Style::isChanged() const {
  return changed;
}

void Style::setChanged(bool changed) {
  this->changed = changed;
}

int Style::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // limits without volume
  return static_cast<int>(Water::Value::Size) - 1;
}

int Style::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  // Enable disable + Limits
  return 1 + static_cast<int>(Limit::Size);
}

QVariant Style::data(const QModelIndex& index, int role) const {
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
  // enable disable
  if (col == 0) {
    return limited[row];
  }
  // values
  if (col > 0 && col < static_cast<int>(Limit::Size) + 1) {
    return limits[row][col - 1];
  }
  return {};
}

QVariant Style::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return {};
  }
  if (orientation == Qt::Horizontal) {
    // enable disable
    switch (section) {
      case 0:
        return tr("Limitieren");
      case 1:
        return tr("Minimum");
      case 2:
        return tr("Ziel");
      case 3:
        return tr("Maximum");
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

bool Style::setData(const QModelIndex& index, const QVariant& value, int role) {
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
  // enable disable
  if (col == 0) {
    limited[row] = value.toBool();
    updateEditTime();
    changed = true;
    return true;
  }
  // values
  if (col > 0 && col < static_cast<int>(Limit::Size) + 1) {
    limits[row][col - 1] = value.toFloat();
    updateEditTime();
    changed = true;
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
  return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
