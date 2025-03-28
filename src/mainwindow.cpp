// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainwindow.h"

#include "version.h"

#include "common/buttons.h"
#include "common/dialogs.h"
#include "mixture/mixture.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  // Would be a very long initializer list => ignore error here
  // NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)
  setWindowTitle("Aqua-mixtura");

  setupMenuBar();

  // Headlines
  auto* txtSettings = new QLabel("<b>" + tr("Einstellungen") + "</b>");
  auto* txtMix = new QLabel("<b>" + tr("Aufbereitung") + "</b>");

  // settings
  auto* btnLayout = new QVBoxLayout();
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
  btnSettings = new QPushButton(tr("Einstellungen"));
  QObject::connect(btnSettings, &QPushButton::clicked, this, &MainWindow::settings);
  btnLayout->addWidget(btnSettings);
  btnLayout->addStretch(1);

  // Mixtures
  mixturesView = new QTableView();
  mixturesView->setModel(&model.mixtures);
  mixturesView->verticalHeader()->setVisible(false);
  mixturesView->resizeColumnsToContents();
  mixturesView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  QObject::connect(mixturesView, &QTableView::doubleClicked, this, &MainWindow::mixDoubleClicked);
  auto* mixBtns = new Buttons(tr("Aufbereitung hinzufügen"), tr("Aufbereitung kopieren"), tr("Aufbereitung löschen"),
                              tr("Aufbereitung importieren"), tr("Aufbereitung exportieren"), "", "");
  QObject::connect(mixBtns->btnAdd, &QPushButton::clicked, this, &MainWindow::mixAdd);
  QObject::connect(mixBtns->btnCopy, &QPushButton::clicked, this, &MainWindow::mixCopy);
  QObject::connect(mixBtns->btnDelete, &QPushButton::clicked, this, &MainWindow::mixDelete);
  QObject::connect(mixBtns->btnImport, &QPushButton::clicked, this, &MainWindow::mixImport);
  QObject::connect(mixBtns->btnExport, &QPushButton::clicked, this, &MainWindow::mixExport);

  // sticking it together
  auto* mainLayout = new QGridLayout();
  mainLayout->addWidget(txtSettings, 0, 0, Qt::AlignLeft | Qt::AlignTop);
  mainLayout->addLayout(btnLayout, 1, 0);
  mainLayout->addWidget(txtMix, 0, 1, Qt::AlignLeft | Qt::AlignTop);
  mainLayout->addWidget(mixturesView, 1, 1);
  mainLayout->addWidget(mixBtns, 2, 1);
  // mainLayout->addLayout(btnLayount, 1, 1);
  auto* mainWidget = new QWidget();
  mainWidget->setLayout(mainLayout);

  // create separate windows
  wsources = new WatersourceWindow(model.mixtures.waterDb, this);
  wsources->setWindowFlags(Qt::Window);

  wadditives = new AdditiveWindow(model.mixtures.additiveCfg, this);
  wadditives->setWindowFlags(Qt::Window);

  wmalts = new MaltWindow(model.mixtures.maltDb, this);
  wmalts->setWindowFlags(Qt::Window);
  QObject::connect(wmalts, &MaltWindow::maltWindowUnsavedChanges, this, &MainWindow::unsavedMalts);

  wstyles = new StylesWindow(model.mixtures.styleDb, this);
  wstyles->setWindowFlags(Qt::Window);

  wSettings = new SettingsWindow(model.mixtures.settings, this);
  wSettings->setWindowFlags(Qt::Window);

  setCentralWidget(mainWidget);
  // NOLINTEND(cppcoreguidelines-prefer-member-initializer)
}

void MainWindow::closeEvent(QCloseEvent* event) {
  // close all other windows
  if (!wsources->close()) {
    event->ignore();
    return;
  }
  if (!wadditives->close()) {
    event->ignore();
    return;
  }
  if (!wmalts->close()) {
    event->ignore();
    return;
  }
  if (!wstyles->close()) {
    event->ignore();
    return;
  }
  if (!wSettings->close()) {
    event->ignore();
    return;
  }
  // close all mixtures
  if (!model.mixtures.closeAll()) {
    event->ignore();
    return;
  }
  event->accept();
}

void MainWindow::save() {
  model.save();
}

void MainWindow::about() {
  Dialogs::info(QStringLiteral("Aqua-mixtura %1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_PATCH),
                tr("Rechner zur Wasseraufbereitung fürs Bierbrauen.<br>"
                   "<br>"
                   "<a href=\"https://github.com/jo-hannes/Aqua-mixtura\">github.com/jo-hannes/Aqua-mixtura</a>"),
                this);
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

void MainWindow::settings() {
  wSettings->show();
}

void MainWindow::mixAdd() {
  Mixture m;
  m.setName("New");
  m.save();
  model.mixtures.addMixture(m);
  model.mixtures.save();
}

void MainWindow::mixCopy() {
  const QModelIndex idx = mixturesView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  Mixture m = model.mixtures.getMixture(idx.row());
  m.updateCreationTime();
  m.newUuid();
  m.resetPath();
  m.setName("Copy of " + m.getName());
  m.save();
  model.mixtures.addMixture(m);
  model.mixtures.save();
}

void MainWindow::mixDelete() {
  const QModelIndex idx = mixturesView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  const int ret = Dialogs::yesNo(tr("Aufbereitung wirklich löschen?"), model.mixtures.getMixture(idx.row()).getName());
  if (ret == QMessageBox::Yes) {
    model.mixtures.deleteMixture(idx.row());
    model.mixtures.save();
  }
}

void MainWindow::mixImport() {
  const QString path = QFileDialog::getOpenFileName(this, tr("Aufbereitung importieren"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!model.mixtures.importMixture(path)) {
    Dialogs::info(tr("Fehler beim Importieren"), tr("Konnte Aufbereitung nicht im JSON finden"));
  } else {
    model.mixtures.save();
  }
}

void MainWindow::mixExport() {
  const QModelIndex idx = mixturesView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  const QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                                    model.mixtures.getMixture(idx.row()).getName() + ".json";
  const QString path =
      QFileDialog::getSaveFileName(this, tr("Aufbereitung exportieren"), suggestedFileName, "JSON (*.json)");
  if (path.isEmpty()) {
    return;
  }
  if (!model.mixtures.exportMixture(path, idx.row())) {
    Dialogs::info(tr("Fehler beim Exportieren"), tr("Konnte Aufbereitung nicht speichern"));
  }
}

void MainWindow::mixDoubleClicked(const QModelIndex& idx) {
  model.mixtures.show(idx.row());
}

void MainWindow::setupMenuBar() {
  // actions
  auto* settingsAct = new QAction(tr("Preferences..."), this);
  settingsAct->setStatusTip(tr("Die Anwendung Konfigurieren"));
  settingsAct->setShortcut(QKeySequence::Preferences);
  settingsAct->setMenuRole(QAction::PreferencesRole);
  connect(settingsAct, &QAction::triggered, this, &MainWindow::settings);

  auto* saveAct = new QAction(tr("Save"), this);
  saveAct->setStatusTip(tr("Save changes"));
  saveAct->setShortcut(QKeySequence::Save);
  connect(saveAct, &QAction::triggered, this, &MainWindow::save);

  auto* exitAct = new QAction(tr("Exit"), this);
  exitAct->setStatusTip(tr("Exit the application"));
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setMenuRole(QAction::QuitRole);
  connect(exitAct, &QAction::triggered, this, &QWidget::close);

  auto* aboutAct = new QAction(tr("About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  aboutAct->setMenuRole(QAction::AboutRole);
  connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

  auto* aboutQtAct = new QAction(tr("About Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  aboutQtAct->setMenuRole(QAction::AboutQtRole);
  connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

  // menu bar
  QMenu* fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(settingsAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(exitAct);
  QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
}
