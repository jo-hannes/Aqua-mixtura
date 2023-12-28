// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLESWINDOW_H
#define STYLESWINDOW_H

#include "../mainmodel.h"

#include <QListView>
#include <QTableView>
#include <QWidget>

/**
 * @brief Window for diting beer styles
 */
class StylesWindow : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief StylesWindow constructor
   * @param model Pointer to main model for accessing data
   * @param parent
   */
  explicit StylesWindow(MainModel* model, QWidget* parent = nullptr);

 public slots:
  void styleSelectionChanged(const QModelIndex& current,
                             const QModelIndex& previous); /**< @brief Slot for selection changes */
  void styleSave();                           /**< @brief Save changed style */
  void styleDiscard();                        /**< @brief Discard changes */
  void styleAdd();                            /**< @brief Add a new style */
  void styleCopy();                           /**< @brief Copy a existing style */
  void styleDelete();                         /**< @brief Delete the selected style */
  void styleImport();                         /**< @brief Import a JSON style */
  void styleExport();                         /**< @brief Export a style as JSON */

 signals:
  void stylesWindowUnsavedChanges(bool changed); /**< @brief Singal to notify save state of styles */

 private:
  void styleSelect(const qsizetype index); /**< @brief Select style at index for style editor */

  MainModel* model;           /**< @brief Pointer to data model */
  QListView* stylesView;      /**< @brief View with list of styles */
  QTableView* styleTableView; /**< @brief View with style data */
  qsizetype selected;         /**< @brief Index of currently selected style */
};

#endif  // STYLESWINDOW_H
