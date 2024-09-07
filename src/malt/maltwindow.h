// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MALTWINDOW_H
#define MALTWINDOW_H

#include "malts.h"

#include <QTableView>
#include <QWidget>

class MaltWindow : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Create MaltWindow
   * @param model Reference to model
   * @param parent QWidget parent
   */
  explicit MaltWindow(Malts& model, QWidget* parent = nullptr);
  ~MaltWindow();

 private slots:
  void maltAdd();                  /**< @brief Slot for adding a new malt */
  void maltCopy();                 /**< @brief Slot to copy the currently selected malt */
  void maltDelete();               /**< @brief Slot to delete the currently selected malt */
  void maltImport();               /**< @brief Slot for importing a malt form JSON */
  void maltExport();               /**< @brief Slot for exporting a malt to JSON */
  void unsavedMalts();             /**< @brief Slot for checking if unsaved changes exist */

 signals:
  void load();
  void save();
  void maltWindowUnsavedChanges(bool changed); /**< @brief Singal to notify save state of malts */

 private:
  Malts& malts;          /**< @brief Reference to model for accessing data */
  QTableView* maltsView; /**< @brief Table view for malts */
  QString title;         /**< @brief Window tittle */
};

#endif // MALTWINDOW_H
