// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTURES_H
#define MIXTURES_H

#include "mixture.h"

#include <QAbstractTableModel>
#include <QObject>
#include <QVector>

class Mixtures : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit Mixtures(QObject* parent = nullptr);

  // JSON conversion
  bool fromJson(const QJsonObject& json);               /**< @brief convert JSON to Mixtures */
  QJsonObject toJson() const;                           /**< @brief convert this Mixtures to JSON */
  bool importMixture(const QString& path);              /**< @brief import and add a Mixture from JSON */
  bool exportMixture(const QString& path, qsizetype i); /**< @brief export a Mixture at index as JSON */

  const Mixture& getMixture(qsizetype i);            /**< @brief Get mixture at index */
  void updateMixture(Mixture& mixture, qsizetype i); /**< @brief Update mixture at index */
  void addMixture(Mixture& mixture);                 /**< @brief Add a mixture */
  void deleteMixture(qsizetype i);                   /**< @brief Delete mixture at index */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  // bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  // Qt::ItemFlags flags(const QModelIndex& index) const;

 private:
  QVector<Mixture> mixtures;
  Mixture noMixture;
};

#endif  // MIXTURES_H
