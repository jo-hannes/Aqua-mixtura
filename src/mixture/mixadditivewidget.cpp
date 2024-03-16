// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixadditivewidget.h"

#include <QLabel>

MixAdditiveWidget::MixAdditiveWidget(Additive* mixtureAdditive, Additive* additiveDb, QWidget* parent)
    : QFrame{parent} {
  aMix = mixtureAdditive;
  aDb = additiveDb;
  mergeAdditive();

  layout = new QGridLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  int row = 0;
  // heading
  layout->addWidget(new QLabel(tr("Zusatzstoffe"), this), row++, 0, 1, 3, Qt::AlignLeft);

  // heading liquids
  layout->addWidget(new QLabel("<b>" + tr("Säuren") + "</b>", this), row, 0, 1, 2, Qt::AlignLeft);
  layout->addWidget(new QLabel("<b>%</b>", this), row, 2, Qt::AlignRight);
  layout->addWidget(new QLabel("<b>mL</b>", this), row, 3, Qt::AlignRight);
  row++;

  // liquids
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    Additive::Value what = static_cast<Additive::Value>(i);
    // only add enabled items
    if (aMix->isEnabled(what)) {
      amounts[i] = new QDoubleSpinBox(this);
      amounts[i]->setValue(aMix->get(what));
      addAcid(row++, Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)],
              Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)], "10", amounts[i]);
      QObject::connect(amounts[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
        aMix->set(static_cast<Additive::Value>(i), val);
      });
    }
  }

  // heading solids
  QLabel* txtSolid = new QLabel("<b>" + tr("Feststoffe") + "</b>", this);
  layout->addWidget(txtSolid, row, 0, 1, 3, Qt::AlignLeft);
  QLabel* txtG = new QLabel("<b>g</b>", this);
  txtG->setMaximumHeight(20);
  layout->addWidget(txtG, row, 3, Qt::AlignRight);
  row++;

  // solids
  for (int i = static_cast<int>(Additive::Value::lastLiquid) + 1; i < static_cast<int>(Additive::Value::Size); i++) {
    Additive::Value what = static_cast<Additive::Value>(i);
    if (aMix->isEnabled(what)) {
      amounts[i] = new QDoubleSpinBox(this);
      amounts[i]->setValue(aMix->get(what));
      addSolid(row++, Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)],
               Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)], amounts[i]);
      QObject::connect(amounts[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
        aMix->set(static_cast<Additive::Value>(i), val);
      });
    }
  }

  // Add expanding widget
  layout->addWidget(new QWidget(this), row, 0);
  layout->setRowStretch(row, 10);
}

void MixAdditiveWidget::mergeAdditive() {
  // it is merged into the aMix but use percentages from aDb
  // merge cases (Enabeld state)
  // | aDb | aMix | Action                 |
  // |---: | ---: | :--------------------- |
  // |   0 |    0 | Nothng to do           |
  // |   0 |    1 | Nothng to do           |
  // |   1 |    0 | Enable in aMix         |
  // |   1 |    1 | Convert concentrations |

  // merge liquids
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    Additive::Value what = static_cast<Additive::Value>(i);
    if (aDb->isEnabled(what) && !aMix->isEnabled(what)) {
      aMix->enable(what, true);
      aMix->setConcentration(what, aDb->getConcentration(what));
      aMix->set(what, 0);
    }
    if (aDb->isEnabled(what) && aMix->isEnabled(what) && aMix->getConcentration(what) != 0) {
      float amount = aMix->get(what) / aMix->getConcentration(what) * aDb->getConcentration(what);
      aMix->setConcentration(what, aDb->getConcentration(what));
      aMix->set(what, amount);
    }
  }
  // merge solids
  for (int i = static_cast<int>(Additive::Value::lastLiquid) + 1; i < static_cast<int>(Additive::Value::Size); i++) {
    // just need to add enble flag
    Additive::Value what = static_cast<Additive::Value>(i);
    aMix->enable(what, (aMix->isEnabled(what) || aDb->isEnabled(what)));
  }
}

void MixAdditiveWidget::addAcid(int row, QString formula, QString text, QString percent, QDoubleSpinBox* ml) {
  layout->addWidget(new QLabel(formula, this), row, 0, Qt::AlignLeft);
  layout->addWidget(new QLabel(text, this), row, 1, Qt::AlignLeft);
  layout->addWidget(new QLabel(percent, this), row, 2, Qt::AlignRight);
  ml->setDecimals(1);
  ml->setMinimum(0);
  ml->setMaximum(9999);
  layout->addWidget(ml, row, 3, Qt::AlignRight);
}

void MixAdditiveWidget::addSolid(int row, QString formula, QString text, QDoubleSpinBox* g) {
  layout->addWidget(new QLabel(formula, this), row, 0, Qt::AlignLeft);
  layout->addWidget(new QLabel(text, this), row, 1, Qt::AlignLeft);
  g->setDecimals(1);
  g->setMinimum(0);
  g->setMaximum(9999);
  layout->addWidget(g, row, 3, Qt::AlignRight);
}
