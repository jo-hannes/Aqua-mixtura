// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainmodel.h"

#include "additive/additivewindow.h"
#include "limits/limitswindow.h"
#include "malt/maltwindow.h"
#include "style/styleswindow.h"
#include "water/watersourcewindow.h"

#include <QMainWindow>
#include <QModelIndex>
#include <QPushButton>
#include <QVector>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  // menu slots
  void settings();
  void save();
  void about();

  // button + unsaved slots
  void sources();                  /**< @brief Slot to show window with water sources */
  void additions();                /**< @brief Slot to show window with additives */
  void malts();                    /**< @brief Slot to show window with malts */
  void unsavedMalts(bool unsaved); /**< @brief Slot for indicating of unsaved changes exist */
  void styles();                   /**< @brief Slot to show window with beer styles */
  void limits();                   /**< @brief Slot to show window with limits */

  // mixtures slots
  void mixAdd();     /**< @brief Slot for adding a new mixture */
  void mixCopy();    /**< @brief Slot to copy the currently selected mixture */
  void mixDelete();  /**< @brief Slot to delete the currently selected mixture */
  void mixImport();  /**< @brief Slot for importing a mixture form JSON */
  void mixExport();  /**< @brief Slot for exporting a mixture to JSON */
  void mixSave();    /**< @brief Slot for saving all mixtures */
  void mixDiscard(); /**< @brief Slot for discarding all unsaved mixture changes */
  void mixDoubleClicked(const QModelIndex& idx); /**< @brief Slot for double clicks on mixtures */

 private:
  void setupMenuBar(); /**< @brief Creates menu bor for main window */
  MainModel* model;    /**< @brief Pointer to main model */

  // Windows
  WatersourceWindow* wsources; /**< @brief Window for editing water sources */
  AdditiveWindow* wadditives;  /**< @brief Window for editing additives */
  MaltWindow* wmalts;          /**< @brief Window for editing malts */
  StylesWindow* wstyles;       /**< @brief Window for editing beer styles */
  LimitsWindow* wLimits;       /**< @brief Window for editing limits */
  QString txtMalts;            /**< @brief Text of malt window button */

  // Pointer to all buttons
  QPushButton *btnSources, *btnAdditions, *btnMalts, *btnStyles, *btnLimits;

  // mixtures
  QTableView* mixturesView;
};
#endif  // MAINWINDOW_H
