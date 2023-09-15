// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERSOURCEWINDOW_H
#define WATERSOURCEWINDOW_H

#include "waterprofileedit.h"

#include "../model/watersources.h"

#include <QListView>
#include <QObject>
#include <QWidget>

class WatersourceWindow : public QWidget {
  Q_OBJECT

 public:
  explicit WatersourceWindow(WaterSources* srcs, QWidget* parent = nullptr);

 public slots:
  void selectSource(const QModelIndex& index);
  void saveProfile(WaterProfile& profile);

 private:
  WaterSources* sources;
  QListView* sourcesView;
  WaterProfileEdit* waterEdit;
  int selected;
};

#endif  // WATERSOURCEWINDOW_H
