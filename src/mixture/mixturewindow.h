// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTUREWINDOW_H
#define MIXTUREWINDOW_H

#include "mixadditivewidget.h"
#include "mixmaltwidget.h"
#include "mixresultwidget.h"
#include "mixture.h"
#include "mixwaterwidget.h"

#include "../additive/additive.h"
#include "../limits/limits.h"
#include "../malt/malts.h"
#include "../style/styles.h"
#include "../water/watersources.h"

#include <QLineEdit>
#include <QWidget>

class MixtureWindow : public QWidget {
  Q_OBJECT
 public:
  explicit MixtureWindow(Mixture& mixture, WaterSources* waterDb, Additive* additiveDb, Malts* maltDb, Styles* styleDb,
                         Limits* limits, QWidget* parent = nullptr);

 public slots:
  void setName(QString name);
  void load();
  void save();

 private:
  Mixture& mix;
  QLineEdit* nameEdit;
  MixWaterWidget* mww;
  MixMaltWidget* mmw;
  MixAdditiveWidget* maw;
  MixResultWidget* mrw;
};

#endif  // MIXTUREWINDOW_H
