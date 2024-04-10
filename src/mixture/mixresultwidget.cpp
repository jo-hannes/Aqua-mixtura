// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixresultwidget.h"

#include <QGridLayout>

MixResultWidget::MixResultWidget(Mixture& mixture, Styles* styleDb, Limits* limits, QWidget* parent)
    : QFrame{parent}, mix{mixture} {
  Q_ASSERT(styleDb != nullptr);
  Q_ASSERT(limits != nullptr);
  sDb = styleDb;
  lim = limits;

  // set frame
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // build ui
  QGridLayout* layout = new QGridLayout();
  this->setLayout(layout);

  layout->addWidget(new QLabel(tr("Ergebnis")), 0, 0);

  // values
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // Description
    layout->addWidget(new QLabel(AM::waterStrings[i][AM::JsonKey]), i + 1, 0);
    // value
    vals[i] = new QLabel();
    layout->addWidget(vals[i], i + 1, 1);
    // Unit
    layout->addWidget(new QLabel(AM::waterStrings[i][AM::Unit]), i + 1, 2);
  }

  Water tst = mix.calc();
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    vals[i]->setText(QString::number(tst.get(static_cast<AM::WaterValue>(i)), 'f', 2));
  }

  // for some reasons these functions lead to a segfault => Need to check memory usage!
  // QObject::dumpObjectTree();
  // update();
  // updVals();
}

void MixResultWidget::update(void) {
  Water tst = mix.calc();
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    vals[i]->setText(QString::number(tst.get(static_cast<AM::WaterValue>(i)), 'f', 2));
  }
}

void MixResultWidget::updVals() {
  Water tst = mix.calc();
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    vals[i]->setText(QString::number(tst.get(static_cast<AM::WaterValue>(i)), 'f', 2));
  }
}
