// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "style.h"

#include "QJsonValue"

Style::Style(QString name) : Meta(name) {}

Style::Style(const QJsonObject& json) {
  fromJson(json);
}

bool Style::fromJson(const QJsonObject& json) {
  const bool ret = Meta::fromJson(json);
  // start at index 1 to skip volume
  for (int i = 1; i < static_cast<int>(Water::Value::Size); i++) {
    // get sub object
    const QJsonValue limit = json[Water::strJsonKey.at(i)];
    limited.at(i) = limit.isObject();
    for (uint j = 0; j < strJsonKey.size(); j++) {
      limits.at(i).at(j) = limit[strJsonKey.at(j)].toDouble(0);
    }
  }
  changed = false;
  return ret;
}

QJsonObject Style::toJson() const {
  QJsonObject json;
  Meta::toJson(json);
  // start at index 1 to skip volume
  for (uint i = 1; i < limits.size(); i++) {
    // skip values not limited
    if (!limited.at(i)) {
      continue;
    }
    // create limit object
    QJsonObject limit;
    for (uint j = 0; j < strJsonKey.size(); j++) {
      limit[strJsonKey.at(j)] = limits.at(i).at(j);
    }
    // add object to main json
    json[Water::strJsonKey.at(i)] = limit;
  }
  return json;
}

Style* Style::copy() const {
  auto* copy = new Style(this->toJson());
  return copy;
}

double Style::get(Water::Value what, Limit limit) const {
  auto w = static_cast<uint>(what);
  auto l = static_cast<uint>(limit);
  if (w < limits.size() && l < limits[0].size()) {
    // if (what < Water::Value::Size && limit < Limit::Size) {
    return limits.at(w).at(l);
  }
  return -1;
}

void Style::set(Water::Value what, Limit limit, double value) {
  auto w = static_cast<uint>(what);
  auto l = static_cast<uint>(limit);
  if (w < limits.size() && l < limits[0].size()) {
    // if (what < Water::Value::Size && limit < Limit::Size) {
    limits.at(w).at(l) = value;
    updateEditTime();
    changed = true;
  }
}

bool Style::isLimited(Water::Value what) const {
  auto w = static_cast<uint>(what);
  if (w < limited.size()) {
    return limited.at(w);
  }
  return false;
}

void Style::limit(Water::Value what, bool limit) {
  auto w = static_cast<uint>(what);
  if (w < limited.size()) {
    limited.at(w) = limit;
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
    return limited.at(row);
  }
  // values
  if (col > 0 && col < static_cast<int>(Limit::Size) + 1) {
    return limits.at(row).at(col - 1);
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
      if (!Water::strUnit.at(idx).isEmpty()) {
        return Water::strTranslate.at(idx) + " (" + Water::strUnit.at(idx) + ")";
      }
      return Water::strTranslate.at(idx);
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
    limited.at(row) = value.toBool();
    updateEditTime();
    changed = true;
    return true;
  }
  // values
  if (col > 0 && col < static_cast<int>(Limit::Size) + 1) {
    limits.at(row).at(col - 1) = value.toDouble();
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
