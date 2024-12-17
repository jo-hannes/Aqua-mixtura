// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXRESULTWIDGET_H
#define MIXRESULTWIDGET_H

#include "mixture.h"
#include "resultbar.h"

#include "../settings/settings.h"
#include "../style/styles.h"
#include "../water/water.h"

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QObject>

/**
 * @brief Widget for showing results in mixture window
 */
class MixResultWidget : public QFrame {
  Q_OBJECT
 public:
  /**
   * @brief Create result Widget
   * @param mixture Reference to mixture
   * @param styleDb Reference to beer styles database
   * @param limits Pointer to configured limits
   * @param parent QObject parent
   */
  MixResultWidget(Mixture& mixture, Styles& styleDb, Settings& settings, QWidget* parent = nullptr);

  [[nodiscard]] bool isChanged() const; /**< @brief True if changes not saved */

 signals:
  void selectionChanged();

 public slots:
  void update();       /**< @brief Update all shown values */
  void updateStyles(); /**< @brief Update beer style selection */

 private slots:
  void selectStyle(int index); /**< @brief Select style (via QComboBox) */

 private:
  Mixture& mix;           /**< @brief Reference to mixture */
  Style mixStyle;         /**< @brief Backup of style from mix */
  Styles& sDb;            /**< @brief Reference to beer styles database */
  Settings& lim;          /**< @brief Reference to configured limits */
  QComboBox* styleSelect; /**< @brief ComboBox for selecting beer style */
  int styleIdx;           /**< @brief Index of selected style */

  std::array<QLabel*, static_cast<int>(Water::Value::Size)> vals{};    /**< @brief Labels with calculated valued */
  std::array<ResultBar*, static_cast<int>(Water::Value::Size)> bars{}; /**< @brief Graphical bar indicating value */
};

#endif  // MIXRESULTWIDGET_H
