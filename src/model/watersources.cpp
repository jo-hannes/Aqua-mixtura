// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersources.h"

#include "QtCore/qjsonarray.h"

#include <QFile>
#include <QJsonDocument>

WaterSources::WaterSources() {}

bool WaterSources::load(const QString& path) {
  QFile loadFile(path);
  if (!loadFile.open(QIODevice::ReadOnly)) {
    qWarning("Unable to open water sources");
    return false;
  }
  QJsonDocument jsonDoc(QJsonDocument::fromJson(loadFile.readAll()));

  QJsonValue jsonSources = jsonDoc.object()["WaterSources"];
  if (!jsonSources.isArray()) {
    qWarning("No valid sources in JSON found");
  }
  for (const auto& profile : jsonSources.toArray()) {
    sources.append(WaterProfile::fromJson(profile.toObject()));
    qDebug("Loading water source");
  }

  return true;
}

bool WaterSources::save(const QString& path) {
  QFile saveFile(path);
  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Unable to open water sources");
    return false;
  }

  QJsonArray jsonSrcArray;
  for (const auto& src : sources) {
    jsonSrcArray.append(src.toJson());
  }
  QJsonObject jsonSources;
  jsonSources["WaterSources"] = jsonSrcArray;

  saveFile.write(QJsonDocument(jsonSources).toJson());
  saveFile.close();
  return true;
}

const WaterProfile& WaterSources::getProfile(qsizetype i) {
  if (i >= 0 && i < sources.size()) {
    return sources.at(i);
  } else {
    return noProfile;
  }
}

void WaterSources::updateProfile(WaterProfile& profile, qsizetype i) {
  if (i >= 0 && i < sources.size()) {
    sources.replace(i, profile);
    emit dataChanged(index(i, 0), index(i, 1));
  }
}

void WaterSources::addProfile(WaterProfile &profile)
{
  qsizetype i = sources.size();
  beginInsertRows(QModelIndex(), i, i);
  sources.append(profile);
  endInsertRows();
}

void WaterSources::deleteProfile(qsizetype i) {
  if (i >= 0 && i < sources.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    sources.removeAt(i);
    endRemoveRows();
  }
}

int WaterSources::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return sources.size();
}

int WaterSources::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 2;
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
      return sources.at(row).getRestalkalitaet();
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
      default:
        return QVariant();
    }
    return QString(tr("Name"));
  } else
    return QString("Row %1").arg(section);
}
