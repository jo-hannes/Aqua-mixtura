// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainmodel.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void settings();
  void save();
  void about();

 private:
  void setupMenuBar();
  QTabWidget* tabWidget;
  MainModel* model;
};
#endif  // MAINWINDOW_H
