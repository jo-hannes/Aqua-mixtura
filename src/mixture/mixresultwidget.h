// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXRESULTWIDGET_H
#define MIXRESULTWIDGET_H

#include "mixture.h"
#include "resultbar.h"

#include "../common/global.h"
#include "../limits/limits.h"
#include "../style/styles.h"

#include <QFrame>
#include <QLabel>
#include <QObject>

class MixResultWidget : public QFrame {
  Q_OBJECT
 public:
  MixResultWidget(Mixture& mixture, Styles* styleDb, Limits* limits, QWidget* parent = nullptr);

 public slots:
  void update(void);

 private:

  Mixture& mix;
  Styles* sDb;
  Limits* lim;

  QLabel* vals[static_cast<int>(AM::WaterValue::Size)];
  ResultBar* bars[static_cast<int>(AM::WaterValue::Size)];
};

#endif  // MIXRESULTWIDGET_H
