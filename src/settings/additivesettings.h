// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVESETTINGS_H
#define ADDITIVESETTINGS_H

#include "../additive/additive.h"
#include "../common/meta.h"

#include <QJsonObject>

/**
 * @brief Settings for additives
 */
class AdditiveSettings : public Meta {
 public:
  enum class LiquidUnit { gramm, milliLiter }; /**< @brief Used unit for liquids */

  AdditiveSettings();

  // JSON conversion
  AdditiveSettings(const QJsonObject& json); /**< @brief Create AdditiveSettings from JSON */
  bool fromJson(const QJsonObject& json);    /**< @brief Update AdditiveSettings from JSON */
  QJsonObject toJson() const;                /**< @brief convert this AdditiveSettings to JSON */
  void toJson(QJsonObject& json) const;      /**< @brief Append AdditiveSettings to JSON */

  float getConcentration(Additive::Value what) const;       /**< @brief get requested concentration */
  void setConcentration(Additive::Value what, float value); /**< @brief set given type with concentration */
  bool isEnabled(Additive::Value what) const;               /**< @brief get enable status */
  void enable(Additive::Value what, bool enable);           /**< @brief set enable status */
  LiquidUnit getLiquidUnit() const;                         /**< @brief get unit used for liquids */
  void setLiquidUnit(LiquidUnit newUnit);                   /**< @brief get unit used for liquids */

 private:
  bool enabled[static_cast<int>(Additive::Value::Size)];                  /**< @brief Enable status */
  float concentration[static_cast<int>(Additive::Value::lastLiquid) + 1]; /**< @brief Concentration in % */
  LiquidUnit unit;
};

#endif  // ADDITIVESETTINGS_H
