// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVEWINDOW_H
#define ADDITIVEWINDOW_H

#include "additive.h"
#include "additivesettings.h"

#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
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
   * @param model Reference to model for accessing data
   * @param parent
   */
  explicit AdditiveWindow(AdditiveSettings& model, QWidget* parent = nullptr);

  void closeEvent(QCloseEvent* event) override;

 private slots:
  void cancel();              /**< @brief Revert changes */
  void selectUnit(int index); /**< @brief Select unit (via QComboBox) */

 private:
  void update(); /**< @brief Update all values */

  AdditiveSettings& additive; /**< @brief Reference to model for accessing data */

  // Pointer to UI elements
  QDoubleSpinBox* concentrations[static_cast<int>(Additive::Value::lastLiquid) + 1];
  QComboBox* unitSelect;
};

#endif // ADDITIVEWINDOW_H
