// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVEWINDOW_H
#define ADDITIVEWINDOW_H

#include "additive.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QObject>
#include <QString>
#include <QWidget>

/**
 * @brief Window for managing your additions
 */
class AdditiveWindow : public QWidget
{
  Q_OBJECT

 public:
  /**
   * @brief Constructor
   * @param model Pointer to model for accessing data
   * @param parent
   */
  explicit AdditiveWindow(Additive* model, QWidget* parent = nullptr);

 private slots:
  void cancel();      /**< @brief Revert changes */

 private:
  void update(); /**< @brief Update all values */
  void addAcid(int row, QCheckBox* check, QString text, QDoubleSpinBox* percent, QDoubleSpinBox* ml);
  void addSolid(int row, QCheckBox* check, QString text, QDoubleSpinBox* g);

  Additive* additive;  /**< @brief Pointer to model for accessing data */
  QGridLayout* layout; /**< @brief Main layout */

  // Pointer to UI elements
  QCheckBox* enabled[static_cast<int>(Additive::Value::Size)];
  QDoubleSpinBox* amounts[static_cast<int>(Additive::Value::Size)];
  QDoubleSpinBox* concentrations[static_cast<int>(Additive::Value::lastLiquid) + 1];
};

#endif // ADDITIVEWINDOW_H
