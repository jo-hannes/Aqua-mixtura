// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixmaltwidget.h"

#include "../common/buttons.h"
#include "../malt/malttabledelegate.h"

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

MixMaltWidget::MixMaltWidget(Malts* mixtureMalts, Malts& maltDb, QWidget* parent)
    : QFrame{parent}, mMix(mixtureMalts), mDb{maltDb} {
  // Build ui
  auto* layout = new QVBoxLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // heading
  auto* heading = new QLabel(tr("Malz"));
  layout->addWidget(heading);

  // View with malts
  maltView = new QTableView(this);
  maltView->setModel(mMix);
  maltView->verticalHeader()->setVisible(false);
  auto* delegate = new MaltTableDelegate(this);
  maltView->setItemDelegate(delegate);
  maltView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  layout->addWidget(maltView);

  // Buttons
  auto* buttons = new Buttons(tr("Malz hinzufügen"), "", tr("Malz löschen"), "", "", "", "");
  QObject::connect(buttons->btnAdd, &QPushButton::clicked, this, &MixMaltWidget::add);
  QObject::connect(buttons->btnDelete, &QPushButton::clicked, this, &MixMaltWidget::remove);
  layout->addWidget(buttons);

  // build button menu
  maltMenu = new QMenu(this);
  updateMaltDb();
  QObject::connect(&mDb, &Malts::dataModified, this, &MixMaltWidget::updateMaltDb);
  buttons->btnAdd->setMenu(maltMenu);
}

void MixMaltWidget::add(int i) {
  mMix->addMalt(mDb.getMalt(i));
}

void MixMaltWidget::remove() {
  const QModelIndex idx = maltView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  mMix->deleteMalt(idx.row());
}

void MixMaltWidget::updateMaltDb() {
  maltMenu->clear();
  for (int i = 0; i < mDb.rowCount(); i++) {
    auto* act = new QAction(mDb.getMalt(i).getName(), maltMenu);
    QObject::connect(act, &QAction::triggered, this, [=]() {
      add(i);
    });
    maltMenu->addAction(act);
  }
}
