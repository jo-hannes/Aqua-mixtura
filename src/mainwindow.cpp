// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainwindow.h"

#include "common/buttons.h"
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

  // Mixtures
  mixturesView = new QTableView();
  mixturesView->setModel(model->mixtures);
  mixturesView->verticalHeader()->setVisible(false);
  mixturesView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  QObject::connect(mixturesView, &QTableView::doubleClicked, this, &MainWindow::mixDoubleClicked);
  Buttons* mixBtns =
      new Buttons(tr("Aufbereitung hinzufügen"), tr("Aufbereitung kopieren"), tr("Aufbereitung löschen"),
                  tr("Aufbereitung importieren"), tr("Aufbereitung exportieren"), tr("Speichern"), tr("Abbrechen"));
  QObject::connect(mixBtns->btnAdd, &QPushButton::clicked, this, &MainWindow::mixAdd);
  QObject::connect(mixBtns->btnCopy, &QPushButton::clicked, this, &MainWindow::mixCopy);
  QObject::connect(mixBtns->btnDelete, &QPushButton::clicked, this, &MainWindow::mixDelete);
  QObject::connect(mixBtns->btnImport, &QPushButton::clicked, this, &MainWindow::mixImport);
  QObject::connect(mixBtns->btnExport, &QPushButton::clicked, this, &MainWindow::mixExport);
  QObject::connect(mixBtns->btnSave, &QPushButton::clicked, this, &MainWindow::mixSave);
  QObject::connect(mixBtns->btnCancel, &QPushButton::clicked, this, &MainWindow::mixDiscard);

  // sticking it together
  QGridLayout* mainLayout = new QGridLayout();
  mainLayout->addWidget(txtSettings, 0, 0, Qt::AlignLeft | Qt::AlignTop);
  mainLayout->addLayout(btnLayout, 1, 0);
  mainLayout->addWidget(txtMix, 0, 1, Qt::AlignLeft | Qt::AlignTop);
  mainLayout->addWidget(mixturesView, 1, 1);
  mainLayout->addWidget(mixBtns, 2, 1);
  // mainLayout->addLayout(btnLayount, 1, 1);
  QWidget* mainWidget = new QWidget();
  mainWidget->setLayout(mainLayout);

  // create separate windows
  wsources = new WatersourceWindow(model->mixtures->waterDb, this);
  wsources->setWindowFlags(Qt::Window);

  wadditives = new AdditiveWindow(model->mixtures->additiveDb, this);
  wadditives->setWindowFlags(Qt::Window);

  wmalts = new MaltWindow(model->mixtures->maltDb, this);
  wmalts->setWindowFlags(Qt::Window);
  QObject::connect(wmalts, &MaltWindow::maltWindowUnsavedChanges, this, &MainWindow::unsavedMalts);

  wstyles = new StylesWindow(model->mixtures->styleDb, this);
  wstyles->setWindowFlags(Qt::Window);

  wLimits = new LimitsWindow(model->mixtures->limits, this);
  wLimits->setWindowFlags(Qt::Window);

  setCentralWidget(mainWidget);

  // Playground
#if 0
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
#endif
}

MainWindow::~MainWindow() {
  delete model;
}

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
  wLimits->show();
}

void MainWindow::mixAdd() {
  Mixture m("New");
  model->mixtures->addMixture(m);
}

void MainWindow::mixCopy() {
  QModelIndex idx = mixturesView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  Mixture m = model->mixtures->getMixture(idx.row());
  m.updateCreationTime();
  m.newUuid();
  m.setName("Copy of " + m.getName());
  model->mixtures->addMixture(m);
}

void MainWindow::mixDelete() {
  QModelIndex idx = mixturesView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  model->mixtures->deleteMixture(idx.row());
}

void MainWindow::mixImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Aufbereitung Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!model->mixtures->importMixture(path)) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Aufbereitung nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void MainWindow::mixExport() {
  QModelIndex idx = mixturesView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              model->mixtures->getMixture(idx.row()).getName() + ".json";
  QString path =
      QFileDialog::getSaveFileName(this, tr("Aufbereitung Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!model->mixtures->exportMixture(path, idx.row())) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Aufbereitung nicht speichern"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void MainWindow::mixSave() {
  model->mixtures->save();
}

void MainWindow::mixDiscard() {
  model->mixtures->load();
}

void MainWindow::mixDoubleClicked(const QModelIndex& idx) {
  if (idx.column() != 0) {
    model->mixtures->show(idx.row());
  }
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
