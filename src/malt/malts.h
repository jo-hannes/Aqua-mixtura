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
  Malts();
  Malts(const QJsonObject& json); /**< @brief Create Malts from JSON */

  // JSON conversion
  bool fromJson(const QJsonObject& json); /**< @brief Update Malts from JSON */
  QJsonObject toJson() const;             /**< @brief convert this Malts to JSON */
  bool importMalt(const QString& path);   /**< @brief import and add a Malt from JSON */
  bool exportMalt(const QString& path, qsizetype i) const; /**< @brief export a Malt at index as JSON */

  const Malt& getMalt(qsizetype i);         /**< @brief Get malt at index */
  void addMalt(const Malt& malt);           /**< @brief Add a malt */
  void deleteMalt(qsizetype i);             /**< @brief Delete malt at index */

  bool isChanged() const;

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex& index) const;

 public slots:
  void load();
  void save();

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark malts unsaved/saved */

  QVector<Malt> malts; /**< @brief Vector containing all malts */
  bool changed;        /**< @brief True if malts where changed but not saved */
  Malt noMalt;         /**< @brief Empty malt used to return no malt on error */
};

#endif  // MALTS_H
