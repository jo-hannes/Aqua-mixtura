// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXADDITIVEWIDGET_H
#define MIXADDITIVEWIDGET_H

#include "../additive/additive.h"

#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
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

 private:
  void mergeAdditive(); /** @brief Merge mixture additive and additive db */
  void addAcid(int row, QString formula, QString text, QString percent, QDoubleSpinBox* ml);
  void addSolid(int row, QString formula, QString text, QDoubleSpinBox* g);

  Additive* aMix;                                                   /** @brief Additive in mixture */
  Additive* aDb;                                                    /** @brief Additive data base*/
  QGridLayout* layout;                                              /**< @brief Main layout */
  QDoubleSpinBox* amounts[static_cast<int>(Additive::Value::Size)]; /** @brief Spin boxes of additive */
};

#endif  // MIXADDITIVEWIDGET_H
