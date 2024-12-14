// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLESWINDOW_H
#define STYLESWINDOW_H

#include "styles.h"

#include <QCloseEvent>
#include <QLineEdit>
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
   * @param model Reference to model for accessing data
   * @param parent
   */
  explicit StylesWindow(Styles& model, QWidget* parent = nullptr);

  void closeEvent(QCloseEvent* event) override;

 private slots:
  void setName(QString name);
  void styleAdd();                            /**< @brief Add a new style */
  void styleCopy();                           /**< @brief Copy a existing style */
  void styleDelete();                         /**< @brief Delete the selected style */
  void styleImport();                         /**< @brief Import a JSON style */
  void styleExport();                         /**< @brief Export a style as JSON */
  void stylesLoad();                          /**< @brief Load styles */
  void styleSelect(const QModelIndex& index); /**< @brief Select style at index for style editor */

 private:
  /**
   * @brief Check for unsaved changes and ask user how to handle this
   * @return 0                    No changes done we could save
   * @return QMessageBox::Save    User clicked "Save"
   * @return QMessageBox::Discard User clicked "Don't Save"
   * @return QMessageBox::Cancel  User clicked "Cancel"
   */
  int saveChangesDialog();

  Styles& styles;             /**< @brief Reference to data model */
  QListView* stylesView;      /**< @brief View with list of styles */
  QLineEdit* nameEdit;        /**< @brief Editor for style name */
  QTableView* styleTableView; /**< @brief View with style data */
  int selected{-1};           /**< @brief Index of currently selected style */
};

#endif  // STYLESWINDOW_H
