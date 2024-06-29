// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "limits.h"

#include <QTableView>
#include <QWidget>

/**
 * @brief Window for defining settings
 */
class SettingsWindow : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief SettingsWindow constructor
   * @param model Pointer to model for accessing data
   * @param parent
   */
  explicit SettingsWindow(Limits* model, QWidget* parent = nullptr);

 private slots:
  void settingsImport(); /**< @brief Import JSON settings */
  void settingsExport(); /**< @brief Export settings as JSON */

 private:
  Limits* limits;
  QTableView* limitsView;
};

#endif  // SETTINGSWINDOW_H
