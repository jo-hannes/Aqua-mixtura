// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainwindow.h"

#include "mainmodel.h"

#include "model/mixture.h"
#include "model/water.h"
#include "view/additivewindow.h"
#include "view/waterprofileview.h"
#include "view/watersourcewindow.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Aqua mixtura");

  // Entscheidung: Es wird deutsch verwendet!

  // initialize all models
  MainModel model;

  setupMenuBar();

  Mixture* mix = new Mixture();
  Water* wNbg = new Water("Leitungswasser", 30, 47, 14, 6.5, 199, 11, 17, 0.11, 0);
  Water* wDest = new Water("VE-Wasser", 10);
  mix->AddWater(*wNbg);
  mix->AddWater(*wDest);
  Water result = mix->calc();

  QVBoxLayout* mainL = new QVBoxLayout();
  mainL->addWidget(new WaterProfileView(*wNbg, true), Qt::AlignTop);
  mainL->addWidget(new WaterProfileView(*wDest, true), Qt::AlignTop);
  mainL->addWidget(new QWidget());
  mainL->addWidget(new WaterProfileView(result, true), Qt::AlignBottom);

  QWidget* mainWidget = new QWidget();
  mainWidget->setLayout(mainL);

  QScrollArea* scrollArea = new QScrollArea;
  scrollArea->setWidget(mainWidget);

  tabWidget = new QTabWidget;
  // tabWidget->addTab(mainWidget, tr("Test"));
  WatersourceWindow* wsource = new WatersourceWindow(model.sources, this);
  tabWidget->addTab(wsource, tr("Wasserquellen"));
  tabWidget->addTab(new QWidget, tr("Zusatzstoffe"));
  tabWidget->addTab(new QWidget, tr("Malze"));
  tabWidget->addTab(new QWidget, tr("Bierstiele"));
  tabWidget->addTab(scrollArea, tr("Aufbereitung"));

  setCentralWidget(tabWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::settings() {
  QMessageBox::information(this, "settings", "TODO settings");
}

void MainWindow::save() {
  QMessageBox::information(this, "save", "TODO save");
}

void MainWindow::about() {
  QMessageBox::about(this, tr("About"), tr("TODO about this app"));
}

void MainWindow::setupMenuBar() {
  // actions
  QAction* settingsAct = new QAction(tr("Preferences..."), this);
  settingsAct->setStatusTip(tr("Die Anwendung Konfigurieren"));
  settingsAct->setShortcut(QKeySequence::Preferences);
  settingsAct->setMenuRole(QAction::PreferencesRole);
  connect(settingsAct, &QAction::triggered, this, &MainWindow::settings);

  QAction* saveAct = new QAction(tr("Save"), this);
  saveAct->setStatusTip(tr("Save changes"));
  saveAct->setShortcut(QKeySequence::Save);
  connect(saveAct, &QAction::triggered, this, &MainWindow::save);

  QAction* exitAct = new QAction(tr("E&xit"), this);
  exitAct->setStatusTip(tr("Exit the application"));
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setMenuRole(QAction::QuitRole);
  connect(exitAct, &QAction::triggered, this, &QWidget::close);

  QAction* aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  aboutAct->setMenuRole(QAction::AboutRole);
  connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

  QAction* aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  aboutQtAct->setMenuRole(QAction::AboutQtRole);
  connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

  // menu bar
  QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(settingsAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(exitAct);
  QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}
