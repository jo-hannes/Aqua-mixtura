// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersources.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

WaterSources::WaterSources(const QJsonObject& json) {
  fromJson(json);
}

bool WaterSources::fromJson(const QJsonObject& json) {
  const QJsonValue jsonSources = json["WaterSources"];
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
  // Calculate total amount from sources
  total = 0;
  for (const auto& w : sources) {
    total += w.get(Water::Value::Volume);
  }
  // get strike and sparging water
  strike = json["StrikeWater"].toDouble(total);
  sparging = total - strike;
  emit maxStrikeSpargingChanged(HUGE_VAL);  // ensure new value will fit
  emit strikeVolumeChanged(strike);
  emit spargingVolumeChanged(sparging);
  emit totalVolumeChanged(total);
  emit maxStrikeSpargingChanged(total);
  return true;
}

QJsonObject WaterSources::toJson() const {
  QJsonArray jsonSrcArray;
  for (const auto& src : sources) {
    jsonSrcArray.append(src.toJson());
  }
  QJsonObject jsonSources;
  jsonSources["WaterSources"] = jsonSrcArray;
  jsonSources["StrikeWater"] = strike;
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

double WaterSources::getTotalVolume() const {
  return total;
}

void WaterSources::setTotalVolume(double volume) {
  // When just changing the total amount we should not change the percentage amounts of the different waters
  if (sources.empty()) {
    return;
  }
  if (volume == 0) {
    return;
  }
  const double factor = volume / total;
  qDebug() << "WaterSources::setTotalVolume; vol:" << volume << "fact:" << factor;
  total = volume;
  for (int i = 0; i < sources.size(); i++) {
    sources[i].set(Water::Value::Volume, sources.at(i).get(Water::Value::Volume) * factor);
    emit dataChanged(index(i, 2), index(i, 2));
  }
  strike *= factor;
  sparging *= factor;

  // ensure maximum for strike and sparging is big enougth
  if (factor > 1) {
    // total increased, first increase max so it will be bigger than strike and sparging
    emit maxStrikeSpargingChanged(total);
    emit strikeVolumeChanged(strike);
    emit spargingVolumeChanged(sparging);
    emit totalVolumeChanged(total);
  } else {
    // total decreased, first decrease strike and sparging so it will be lower than max
    emit strikeVolumeChanged(strike);
    emit spargingVolumeChanged(sparging);
    emit totalVolumeChanged(total);
    emit maxStrikeSpargingChanged(total);
  }
}

double WaterSources::getStrikeWater() const {
  return strike;
}

void WaterSources::setStrikeWater(double volume) {
  // limit ragne
  if (volume < 0) {
    volume = 0;
  }
  if (volume > total) {
    volume = total;
  }
  strike = volume;
  sparging = total - strike;
  emit spargingVolumeChanged(sparging);
  emit dataChanged(index(3, 0),
                   index(4, sources.size()));  // NOLINT(*-narrowing-conversions): dataChanged index uses int
}

double WaterSources::getSpargingWater() const {
  return sparging;
}

void WaterSources::setSpargingWater(double volume) {
  // limit ragne
  if (volume < 0) {
    volume = 0;
  }
  if (volume > total) {
    volume = total;
  }
  sparging = volume;
  strike = total - sparging;
  emit strikeVolumeChanged(strike);
  emit dataChanged(index(3, 0),
                   index(4, sources.size()));  // NOLINT(*-narrowing-conversions): dataChanged index uses int
}

const Water& WaterSources::getProfile(int i) {
  if (i >= 0 && i < sources.size()) {
    return sources.at(i);
  }
  return noWater;
}

Water WaterSources::getMix() {
  Water result("Mix");
  for (const Water& w : sources) {
    result += w;
  }
  return result;
}

void WaterSources::updateProfile(Water& profile, int i) {
  if (i >= 0 && i < sources.size()) {
    sources.replace(i, profile);
    updateAllVolumes(i);
    emit dataChanged(index(i, 0), index(i, 1));
  }
}

void WaterSources::addProfile(const Water& profile) {
  const int i = sources.size();  // NOLINT(*-narrowing-conversions): beginInsertRows requires int
  beginInsertRows(QModelIndex(), i, i);
  sources.append(profile);
  updateAllVolumes(i);
  emit dataModified();
  endInsertRows();
}

void WaterSources::deleteProfile(int i) {
  if (i >= 0 && i < sources.size()) {
    beginRemoveRows(QModelIndex(), i, i);
    sources.removeAt(i);
    updateAllVolumes(-1);
    emit dataModified();
    endRemoveRows();
  }
}

int WaterSources::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return sources.size();  // NOLINT(*-narrowing-conversions): using int because of QAbstractTableModel
}

int WaterSources::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 5;
}

QVariant WaterSources::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return {};
  }
  if (role != Qt::DisplayRole) {
    return {};
  }
  const qsizetype row = index.row();
  if (row < 0 || row >= sources.size()) {
    return {};
  }

  switch (index.column()) {
    case 0:
      return sources.at(row).getName();
    case 1:
      return sources.at(row).get(Water::Value::Restalkalitaet);
    case 2:
      return sources.at(row).get(Water::Value::Volume);
    case 3:
      return sources.at(row).get(Water::Value::Volume) * strike / total;
    case 4:
      return sources.at(row).get(Water::Value::Volume) * sparging / total;
    default:
      return {};
  }
  return {};
}

QVariant WaterSources::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return {};
  }
  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString(tr("Name"));
      case 1:
        return QString(tr("Restalkalität"));
      case 2:
        return QString(tr("Menge") + " (L)");
      case 3:
        return QString(tr("HG") + " (L)");
      case 4:
        return QString(tr("NG") + " (L)");
      default:
        return {};
    }
    return QString(tr("Name"));
  }
  return QString("Row %1").arg(section);
}

bool WaterSources::setData(const QModelIndex& index, const QVariant& value, int role) {
  if (role != Qt::EditRole) {
    return false;
  }
  if (!index.isValid()) {
    return false;
  }
  const int row = index.row();
  if (row < 0 || row >= sources.size()) {
    return false;
  }
  if (index.column() == 2) {
    sources[row].set(Water::Value::Volume, value.toDouble());
    emit dataChanged(index, index);
    updateAllVolumes(row);
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
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
  }
  return QAbstractTableModel::flags(index);
}

void WaterSources::load() {
  const QString fileName = "sources.json";
  Download::loadDefaults(fileName);
  const QString file = Paths::dataDir() + "/" + fileName;
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void WaterSources::save() const {
  const QString file = Paths::dataDir() + "/sources.json";
  JsonHelper::saveFile(file, this->profileToJson());
}

void WaterSources::updateAllVolumes(int preserve) {
  if (sources.empty()) {
    return;
  }
  double residual = total;
  // first check the index we preserve
  if (preserve >= 0 && preserve < sources.size()) {
    residual = updateVolume(preserve, residual);
  }
  for (int i = 0; i < sources.size(); i++) {
    if (i == preserve) {
      continue;
    }
    residual = updateVolume(i, residual);
  }
  if (residual > 0) {
    // something left we need to add to last water
    const int idx = sources.size() - 1;  // NOLINT(*-narrowing-conversions): dataChanged index uses int
    sources[idx].set(Water::Value::Volume, sources.at(idx).get(Water::Value::Volume) + residual);
    emit dataChanged(index(idx, 2), index(idx, 2));
  }
}

double WaterSources::updateVolume(int idx, double residual) {
  const double val = sources.at(idx).get(Water::Value::Volume);
  if (val > residual) {
    sources[idx].set(Water::Value::Volume, residual);
    emit dataChanged(index(idx, 2), index(idx, 2));
    return 0;
  }
  return residual - val;
}
