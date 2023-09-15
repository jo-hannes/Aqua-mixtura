// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersourcewindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

WatersourceWindow::WatersourceWindow(WaterSources* srcs, QWidget* parent) : QWidget(parent) {
  sources = srcs;
  selected = -1;
  // Überschriften
  QLabel* txtQuellen = new QLabel(tr("Wasserquellen"));
  txtQuellen->setStyleSheet("font-weight: bold");

  QLabel* txtProfil = new QLabel(tr("Wasserprofil"));
  txtProfil->setStyleSheet("font-weight: bold");

  // Quellen
  QVBoxLayout* layoutQuellen = new QVBoxLayout();

  QListView* sourcesView = new QListView();
  sourcesView->setModel(sources);
  layoutQuellen->addWidget(sourcesView);

  QPushButton* btnAdd = new QPushButton("+");
  QPushButton* btnCopy = new QPushButton("copy");
  QPushButton* btnDelete = new QPushButton("-");
  QPushButton* btnImport = new QPushButton("import");
  QPushButton* btnExport = new QPushButton("export");
  QHBoxLayout* buttonsQuellen = new QHBoxLayout();
  buttonsQuellen->addWidget(btnAdd);
  buttonsQuellen->addWidget(btnCopy);
  buttonsQuellen->addWidget(btnDelete);
  buttonsQuellen->addWidget(btnImport);
  buttonsQuellen->addWidget(btnExport);
  layoutQuellen->addLayout(buttonsQuellen);

  // Profil Editor
  waterEdit = new WaterProfileEdit();

  // sticking it together
  QGridLayout* layoutMain = new QGridLayout();
  layoutMain->addWidget(txtQuellen, 0, 0, Qt::AlignLeft);
  layoutMain->addWidget(txtProfil, 0, 1, Qt::AlignLeft);
  layoutMain->addLayout(layoutQuellen, 1, 0);
  layoutMain->addWidget(waterEdit, 1, 1);
  setLayout(layoutMain);

  QObject::connect(sourcesView, &QListView::pressed, this, &WatersourceWindow::selectSource);
  QObject::connect(waterEdit, &WaterProfileEdit::saveProfile, this, &WatersourceWindow::saveProfile);
}

void WatersourceWindow::selectSource(const QModelIndex& index) {
  selected = index.row();
  waterEdit->setProfile(sources->getProfile(selected));
}

void WatersourceWindow::saveProfile(WaterProfile& profile) {
  if (selected >= 0 && selected < sources->rowCount()) {
    sources->updateProfile(profile, selected);
    // sourcesView->update(); // for some reason this crashes
  }
}
