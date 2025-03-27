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
  Malts() = default;
  explicit Malts(const QJsonObject& json); /**< @brief Create Malts from JSON */

  // JSON conversion
  bool fromJson(const QJsonObject& json);                          /**< @brief Update Malts from JSON */
  [[nodiscard]] QJsonObject toJson() const;                        /**< @brief convert this Malts to JSON */
  bool importMalt(const QString& path);                            /**< @brief import and add a Malt from JSON */
  [[nodiscard]] bool exportMalt(const QString& path, int i) const; /**< @brief export a Malt at index as JSON */

  const Malt& getMalt(int i);     /**< @brief Get malt at index */
  void addMalt(const Malt& malt); /**< @brief Add a malt */
  void deleteMalt(int i);         /**< @brief Delete malt at index */

  [[nodiscard]] bool isChanged() const; /**< @brief True if changes not saved */

  double mashPh(const double RA, const double waterVolume);

  // for QAbstractTableModel, see QT documentation for details
  // NOLINTBEGIN(modernize-use-nodiscard)
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  // NOLINTEND(modernize-use-nodiscard)

 public slots:
  void load();
  void save();

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark malts unsaved/saved */

  QVector<Malt> malts; /**< @brief Vector containing all malts */
  bool changed{false}; /**< @brief True if malts where changed but not saved */
  Malt noMalt;         /**< @brief Empty malt used to return no malt on error */
};

#endif  // MALTS_H
