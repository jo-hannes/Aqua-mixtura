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

  layout->addWidget(new QLabel(tr("Ergebnis")), 0, 0, Qt::AlignLeft);
  layout->addWidget(new QLabel(tr("Bierstil:")), 0, 1, 1, 2, Qt::AlignLeft);

  styleSelect = new QComboBox();
  layout->addWidget(styleSelect, 0, 3, Qt::AlignLeft);
  QObject::connect(styleSelect, &QComboBox::activated, this, &MixResultWidget::selectStyle);

  // values
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    // Description
    layout->addWidget(new QLabel(AM::waterStrings[i][AM::JsonKey]), i + 1, 0);
    // value
    vals[i] = new QLabel();
    layout->addWidget(vals[i], i + 1, 1);
    // Unit
    layout->addWidget(new QLabel(AM::waterStrings[i][AM::Unit]), i + 1, 2);
    // Bars
    if (i != static_cast<int>(AM::WaterValue::Volume)) {
      bars[i] = new ResultBar();
      layout->addWidget(bars[i], i + 1, 3);
    }
  }

  update();
  updateStyles();
}

void MixResultWidget::update(void) {
  qDebug() << "Update reslut calculation";
  Water tst = mix.calc();
  for (int i = 0; i < static_cast<int>(AM::WaterValue::Size); i++) {
    vals[i]->setText(QString::number(tst.get(static_cast<AM::WaterValue>(i)), 'f', 2));
    // Update Bars
    if (i != static_cast<int>(AM::WaterValue::Volume)) {  // Skip volume
      bars[i]->setLimits(lim->getMin(static_cast<AM::WaterValue>(i)), lim->getMax(static_cast<AM::WaterValue>(i)));
      if (mix.style->isLimited(static_cast<AM::WaterValue>(i))) {
        bars[i]->setStyle(mix.style->get(static_cast<AM::WaterValue>(i), Style::Limit::Min),
                          mix.style->get(static_cast<AM::WaterValue>(i), Style::Limit::Target),
                          mix.style->get(static_cast<AM::WaterValue>(i), Style::Limit::Max));
      } else {
        bars[i]->setNoStyle();
      }
      bars[i]->setValue(tst.get(static_cast<AM::WaterValue>(i)));
    }
  }
}

void MixResultWidget::selectStyle(int index) {
  qDebug() << "TODO: Select style at index:" << index;
}

void MixResultWidget::updateStyles() {
  // save selection
  int selection = styleSelect->currentIndex();
  styleSelect->clear();
  // first add style from mixture
  QString self = "[" + mix.style->getName() + "]";
  styleSelect->addItem(self);
  // add styles form database
  for (int i = 0; i < sDb->rowCount(QModelIndex()); i++) {
    styleSelect->addItem(sDb->getStyle(i)->getName());
  }
  // restore selection
  if (selection >= 0) {
    styleSelect->setCurrentIndex(selection);
  }
}
