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

  [[nodiscard]] double getTotalVolume() const;   /**< @brief Get total water amount */
  void setTotalVolume(double volume);            /**< @brief Set total water amount */
  [[nodiscard]] double getStrikeWater() const;   /**< @brief Get strike water amount */
  void setStrikeWater(double volume);            /**< @brief Set strike water amount */
  [[nodiscard]] double getSpargingWater() const; /**< @brief Get sparging water amount */
  void setSpargingWater(double volume);          /**< @brief Set sparging water amount */

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
  void totalVolumeChanged(double total);
  void maxStrikeSpargingChanged(double max);
  void strikeVolumeChanged(double voulme);
  void spargingVolumeChanged(double voulme);

 private:
  /**
   * @brief updateAllVolumes Update volumes of water sources to match total water amount
   * @param preserve Index of water source where volume should be preserved if possible
   */
  void updateAllVolumes(int preserve);
  /**
   * @brief updateVolume Update volume at index and limit to residual
   * @param idx Index of water
   * @param residual Residual amount of water
   * @return Residual amount of water after updating idx
   */
  double updateVolume(int idx, double residual);

  QVector<Water> sources; /**< @brief Vector with all waters contained */
  double total{0};        /**< @brief Total water amount */
  double strike{0};       /**< @brief Strike water amount (Hauptguss) */
  double sparging{0};     /**< @brief Sparging water amount (Nachguss) */
  Water noWater;          /**< @brief Dummy for returning noWater on out of index access */
};

#endif  // WATERSOURCES_H
