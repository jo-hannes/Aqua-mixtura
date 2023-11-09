// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "malts.h"

#include "jsonhelper.h"

#include "QtCore/qjsonarray.h"

#include <QFile>
#include <QJsonDocument>

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
  malts.clear();
  for (const auto& malt : jsonMalts.toArray()) {
    malts.append(Malt::fromJson(malt.toObject()));
  }
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
  malts.append(m);
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
  }
}

void Malts::addMalt(Malt& malt) {
  qsizetype i = malts.size();
  beginInsertRows(QModelIndex(), i, i);
  malts.append(malt);
  endInsertRows();
}

void Malts::deleteMalt(qsizetype i) {
  if (i >= 0 && i < malts.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    malts.removeAt(i);
    endRemoveRows();
  }
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
