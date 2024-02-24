// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "malts.h"

#include "../common/jsonhelper.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Malts::Malts() {
  unsavedChanges = false;
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
    malts.append(Malt::fromJson(malt.toObject()));
  }
  setUnsaved(false);
  endResetModel();
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
  Malt m = Malt::fromJson(jsonMalt["Malt"].toObject());
  addMalt(m);
  setUnsaved(true);
  return true;
}

bool Malts::exportMalt(const QString& path, qsizetype i) {
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

void Malts::updateMalt(Malt& malt, qsizetype i) {
  if (i >= 0 && i < malts.size()) {
    malts.replace(i, malt);
    emit dataChanged(index(i, 0), index(i, 3));
    setUnsaved(true);
  }
}

void Malts::addMalt(Malt& malt) {
  qsizetype i = malts.size();
  beginInsertRows(QModelIndex(), i, i);
  malts.append(malt);
  endInsertRows();
  setUnsaved(true);
}

void Malts::deleteMalt(qsizetype i) {
  if (i >= 0 && i < malts.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    malts.removeAt(i);
    endRemoveRows();
    setUnsaved(true);
  }
}

void Malts::setMalts(const QVector<Malt>& newMalts) {
  beginResetModel();
  malts = newMalts;
  endResetModel();
  setUnsaved(false);
}

QVector<Malt>& Malts::getMalts() {
  return malts;
}

void Malts::setSaved() {
  setUnsaved(false);
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
    case 1:
      return malts.at(row).getMass();
    case 2:
      return malts.at(row).getEbc();
    case 3:
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
      case 1:
        return QString(tr("Menge") + " (kg)");
      case 2:
        return QString(tr("Farbe") + " (EBC)");
      case 3:
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
    case 1:
      malts[index.row()].setMass(value.toFloat());
      break;
    case 2:
      malts[index.row()].setEbc(value.toFloat());
      break;
    case 3:
      malts[index.row()].setPh(value.toFloat());
      break;
    default:
      return false;
      break;
  }
  setUnsaved(true);
  return true;
}

Qt::ItemFlags Malts::flags(const QModelIndex& index) const {
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

void Malts::setUnsaved(bool unsaved) {
  if (this->unsavedChanges != unsaved) {
    this->unsavedChanges = unsaved;
    emit unsavedMalts(unsaved);
  }
}
