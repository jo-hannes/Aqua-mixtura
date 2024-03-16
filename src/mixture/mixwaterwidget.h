// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXWATERWIDGET_H
#define MIXWATERWIDGET_H

#include "mixture.h"

#include <QDoubleSpinBox>
#include <QFrame>
#include <QTableView>
#include <QWidget>

class MixWaterWidget : public QFrame {
  Q_OBJECT
 public:
  explicit MixWaterWidget(Mixture& mixture, QWidget* parent = nullptr);

 public slots:
  void add();    /**< @brief Slot for adding water */
  void remove(); /**< @brief Slot to delete the currently selected water */

 signals:

 private:
  Mixture& mix;

  QTableView* waterView; /**< @brief Table view for malts */
};

#endif  // MIXWATERWIDGET_H
