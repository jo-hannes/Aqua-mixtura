// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MALTS_H
#define MALTS_H

#include "malt.h"

#include <QAbstractTableModel>
#include <QString>
#include <QVector>

class Malts : public QAbstractTableModel {
  Q_OBJECT

 public:
  Malts() {}

  // JSON conversion
  bool fromJson(const QJsonObject& json); /**< @brief convert JSON to Malts */
  QJsonObject toJson() const;             /**< @brief convert this Malts to JSON */
  bool importMalt(const QString& path);   /**< @brief import and add a Malt from JSON */
  bool exportMalt(const QString& path, qsizetype i); /**< @brief export a Malt at index as JSON */

  const Malt& getMalt(qsizetype i);         /**< @brief Get malt at index */
  void updateMalt(Malt& malt, qsizetype i); /**< @brief Update malt at index */
  void addMalt(Malt& malt);                 /**< @brief Add a malt */
  void deleteMalt(qsizetype i);             /**< @brief Delete malt at index */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

 private:
  QVector<Malt> malts;
  Malt noMalt;
};

#endif  // MALTS_H
