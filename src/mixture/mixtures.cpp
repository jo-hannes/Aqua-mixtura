// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixtures.h"

#include "../common/jsonhelper.h"

#include <QJsonArray>

Mixtures::Mixtures(QObject* parent) : QAbstractTableModel{parent} {}

bool Mixtures::fromJson(const QJsonObject& json) {
  QJsonValue jsonMixtures = json["Mixtures"];
  if (!jsonMixtures.isArray()) {
    qWarning("No valid mixtures in JSON found");
    return false;
  }
  mixtures.clear();
  for (const auto& mix : jsonMixtures.toArray()) {
    mixtures.append(Mixture::fromJson(mix.toObject()));
  }
  return true;
}

QJsonObject Mixtures::toJson() const {
  QJsonArray jsonMixtureArray;
  for (const auto& mix : mixtures) {
    jsonMixtureArray.append(mix.toJson());
  }
  QJsonObject jsonMixtures;
  jsonMixtures["Mixtures"] = jsonMixtureArray;
  return jsonMixtures;
}

bool Mixtures::importMixture(const QString& path) {
  if (path.isEmpty()) {
    return false;
  }
  QJsonObject json = JsonHelper::loadFile(path);
  if (!json.contains("AquaMixture")) {
    return false;
  }
  Mixture imported = Mixture::fromJson(json["AquaMixture"].toObject());
  addMixture(imported);
  return true;
}

bool Mixtures::exportMixture(const QString& path, qsizetype i) {
  if (path.isEmpty()) {
    return false;
  }
  if (i < 0 || i >= mixtures.size()) {
    return false;
  }
  QJsonObject json;
  json["AquaMixture"] = mixtures.at(i).toJson();
  return JsonHelper::saveFile(path, json);
}

const Mixture& Mixtures::getMixture(qsizetype i) {
  if (i >= 0 && i < mixtures.size()) {
    return mixtures.at(i);
  } else {
    return noMixture;
  }
}

void Mixtures::updateMixture(Mixture& mixture, qsizetype i) {
  if (i >= 0 && i < mixtures.size()) {
    mixtures.replace(i, mixture);
  }
}

void Mixtures::addMixture(Mixture& mixture) {
  qsizetype i = mixtures.size();
  beginInsertRows(QModelIndex(), i, i);
  mixtures.append(mixture);
  endInsertRows();
}

void Mixtures::deleteMixture(qsizetype i) {
  if (i >= 0 && i < mixtures.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    mixtures.removeAt(i);
    endRemoveRows();
  }
}

int Mixtures::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return mixtures.size();
}

int Mixtures::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant Mixtures::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  qsizetype row = index.row();
  if (row < 0 || row >= mixtures.size()) {
    return QVariant();
  }

  switch (index.column()) {
    case 0:
      return mixtures.at(row).getName();
    case 1:
      return mixtures.at(row).getCreationTime();
    case 2:
      return mixtures.at(row).getModificationTime();
    default:
      return QVariant();
  }
}

QVariant Mixtures::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString(tr("Name"));
      case 1:
        return QString(tr("Erstellt"));
      case 2:
        return QString(tr("Geändert"));
      default:
        return QVariant();
    }
  } else {
    return QString("Row %1").arg(section);
  }
}

// bool Mixtures::setData(const QModelIndex& index, const QVariant& value, int role) {}

// Qt::ItemFlags Mixtures::flags(const QModelIndex& index) const {}