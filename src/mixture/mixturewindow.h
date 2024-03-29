// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTUREWINDOW_H
#define MIXTUREWINDOW_H

#include "mixture.h"

#include "../additive/additive.h"
#include "../limits/limits.h"
#include "../malt/malts.h"
#include "../style/styles.h"
#include "../water/watersources.h"

#include <QWidget>

class MixtureWindow : public QWidget {
  Q_OBJECT
 public:
  explicit MixtureWindow(Mixture& mixture, WaterSources* waterDb, Additive* additiveDb, Malts* maltDb, Styles* styleDb,
                         Limits* limits, QWidget* parent = nullptr);

 public slots:
  void updateName();

 private:
  Mixture& mix;
};

#endif  // MIXTUREWINDOW_H
