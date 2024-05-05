// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXADDITIVEWIDGET_H
#define MIXADDITIVEWIDGET_H

#include "../additive/additive.h"

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
   * @param additiveDb Pointer to out additive database
   * @param parent QObject parent
   */
  MixAdditiveWidget(Additive* mixtureAdditive, Additive* additiveDb, QWidget* parent = nullptr);

 private slots:
  void update();

 private:
  void mergeAdditive(); /** @brief Merge mixture additive and additive db */

  Additive* aMix;                                                   /** @brief Additive in mixture */
  Additive* aDb;                                                    /** @brief Additive data base*/
  QGridLayout* layout;                                              /**< @brief Main layout */
  QLabel* formulas[static_cast<int>(Additive::Value::Size)];        /**< @brief Chemical formulas of additive */
  QLabel* texts[static_cast<int>(Additive::Value::Size)];           /**< @brief Name of additive */
  QLabel*
      percents[static_cast<int>(Additive::Value::lastLiquid) + 1];  /**< @brief Volume percent for liquid additives */
  QDoubleSpinBox* amounts[static_cast<int>(Additive::Value::Size)]; /** @brief Spin boxes of additive */
};

#endif  // MIXADDITIVEWIDGET_H
