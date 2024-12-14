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
  void update();
  void valueChange(int idx, double val);

 private:
  Additive* aMix;             /** @brief Additive in mixture */
  AdditiveSettings& aCfg;     /** @brief Additive settings */
  bool valChangeGuard{false}; /**< @brief Guard to avoid value changes on ui updates */
  QLabel* liquidUnit;         /**< @brief Unit for liquids */

  QLabel* percents[static_cast<int>(Additive::Value::lastLiquid) + 1]; /**< @brief Weight percent for liquids */
  QDoubleSpinBox* amounts[static_cast<int>(Additive::Value::Size)];    /**< @brief Spin boxes of additive */
};

#endif  // MIXADDITIVEWIDGET_H
