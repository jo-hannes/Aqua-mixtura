// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXADDITIVEWIDGET_H
#define MIXADDITIVEWIDGET_H

#include "../additive/additive.h"
#include "../additive/additivesettings.h"

#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QObject>

/**
 * @brief Widget for additive in mixture window
 */
class MixAdditiveWidget : public QFrame {
  Q_OBJECT
 public:
  /**
   * @brief Create additive Widget
   * @param mixtureAdditive Pointer to additive of mixture
   * @param additiveCfg Reference to out additive settings
   * @param parent QObject parent
   */
  MixAdditiveWidget(Additive* mixtureAdditive, AdditiveSettings& additiveCfg, QWidget* parent = nullptr);

 public slots:
  void setTotalWater(double volume);
  void setStrikeWater(double volume);
  void setSpargingWater(double volume);
  void update();

 private slots:
  void valueChange(uint idx, double val);

 private:
  Additive* aMix;             /**< @brief Additive in mixture */
  AdditiveSettings& aCfg;     /**< @brief Additive settings */
  bool valChangeGuard{false}; /**< @brief Guard to avoid value changes on ui updates */
  double total{0};            /**< @brief Total water amount */
  double strike{0};           /**< @brief Strike water amount (Hauptguss) */
  double sparging{0};         /**< @brief Sparging water amount (Nachguss) */

  std::array<QLabel*, static_cast<int>(Additive::Value::lastLiquid) + 1> percents{}; /**< @brief Weight percent */
  std::array<QDoubleSpinBox*, static_cast<int>(Additive::Value::Size)> amounts{}; /**< @brief Spin boxes of additive */
  std::array<QLabel*, static_cast<int>(Additive::Value::Size)> lStrike{};         /**< @brief Amount for strike water */
  std::array<QLabel*, static_cast<int>(Additive::Value::Size)> lSparging{}; /**< @brief Amount for sparging water */
};

#endif  // MIXADDITIVEWIDGET_H
