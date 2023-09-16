// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersourcewindow.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QStyleFactory>
#include <QVBoxLayout>

WatersourceWindow::WatersourceWindow(MainModel* model, QWidget* parent) : QWidget(parent) {
  this->model = model;
  selected = -1;
  // Überschriften
  QLabel* txtQuellen = new QLabel(tr("Wasserquellen"));
  txtQuellen->setStyleSheet("font-weight: bold");

  QLabel* txtProfil = new QLabel(tr("Wasserprofil"));
  txtProfil->setStyleSheet("font-weight: bold");

  // Quellen
  QVBoxLayout* layoutQuellen = new QVBoxLayout();

  QListView* sourcesView = new QListView();
  sourcesView->setModel(model->sources);
  layoutQuellen->addWidget(sourcesView);

  // QStyle* style = QStyleFactory::create("");
  qDebug("Styles:");
  for (auto st : QStyleFactory::keys()) {
    qDebug() << "Style: '" << st << "'";
  }

  QPushButton* btnAdd = new QPushButton();
  btnAdd->setToolTip(tr("Wasserquelle hinzufügen"));
  btnAdd->setIcon(QIcon(":/icons/document-plus.svg"));
  QPushButton* btnCopy = new QPushButton();
  btnCopy->setToolTip(tr("Wasserquelle kopieren"));
  btnCopy->setIcon(QIcon(":/icons/document-duplicate.svg"));
  QPushButton* btnDelete = new QPushButton();
  btnDelete->setToolTip(tr("Wasserquelle löschen"));
  btnDelete->setIcon(QIcon(":/icons/document-minus.svg"));
  QPushButton* btnImport = new QPushButton();
  btnImport->setToolTip(tr("Wasserquelle importieren"));
  btnImport->setIcon(QIcon(":/icons/document-arrow-down.svg"));
  QPushButton* btnExport = new QPushButton();
  btnExport->setToolTip(tr("Wasserquelle exportieren"));
  btnExport->setIcon(QIcon(":/icons/document-arrow-up.svg"));
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
  waterEdit->setProfile(model->sources->getProfile(selected));
}

void WatersourceWindow::saveProfile(WaterProfile& profile) {
  if (selected >= 0 && selected < model->sources->rowCount()) {
    model->sources->updateProfile(profile, selected);
    model->saveSources();
    // sourcesView->update(); // for some reason this crashes
  }
}
