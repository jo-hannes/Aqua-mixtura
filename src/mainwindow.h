// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainmodel.h"

#include "view/additivewindow.h"
#include "view/maltwindow.h"
#include "view/watersourcewindow.h"

#include <QMainWindow>
#include <QPushButton>

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

 private:
  void setupMenuBar(); /**< @brief Creates menu bor for main window */
  MainModel* model;    /**< @brief Pointer to main model */

  WatersourceWindow* wsources; /**< @brief Window for editing water sources */
  AdditiveWindow* wadditives;  /**< @brief Window for editing additives */

  MaltWindow* wmalts;    /**< @brief Window for editing malts */
  QString txtMalts;      /**< @brief Text of malt window button */
  QPushButton* btnMalts; /**< @brief Button for opening malt window */

  // Pointer to all buttons
  QPushButton *btnSources, *btnAdditions, *btnStyles, *btnLimits;
};
#endif  // MAINWINDOW_H
