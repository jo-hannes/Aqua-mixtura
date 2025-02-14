// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXWATERWIDGET_H
#define MIXWATERWIDGET_H

#include "../water/watersources.h"

#include <QDoubleSpinBox>
#include <QFrame>
#include <QMenu>
#include <QTableView>

/**
 * @brief Widget for water sources in mixture window
 */
class MixWaterWidget : public QFrame {
  Q_OBJECT
 public:
  /**
   * @brief Create water sources Widget
   * @param mixtureWaters Reference to waters of mixture
   * @param waterDb Reference to our water database
   * @param parent QObject parent
   */
  explicit MixWaterWidget(WaterSources& mixtureWaters, WaterSources& waterDb, QWidget* parent = nullptr);

 private slots:
  void add(int i);      /**< @brief Slot for adding water */
  void remove();        /**< @brief Slot to delete the currently selected water */
  void updateWaterDb(); /**< @brief The used water data base is changed */

 signals:

 private:
  WaterSources& wMix;        /**< @brief Waters used in mixture */
  QDoubleSpinBox* wTotal;    /**< @brief Total water amount */
  QDoubleSpinBox* wStrike;   /**< @brief Strike water amount */
  QDoubleSpinBox* wSparging; /**< @brief Sparging water amount */
  WaterSources& wDb;         /**< @brief Database with waters */
  QMenu* waterMenu;          /**< @brief Menu for add water button */
  QTableView* waterView;     /**< @brief Table view for waters */
};

#endif  // MIXWATERWIDGET_H
