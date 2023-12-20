// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLE_H
#define STYLE_H

#include "../global.h"

#include <QJsonObject>
#include <QString>

/**
 * @brief Class for a single beer style
 */
class Style
{
 public:
  /**
   * @brief Type of limits
   */
  enum class Limit { LowerLimit, LowerRecomendation, UpperRecomendation, UpperLimit, Size };

  Style(QString name = "");

  // getter und setter
  QString getName() const;
  void setName(const QString& newName);
  float get(AM::WaterValue what, Limit limit) const;       /**< @brief get requested value */
  void set(AM::WaterValue what, Limit limit, float value); /**< @brief set given type with value */
  bool isLimited(AM::WaterValue what);                     /**< @brief is value limited */
  void limit(AM::WaterValue what, bool limit);             /**< @brief Limit value */

  // JSON conversion
  static Style fromJson(const QJsonObject& json); /**< @brief convert JSON to Style */
  QJsonObject toJson() const;                     /**< @brief convert this Style to JSON */

 protected:
  QString name;
  bool limited[static_cast<int>(AM::WaterValue::Size)];
  float limits[static_cast<int>(AM::WaterValue::Size)][static_cast<int>(Limit::Size)];

  static const inline QString jsonKeys[static_cast<int>(Limit::Size)] = {"LowerLimit", "LowerRecomendation",
                                                                         "UpperRecomendation", "UpperLimit"};
};

#endif // STYLE_H
