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
  WaterSources() = default;
  explicit WaterSources(const QJsonObject& json); /**< @brief Create WaterSources from JSON */

  // JSON conversion
  bool fromJson(const QJsonObject& json);          /**< @brief Update WaterSources from JSON */
  [[nodiscard]] QJsonObject toJson() const;        /**< @brief convert this WaterSources to JSON */
  [[nodiscard]] QJsonObject profileToJson() const; /**< @brief convert profile of this WaterSources to JSON */

  // bool import(const QString& path);

  const Water& getProfile(int i);            /**< @brief Get water profile at index */
  Water getMix();                            /**< @brief Get mixture of all waters */
  void updateProfile(Water& profile, int i); /**< @brief Update water profile at index */
  void addProfile(const Water& profile);     /**< @brief Add a water profile */
  void deleteProfile(int i);                 /**< @brief Delete water profile at index */

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
  void save() const;

 signals:
  void dataModified();

 private:
  QVector<Water> sources;
  Water noWater;
};

#endif  // WATERSOURCES_H
