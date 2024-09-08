// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef SETTINGS_H
#define SETTINGS_H

#include "limits.h"

#include "../common/global.h"
#include "../common/meta.h"

#include <QAbstractTableModel>
#include <QJsonObject>
#include <QObject>

/**
 * @brief Programm global settings
 */
class Settings : public QAbstractTableModel, public Meta {
  Q_OBJECT
 public:
  explicit Settings(QObject* parent = nullptr);

  Settings(const QJsonObject& json);      /**< @brief Construct Settings from JSON */
  bool fromJson(const QJsonObject& json); /**< @brief Update Settings from JSON */
  QJsonObject toJson() const;             /**< @brief convert Settings to JSON */

  // getter und setter
  float getMin(AM::WaterValue what) const;       /**< @brief get lower limit */
  float getMax(AM::WaterValue what) const;       /**< @brief get upper limit */
  void setMin(AM::WaterValue what, float value); /**< @brief set lower limi */
  void setMax(AM::WaterValue what, float value); /**< @brief set upper limit */

  bool isChanged() const; /**< @brief True if changes not saved */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex& index) const;

 public slots:
  void load(); /**< @brief Load all data from JSON files */
  void save(); /**< @brief Save all data to JSON files */

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark unsaved/saved */
  bool changed;                  /**< @brief True if changed but not saved */

  float limits[static_cast<int>(AM::WaterValue::Size)][2];
};

#endif  // SETTINGS_H
