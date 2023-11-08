// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainmodel.h"

#include "view/additivewindow.h"
#include "view/watersourcewindow.h"

#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  // menu slots
  void settings();
  void save();
  void about();

  // button slots
  void sources();
  void additions();
  void malts();
  void styles();
  void limits();

 private:
  void setupMenuBar();
  QTabWidget* tabWidget;
  MainModel* model;

  WatersourceWindow* wsources;
  AdditiveWindow* wadditives;

  QPushButton *btnSources, *btnAdditions, *btnMalts, *btnStyles, *btnLimits;
};
#endif  // MAINWINDOW_H
