// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "malts.h"

#include "../common/download.h"
#include "../common/jsonhelper.h"
#include "../common/paths.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

Malts::Malts(const QJsonObject& json) {
  fromJson(json);
}

bool Malts::fromJson(const QJsonObject& json) {
  if (!json.contains("Malts")) {
    qWarning("No valid malts in JSON found");
    return false;
  }
  const QJsonValue jsonMalts = json["Malts"];
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
  const Malt m(jsonMalt["Malt"].toObject());
  addMalt(m);
  setChanged(true);
  return true;
}

bool Malts::exportMalt(const QString& path, int i) const {
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

const Malt& Malts::getMalt(int i) {
  if (i >= 0 && i < malts.size()) {
    return malts.at(i);
  }
  return noMalt;
}

void Malts::addMalt(const Malt& malt) {
  const int i = malts.size();  // NOLINT(*-narrowing-conversions): beginInsertRows requires int
  beginInsertRows(QModelIndex(), i, i);
  malts.append(malt);
  endInsertRows();
  setChanged(true);
}

void Malts::deleteMalt(int i) {
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

/**
 * @brief Calculates the mash Ph
 *
 * All calculations are based on https://maischemalzundmehr.de/index.php?inhaltmitte=exp_maischph
 *
 * Verwendete Formelzeichen:
 * \f[ pH_{Maische} = \textnormal{Maische pH} \f]
 * \f[ pH_{DW}      = \textnormal{pH in Destiliertem Wasser} \f]
 * \f[ S_{pH}       = \textnormal{Beziehung pH zu Restalkalität in pH/(mmol/l)} \f]
 * \f[ RA           = \textnormal{Restalkalität in mmol/l} \f]
 *
 * Gesammtformel:
 * \f[ pH_{mash} = pH_{DW} + S_{pH} \cdot RA \f]
 *
 * pH in Destiliertem Wasser:
 * \f[ pH_{DWx} = pH_{DW} \textnormal{ eines Malzes} \f]
 * \f[ A_x      = \textnormal{Anteil eines Malzes} \f]
 * \f[
 * pH_{DW} = -log_{10} \cdot \sum 10^{-pH_{DWx}} \cdot A_x
 * \f]
 *
 * Beziehung pH zu Restalkalität:
 * \f[ D = \textnormal{Maischedicke in Liter/kg} \f]
 * \f[
 * S_{pH} = 0.013 \cdot D + 0.013
 * \f]
 */
double Malts::mashPh(const double RA, const double waterVolume) {
  // get total malt weight
  double maltMass = 0;
  for (const auto& m : malts) {
    maltMass += m.getMass();
  }

  // check if we have some malt
  if (maltMass <= 0.001) {  // NOLINT(*-magic-numbers)
    // No malt
    return 7;
  }

  // calculate SpH
  const double SpH = 0.013 * (waterVolume / maltMass) + 0.013;

  // calculate pH in purified water
  double pHdwSum = 0;
  for (const auto& m : malts) {
    pHdwSum += pow(10, m.getPh() * -1) * m.getMass() / maltMass;
  }
  const double pHdw = log10(pHdwSum) * -1;

  // convert RA into mmol/l
  const double RAmmoll = RA * 0.3566;

  // calcualte mash pH
  const double pHmash = (pHdw + SpH * RAmmoll);
  // TODO remove debugging!
  qDebug() << "Calculating mash ph. SpH:" << SpH << "pHdw:" << pHdw << "RAmmoll:" << RAmmoll << "pHmash:" << pHmash;
  return pHmash;
}

int Malts::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return malts.size();  // NOLINT(*-narrowing-conversions): using int because of QAbstractTableModel
}

int Malts::columnCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return 4;
}

QVariant Malts::data(const QModelIndex& index, int role) const {
  if (!index.isValid()) {
    return {};
  }
  if (role != Qt::DisplayRole) {
    return {};
  }
  const qsizetype row = index.row();
  if (row < 0 || row >= malts.size()) {
    return {};
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
      return {};
  }
  return {};
}

QVariant Malts::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return {};
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
        return {};
    }
  } else {
    return QString("Row %1").arg(section);
  }
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
      malts[index.row()].setMass(value.toDouble());
      break;
    case 1:
      malts[index.row()].setEbc(value.toDouble());
      break;
    case 2:
      malts[index.row()].setPh(value.toDouble());
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
  return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

void Malts::load() {
  const QString fileName = "malts.json";
  Download::loadDefaults(fileName);
  const QString file = Paths::dataDir() + "/" + fileName;
  if (QFile::exists(file)) {
    this->fromJson(JsonHelper::loadFile(file));
  }
}

void Malts::save() {
  const QString file = Paths::dataDir() + "/malts.json";
  JsonHelper::saveFile(file, this->toJson());
  setChanged(false);
}

void Malts::setChanged(bool changed) {
  this->changed = changed;
  emit dataModified();
}
