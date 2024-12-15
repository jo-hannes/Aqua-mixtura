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
class AdditiveSettings : public QObject, public Meta {
  Q_OBJECT

 public:
  enum class LiquidUnit { gramm, milliLiter }; /**< @brief Used unit for liquids */

  AdditiveSettings();

  // JSON conversion
  explicit AdditiveSettings(const QJsonObject& json); /**< @brief Create AdditiveSettings from JSON */
  bool fromJson(const QJsonObject& json);             /**< @brief Update AdditiveSettings from JSON */
  [[nodiscard]] QJsonObject toJson() const;           /**< @brief convert this AdditiveSettings to JSON */
  void toJson(QJsonObject& json) const;               /**< @brief Append AdditiveSettings to JSON */

  [[nodiscard]] double getConcentration(Additive::Value what) const; /**< @brief get requested concentration */
  void setConcentration(Additive::Value what, double value);         /**< @brief set given type with concentration */
  [[nodiscard]] LiquidUnit getLiquidUnit() const;                    /**< @brief get unit used for liquids */
  void setLiquidUnit(LiquidUnit newUnit);                            /**< @brief get unit used for liquids */
  /**
   * @brief get density for liquids when unit is ml
   * Save to use in any case, will just return 1 when no liquid is selected or unit is g
   */
  [[nodiscard]] double getDensity(Additive::Value what) const;

  [[nodiscard]] bool isChanged() const; /**< @brief True if changes not saved */

 public slots:
  void load();
  void save();

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark unsaved/saved */
  bool changed;                  /**< @brief True if changed but not saved */

  double concentration[static_cast<int>(Additive::Value::lastLiquid) + 1]; /**< @brief Concentration in % */
  LiquidUnit unit;

  // clang-format off
  /**
   * @brief Coefficients for calculating density
   * Coefficients of formula a*x^2 + b*x + c
   */
  inline static const double densityCoefficients[static_cast<int>(Additive::Value::lastLiquid) +  1][3] = {
    //  c         b           a
      { 0.999137, 0.00236361, 0},          // c3h6o3
      { 0.997681, 0.00503334, 0},          // hcl
      { 0.993422, 0.00723747, 1.77195e-5}, // h2so4
      { 0.999386, 0.00491011, 3.76331e-5}  // h3po4
  };
  // clang-format on
};

#endif  // ADDITIVESETTINGS_H
