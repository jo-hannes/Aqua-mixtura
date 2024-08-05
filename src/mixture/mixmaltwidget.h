// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXMALTWIDGET_H
#define MIXMALTWIDGET_H

#include "../malt/malts.h"

#include <QFrame>
#include <QMenu>
#include <QObject>
#include <QTableView>

/**
 * @brief Widget for malts in mixture window
 */
class MixMaltWidget : public QFrame {
  Q_OBJECT
 public:
  /**
   * @brief Create malts Widget
   * @param mixtureMalts Pointer to malts of mixture
   * @param maltDb Reference to our malt database
   * @param parent QObject parent
   */
  MixMaltWidget(Malts* mixtureMalts, Malts& maltDb, QWidget* parent = nullptr);

 private slots:
  void add(int i);     /**< @brief Add a malt */
  void remove();       /**< @brief Delete the currently selected malt */
  void updateMaltDb(); /**< @brief The used malt data base is changed */

 signals:

 private:
  Malts* mMix;          /**< @brief Waters used in mixture */
  Malts& mDb;           /**< @brief Database with waters */
  QMenu* maltMenu;      /**< @brief Menu for add water button */
  QTableView* maltView; /**< @brief Table view for waters */
};

#endif  // MIXMALTWIDGET_H
