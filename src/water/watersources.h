// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERSOURCES_H
#define WATERSOURCES_H

#include "water.h"

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <QString>
#include <QVector>

class WaterSources : public QAbstractTableModel {
  Q_OBJECT

 public:
  WaterSources();

  // JSON conversion
  bool fromJson(const QJsonObject& json); /**< @brief convert JSON to WaterSources */
  QJsonObject toJson() const;             /**< @brief convert this WaterSources to JSON */

  // bool import(const QString& path);

  const Water& getProfile(qsizetype i);                   /**< @brief Get water profile at index */
  void updateProfile(Water& profile, qsizetype i);        /**< @brief Update water profile at index */
  void addProfile(Water& profile);                        /**< @brief Add a water profile */
  void deleteProfile(qsizetype i);                        /**< @brief Delete water profile at index */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

 private:
  QVector<Water> sources;
  Water noWater;
};

#endif // WATERSOURCES_H