// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLE_H
#define STYLE_H

#include "../common/global.h"
#include "../common/meta.h"

#include <QAbstractTableModel>
#include <QJsonObject>
#include <QString>

/**
 * @brief Class for a single beer style
 */
class Style : public QAbstractTableModel, public Meta {
  Q_OBJECT

 public:
  /**
   * @brief Type of limits
   */
  enum class Limit { Min, Target, Max, Size };

  Style(QString name = "");

  // JSON conversion
  Style(const QJsonObject& json);         /**< @brief Construct Style from JSON */
  bool fromJson(const QJsonObject& json); /**< @brief Update Style from JSON */
  QJsonObject toJson() const;             /**< @brief convert Style to JSON */

  Style* copy() const; /**< @brief create a copy*/

  // getter und setter
  float get(AM::WaterValue what, Limit limit) const;       /**< @brief get requested value */
  void set(AM::WaterValue what, Limit limit, float value); /**< @brief set given type with value */
  bool isLimited(AM::WaterValue what);                     /**< @brief is value limited */
  void limit(AM::WaterValue what, bool limit);             /**< @brief Limit value */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex& index) const;

 private:
  bool limited[static_cast<int>(AM::WaterValue::Size)];
  float limits[static_cast<int>(AM::WaterValue::Size)][static_cast<int>(Limit::Size)];

  static const inline QString jsonKeys[static_cast<int>(Limit::Size)] = {"Min", "Target", "Max"};
};

#endif // STYLE_H
