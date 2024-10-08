// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersources.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

WaterSources::WaterSources() {}

WaterSources::WaterSources(const QJsonObject& json) {
  fromJson(json);
}

bool WaterSources::fromJson(const QJsonObject& json) {
  QJsonValue jsonSources = json["WaterSources"];
  if (!jsonSources.isArray()) {
    qWarning("No valid sources in JSON found");
    return false;
  }
  beginResetModel();
  sources.clear();
  for (const auto& profile : jsonSources.toArray()) {
    sources.append(Water(profile.toObject()));
  }
  emit dataModified();
  endResetModel();
  return true;
}

QJsonObject WaterSources::toJson() const {
  QJsonArray jsonSrcArray;
  for (const auto& src : sources) {
    jsonSrcArray.append(src.toJson());
  }
  QJsonObject jsonSources;
  jsonSources["WaterSources"] = jsonSrcArray;
  return jsonSources;
}

QJsonObject WaterSources::profileToJson() const {
  QJsonArray jsonSrcArray;
  for (const auto& src : sources) {
    jsonSrcArray.append(src.profileToJson());
  }
  QJsonObject jsonSources;
  jsonSources["WaterSources"] = jsonSrcArray;
  return jsonSources;
}

const Water& WaterSources::getProfile(qsizetype i) {
  if (i >= 0 && i < sources.size()) {
    return sources.at(i);
  } else {
    return noWater;
  }
}

Water WaterSources::getMix() {
  Water result("Mix");
  for (Water& w : sources) {
    result += w;
  }
  return result;
}

void WaterSources::updateProfile(Water& profile, qsizetype i) {
  if (i >= 0 && i < sources.size()) {
    sources.replace(i, profile);
    emit dataChanged(index(i, 0), index(i, 1));
  }
}

void WaterSources::addProfile(const Water& profile) {
  qsizetype i = sources.size();
  beginInsertRows(QModelIndex(), i, i);
  sources.append(profile);
  emit dataModified();
  endInsertRows();
}

void WaterSources::deleteProfile(qsizetype i) {
  if (i >= 0 && i < sources.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    sources.removeAt(i);
    emit dataModified();
    endRemoveRows();
  }
}

int WaterSources::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return sources.size();
}

int WaterSources::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant WaterSources::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  qsizetype row = index.row();
  if (row < 0 || row >= sources.size()) {
    return QVariant();
  }

  switch (index.column()) {
    case 0:
      return sources.at(row).getName();
    case 1:
      return sources.at(row).get(Water::Value::Restalkalitaet);
    case 2:
      return sources.at(row).get(Water::Value::Volume);
    default:
      return QVariant();
  }
  return QVariant();
}

QVariant WaterSources::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString(tr("Name"));
      case 1:
        return QString(tr("Restalkalität"));
      case 2:
        return QString(tr("Menge") + " (L)");
      default:
        return QVariant();
    }
    return QString(tr("Name"));
  } else
    return QString("Row %1").arg(section);
}

bool WaterSources::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (role != Qt::EditRole) {
    return false;
  }
  if (!index.isValid()) {
    return false;
  }
  qsizetype row = index.row();
  if (row < 0 || row >= sources.size()) {
    return false;
  }
  if (index.column() == 2) {
    sources[row].set(Water::Value::Volume, value.toFloat());
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

Qt::ItemFlags WaterSources::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  // only volume is editable
  if (index.column() == 2) {
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
  }
  return QAbstractItemModel::flags(index);
}

void WaterSources::load() {
  const QString fileName = "sources.json";
  Download::loadDefaults(fileName);
  QString file = Paths::dataDir() + "/" + fileName;
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void WaterSources::save() {
  QString file = Paths::dataDir() + "/sources.json";
  JsonHelper::saveFile(file, this->profileToJson());
}
