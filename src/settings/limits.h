// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef LIMITS_H
#define LIMITS_H

#include "../common/global.h"
#include "../common/meta.h"

#include <QAbstractTableModel>
#include <QJsonObject>
#include <QObject>

class Limits : public QAbstractTableModel, public Meta {
  Q_OBJECT
 public:
  explicit Limits(QObject* parent = nullptr);

  Limits(const QJsonObject& json);        /**< @brief Construct Limits from JSON */
  bool fromJson(const QJsonObject& json); /**< @brief Update Limits from JSON */
  QJsonObject toJson() const;             /**< @brief convert Limits to JSON */

  // getter und setter
  float getMin(AM::WaterValue what);             /**< @brief get lower limit */
  float getMax(AM::WaterValue what);             /**< @brief get upper limit */
  void setMin(AM::WaterValue what, float value); /**< @brief set lower limi */
  void setMax(AM::WaterValue what, float value); /**< @brief set upper limit */

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

 private:
  float limits[static_cast<int>(AM::WaterValue::Size)][2];
};

#endif  // LIMITS_H
