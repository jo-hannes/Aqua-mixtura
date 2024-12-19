// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixtures.h"

#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QJsonArray>

Mixtures::Mixtures(QObject* parent) : QAbstractTableModel{parent} {}

Mixtures::Mixtures(const QJsonObject& json, QObject* parent) : Mixtures(parent) {
  fromJson(json);
}

Mixtures::~Mixtures() {
  for (MixtureWindow* w : mixWindows) {
    delete w;
  }
}

bool Mixtures::fromJson(const QJsonObject& json) {
  const QJsonValue jsonMixtures = json["Mixtures"];
  if (!jsonMixtures.isArray()) {
    qWarning("No valid mixtures in JSON found");
    return false;
  }
  beginResetModel();
  mixtures.clear();
  for (const auto& mix : jsonMixtures.toArray()) {
    QJsonObject mixObj = mix.toObject();
    mixtures.append(Mixture(mixObj["file"].toString("")));
  }
  endResetModel();
  return true;
}

QJsonObject Mixtures::toJson() const {
  QJsonArray jsonMixtureArray;
  for (const auto& mix : mixtures) {
    QJsonObject jmix;
    jmix["file"] = mix.getPath();
    jsonMixtureArray.append(jmix);
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
  Mixture imported(json["AquaMixture"].toObject());
  imported.save();
  addMixture(imported);
  return true;
}

bool Mixtures::exportMixture(const QString& path, int i) {
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

Mixture& Mixtures::getMixture(int i) {
  if (i >= 0 && i < mixtures.size()) {
    return mixtures[i];
  }
  return noMixture;
}

void Mixtures::updateMixture(Mixture& mixture, int i) {
  if (i >= 0 && i < mixtures.size()) {
    mixtures.replace(i, mixture);
  }
}

void Mixtures::addMixture(Mixture& mixture) {
  const int i = mixtures.size();  // NOLINT(*-narrowing-conversions): beginInsertRows requires int
  beginInsertRows(QModelIndex(), i, i);
  mixtures.append(mixture);
  endInsertRows();
}

void Mixtures::deleteMixture(int i) {
  if (i >= 0 && i < mixtures.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    mixtures.removeAt(i);
    endRemoveRows();
    if (i < mixWindows.size()) {
      if (mixWindows[i] != nullptr) {
        delete mixWindows[i];
      }
      mixWindows.removeAt(i);
    }
  }
}

void Mixtures::show(int i) {
  // check index
  if (i < 0 || i >= mixtures.size()) {
    return;
  }
  // resize if needed
  if (mixWindows.size() <= i) {
    mixWindows.resize(i + 1, nullptr);
  }
  // create Window if needed
  if (mixWindows[i] == nullptr) {
    mixWindows[i] = new MixtureWindow(mixtures[i], waterDb, additiveCfg, maltDb, styleDb, settings);
  }
  mixWindows[i]->show();
}

bool Mixtures::closeAll() {
  for (MixtureWindow* w : mixWindows) {
    if (w != nullptr) {
      if (!w->close()) {
        return false;
      }
    }
  }
  return true;
}

int Mixtures::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return mixtures.size();  // NOLINT(*-narrowing-conversions): using int because of QAbstractTableModel
}

int Mixtures::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant Mixtures::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return {};
  }
  if (role != Qt::DisplayRole) {
    return {};
  }
  const qsizetype row = index.row();
  if (row < 0 || row >= mixtures.size()) {
    return {};
  }

  switch (index.column()) {
    case 0:
      return mixtures.at(row).getName();
    case 1:
      return mixtures.at(row).getCreationTime();
    case 2:
      return mixtures.at(row).getModificationTime();
    default:
      return {};
  }
}

QVariant Mixtures::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return {};
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
        return {};
    }
  } else {
    return QString("Row %1").arg(section);
  }
}

void Mixtures::load() {
  const QString file = Paths::dataDir() + "/mixtures.json";
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Mixtures::save() const {
  const QString file = Paths::dataDir() + "/mixtures.json";
  JsonHelper::saveFile(file, this->toJson());
}
