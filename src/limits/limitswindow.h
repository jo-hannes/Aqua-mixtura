// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef LIMITSWINDOW_H
#define LIMITSWINDOW_H

#include "limits.h"

#include <QTableView>
#include <QWidget>

/**
 * @brief Window for defining limits
 */
class LimitsWindow : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief LimitsWindow constructor
   * @param model Pointer to model for accessing data
   * @param parent
   */
  explicit LimitsWindow(Limits* model, QWidget* parent = nullptr);

 private slots:
  void limitsImport(); /**< @brief Import JSON limits */
  void limitsExport(); /**< @brief Export limits as JSON */

 signals:
  void load(); /**< @brief Load limits */
  void save(); /**< @brief Save limits*/

 private:
  Limits* limits;
  QTableView* limitsView;
};

#endif  // LIMITSWINDOW_H
