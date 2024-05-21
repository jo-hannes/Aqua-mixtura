// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLESWINDOW_H
#define STYLESWINDOW_H

#include "styles.h"

#include <QListView>
#include <QTableView>
#include <QWidget>

/**
 * @brief Window for beer styles
 */
class StylesWindow : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief StylesWindow constructor
   * @param model Pointer to model for accessing data
   * @param parent
   */
  explicit StylesWindow(Styles* model, QWidget* parent = nullptr);

 private slots:
  void styleSelectionChanged(const QModelIndex& current,
                             const QModelIndex& previous); /**< @brief Slot for selection changes */
  void styleAdd();                            /**< @brief Add a new style */
  void styleCopy();                           /**< @brief Copy a existing style */
  void styleDelete();                         /**< @brief Delete the selected style */
  void styleImport();                         /**< @brief Import a JSON style */
  void styleExport();                         /**< @brief Export a style as JSON */

 signals:
  void load();
  void save();
  void stylesWindowUnsavedChanges(bool changed); /**< @brief Singal to notify save state of styles */

 private:
  void styleSelect(const qsizetype index); /**< @brief Select style at index for style editor */

  Styles* styles;             /**< @brief Pointer to data model */
  QListView* stylesView;      /**< @brief View with list of styles */
  QTableView* styleTableView; /**< @brief View with style data */
  qsizetype selected;         /**< @brief Index of currently selected style */
};

#endif  // STYLESWINDOW_H
