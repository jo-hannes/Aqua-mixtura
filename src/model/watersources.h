// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERSOURCES_H
#define WATERSOURCES_H

#include "waterprofile.h"

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <QString>
#include <QVector>

class WaterSources : public QAbstractTableModel {
  Q_OBJECT

 public:
  WaterSources();

  // load/save all
  bool load(const QString& path); /**< @brief Load water sources/profiles from file*/
  bool save(const QString& path); /**< @brief Save water sources/profiles from file */

  // bool import(const QString& path);

  const WaterProfile& getProfile(qsizetype index);            /**< @brief Get water profile at index */
  void updateProfile(WaterProfile& profile, qsizetype index); /**< @brief Update water profile at index */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

 private:
  QVector<WaterProfile> sources;
};

#endif // WATERSOURCES_H
