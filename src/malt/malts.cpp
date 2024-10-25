// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "malts.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Malts::Malts() {
  changed = false;
}

Malts::Malts(const QJsonObject& json) {
  fromJson(json);
}

bool Malts::fromJson(const QJsonObject& json) {
  if (!json.contains("Malts")) {
    qWarning("No valid malts in JSON found");
    return false;
  }
  QJsonValue jsonMalts = json["Malts"];
  if (!jsonMalts.isArray()) {
    qWarning("No valid malts in JSON found");
    return false;
  }
  beginResetModel();
  malts.clear();
  for (const auto& malt : jsonMalts.toArray()) {
    malts.append(Malt(malt.toObject()));
  }
  endResetModel();
  setChanged(false);
  return true;
}

QJsonObject Malts::toJson() const {
  QJsonArray jsonMaltArray;
  for (const auto& malt : malts) {
    jsonMaltArray.append(malt.toJson());
  }
  QJsonObject jsonMalts;
  jsonMalts["Malts"] = jsonMaltArray;
  return jsonMalts;
}

bool Malts::importMalt(const QString& path) {
  if (path.isEmpty()) {
    return false;
  }
  QJsonObject jsonMalt = JsonHelper::loadFile(path);
  if (!jsonMalt.contains("Malt")) {
    return false;
  }
  Malt m(jsonMalt["Malt"].toObject());
  addMalt(m);
  setChanged(true);
  return true;
}

bool Malts::exportMalt(const QString& path, qsizetype i) const {
  if (path.isEmpty()) {
    return false;
  }
  if (i < 0 || i >= malts.size()) {
    return false;
  }
  QJsonObject jsonMalt;
  jsonMalt["Malt"] = malts.at(i).toJson();
  return JsonHelper::saveFile(path, jsonMalt);
}

const Malt& Malts::getMalt(qsizetype i) {
  if (i >= 0 && i < malts.size()) {
    return malts.at(i);
  } else {
    return noMalt;
  }
}

void Malts::addMalt(const Malt& malt) {
  qsizetype i = malts.size();
  beginInsertRows(QModelIndex(), i, i);
  malts.append(malt);
  endInsertRows();
  setChanged(true);
}

void Malts::deleteMalt(qsizetype i) {
  if (i >= 0 && i < malts.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    malts.removeAt(i);
    endRemoveRows();
    setChanged(true);
  }
}

bool Malts::isChanged() const {
  return changed;
}

int Malts::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return malts.size();
}

int Malts::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 4;
}

QVariant Malts::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  qsizetype row = index.row();
  if (row < 0 || row >= malts.size()) {
    return QVariant();
  }
  switch (index.column()) {
    case 0:
      return malts.at(row).getName();
    case 3:
      return malts.at(row).getMass();
    case 1:
      return malts.at(row).getEbc();
    case 2:
      return malts.at(row).getPh();
    default:
      return QVariant();
  }
  return QVariant();
}

QVariant Malts::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString(tr("Name"));
      case 3:
        return QString(tr("Menge") + " (kg)");
      case 1:
        return QString(tr("Farbe") + " (EBC)");
      case 2:
        return QString("pH");
      default:
        return QVariant();
    }
  } else
    return QString("Row %1").arg(section);
}

bool Malts::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (!index.isValid()) {
    return false;
  }
  if (index.row() < 0 || index.row() >= malts.size()) {
    return false;
  }
  if (role != Qt::EditRole) {
    return false;
  }
  switch (index.column()) {
    case 0:
      malts[index.row()].setName(value.toString());
      break;
    case 3:
      malts[index.row()].setMass(value.toFloat());
      break;
    case 1:
      malts[index.row()].setEbc(value.toFloat());
      break;
    case 2:
      malts[index.row()].setPh(value.toFloat());
      break;
    default:
      return false;
      break;
  }
  setChanged(true);
  return true;
}

Qt::ItemFlags Malts::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void Malts::load() {
  const QString fileName = "malts.json";
  Download::loadDefaults(fileName);
  QString file = Paths::dataDir() + "/" + fileName;
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Malts::save() {
  QString file = Paths::dataDir() + "/malts.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
}

void Malts::setChanged(bool changed) {
  this->changed = changed;
  emit dataModified();
}
