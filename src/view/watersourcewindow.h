// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERSOURCEWINDOW_H
#define WATERSOURCEWINDOW_H

#include "waterprofileedit.h"

#include "../mainmodel.h"

#include <QListView>
#include <QObject>
#include <QWidget>

/**
 * @brief Window for editing water sources/profiles
 */
class WatersourceWindow : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor
   * @param model Pointer to main model for accessing data
   * @param parent
   */
  explicit WatersourceWindow(MainModel* model, QWidget* parent = nullptr);

 public slots:
  void selectSource(const QModelIndex& index); /**< @brief Select source at index for profile editor */
  void saveProfile(Water& profile);            /**< @brief Save changed profile */

  void profileAdd();    /**< @brief Add a new profile */
  void profileCopy();   /**< @brief Copy a existing profile */
  void profileDelete(); /**< @brief Delete the selected profile */
  void profileImport(); /**< @brief Import a JSON profile */
  void profileExport(); /**< @brief Export a profile as JSON */

 private:
  MainModel* model;            /**< @brief Pointer to main model for accessing data */
  QListView* sourcesView;      /**< @brief List with all water sources */
  WaterProfileEdit* waterEdit; /**< @brief Water profile editor */
  int selected;                /**< @brief Index of currently selected water profile */
};

#endif  // WATERSOURCEWINDOW_H
