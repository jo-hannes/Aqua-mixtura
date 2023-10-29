// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERPROFILEVIEW_H
#define WATERPROFILEVIEW_H

#include "../model/water.h"
#include "../model/waterprofile.h"

#include <QLabel>
#include <QObject>
#include <QWidget>

class WaterProfileView : public QWidget {
  Q_OBJECT

 public:
  explicit WaterProfileView(QWidget* parent = nullptr);
  explicit WaterProfileView(const WaterProfile& profile, bool showHeader = true, QWidget* parent = nullptr);
  explicit WaterProfileView(const Water& water, bool showHeader = true, QWidget* parent = nullptr);

  void setProfile(const WaterProfile& profile);
  void setWater(const Water& water);
  void showHeader(bool show);

 private:
  QLabel *name, *amount, *unit;
  QLabel *calcium, *magnesium, *natrium;
  QLabel *hydrogencarbonat, *chlorid, *sulfat, *phosphat, *lactat;
  QLabel *restalkallität, *gesammtHaerte, *carbonHaerte, *caHaerte, *mgHaerte, *nichtcarbonHaerte, *so4cl;
};

#endif // WATERPROFILEVIEW_H
