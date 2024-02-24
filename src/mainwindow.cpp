// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainwindow.h"

#include "mixture/mixture.h"
#include "water/water.h"
#include "water/waterprofileview.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Aqua mixtura");

  // initialize all models
  model = new MainModel();

  setupMenuBar();

  // Headlines
  QLabel* txtSettings = new QLabel("<b>" + tr("Einstellungen") + "</b>");
  QLabel* txtMix = new QLabel("<b>" + tr("Aufbereitung") + "</b>");

  // settings
  QVBoxLayout* btnLayout = new QVBoxLayout();
  btnLayout->addWidget(txtSettings);
  btnSources = new QPushButton(tr("Wasserquellen"));
  QObject::connect(btnSources, &QPushButton::clicked, this, &MainWindow::sources);
  btnLayout->addWidget(btnSources);
  btnAdditions = new QPushButton(tr("Zusatzstoffe"));
  QObject::connect(btnAdditions, &QPushButton::clicked, this, &MainWindow::additions);
  btnLayout->addWidget(btnAdditions);

  txtMalts = tr("Malze");
  btnMalts = new QPushButton(txtMalts);
  QObject::connect(btnMalts, &QPushButton::clicked, this, &MainWindow::malts);
  btnLayout->addWidget(btnMalts);

  btnStyles = new QPushButton(tr("Bierstile"));
  QObject::connect(btnStyles, &QPushButton::clicked, this, &MainWindow::styles);
  btnLayout->addWidget(btnStyles);
  btnLimits = new QPushButton(tr("Beschränkungen"));
  QObject::connect(btnLimits, &QPushButton::clicked, this, &MainWindow::limits);
  btnLayout->addWidget(btnLimits);

  // sticking it together
  QGridLayout* mainLayout = new QGridLayout();
  mainLayout->addWidget(txtSettings, 0, 0, Qt::AlignLeft | Qt::AlignTop);
  mainLayout->addWidget(txtMix, 0, 1, Qt::AlignLeft | Qt::AlignTop);
  mainLayout->addLayout(btnLayout, 1, 0);
  // mainLayout->addLayout(btnLayount, 1, 1);
  QWidget* mainWidget = new QWidget();
  mainWidget->setLayout(mainLayout);

  // create separate windows
  wsources = new WatersourceWindow(model, this);
  wsources->setWindowFlags(Qt::Window);
  wadditives = new AdditiveWindow(model, this);
  wadditives->setWindowFlags(Qt::Window);
  wmalts = new MaltWindow(model, this);
  wmalts->setWindowFlags(Qt::Window);
  QObject::connect(wmalts, &MaltWindow::maltWindowUnsavedChanges, this, &MainWindow::unsavedMalts);

  wstyles = new StylesWindow(model, this);
  wstyles->setWindowFlags(Qt::Window);

  setCentralWidget(mainWidget);

  // Playground
  Mixture* mix = new Mixture();
  Water* wNbg = new Water("Leitungswasser", 30, 47, 14, 6.5, 199, 11, 17, 0.11, 0);
  Water* wDest = new Water("VE-Wasser", 10);
  mix->AddWater(*wNbg);
  mix->AddWater(*wDest);
  Water result = mix->calc();

  QVBoxLayout* mixL = new QVBoxLayout();
  mixL->addWidget(new WaterProfileView(*wNbg, true), Qt::AlignTop);
  mixL->addWidget(new WaterProfileView(*wDest, true), Qt::AlignTop);
  mixL->addWidget(new QWidget());
  mixL->addWidget(new WaterProfileView(result, true), Qt::AlignBottom);

  QWidget* mixWidget = new QWidget();
  mixWidget->setLayout(mixL);

  QScrollArea* scrollArea = new QScrollArea(this);
  scrollArea->setWidget(mixWidget);
  scrollArea->setWindowFlags(Qt::Window);
  scrollArea->show();
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

void MainWindow::sources() {
  wsources->show();
}

void MainWindow::additions() {
  wadditives->show();
}

void MainWindow::malts() {
  wmalts->show();
}

void MainWindow::unsavedMalts(bool unsaved) {
  if (unsaved) {
    btnMalts->setText("* " + txtMalts);
  } else {
    btnMalts->setText(txtMalts);
  }
}

void MainWindow::styles() {
  wstyles->show();
}

void MainWindow::limits() {
  QMessageBox::information(this, "limits", "TODO limits");
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
