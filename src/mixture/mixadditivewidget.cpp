// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixadditivewidget.h"

MixAdditiveWidget::MixAdditiveWidget(Additive* mixtureAdditive, Additive* additiveDb, QWidget* parent)
    : QFrame{parent} {
  aMix = mixtureAdditive;
  aDb = additiveDb;
  mergeAdditive();

  layout = new QGridLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // create ui elements
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    formulas[i] = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)], this);
    texts[i] = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)], this);
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      percents[i] = new QLabel("", this);
    }
    amounts[i] = new QDoubleSpinBox(this);
    amounts[i]->setDecimals(1);
    amounts[i]->setMinimum(0);
    amounts[i]->setMaximum(9999);
    QObject::connect(amounts[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      aMix->set(static_cast<Additive::Value>(i), val);
    });
  }

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
    layout->addWidget(formulas[i], row, 0, Qt::AlignLeft);
    layout->addWidget(texts[i], row, 1, Qt::AlignLeft);
    layout->addWidget(percents[i], row, 2, Qt::AlignRight);
    layout->addWidget(amounts[i], row, 3, Qt::AlignRight);
    row++;
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
    layout->addWidget(formulas[i], row, 0, Qt::AlignLeft);
    // layout->addWidget(texts[i], row, 1, 1, 2, Qt::AlignLeft);
    layout->addWidget(texts[i], row, 1, Qt::AlignLeft);
    layout->addWidget(amounts[i], row, 3, Qt::AlignRight);
    row++;
  }

  // Add expanding space
  layout->setRowStretch(row, 10);

  update();
}

void MixAdditiveWidget::update() {
  mergeAdditive();
  // update values and show/hide additive
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    Additive::Value what = static_cast<Additive::Value>(i);
    bool show = aMix->isEnabled(what);
    formulas[i]->setVisible(show);
    texts[i]->setVisible(show);
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      percents[i]->setText(QString::number(aMix->getConcentration(what), 'f', 0));
      percents[i]->setVisible(show);
    }
    amounts[i]->setValue(aMix->get(what));
    amounts[i]->setVisible(show);
  }
}

void MixAdditiveWidget::mergeAdditive() {
  // it is merged into the aMix but use percentages from aDb
  // merge cases (Enabeld state)
  // | aDb | aMix | Action                  |
  // |---: | ---: | :---------------------- |
  // |   0 |    0 | Nothng to do            |
  // |   0 |    1 | Check if amount is zero |
  // |   1 |    0 | Enable in aMix          |
  // |   1 |    1 | Convert concentrations  |

  // merge liquids
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    Additive::Value what = static_cast<Additive::Value>(i);
    if (!aDb->isEnabled(what) && aMix->isEnabled((what)) && aMix->get(what) == 0) {
      // Disable additive with zero amount if not also in database
      aMix->enable(what, false);
    }
    if (aDb->isEnabled(what) && !aMix->isEnabled(what)) {
      aMix->enable(what, true);
      aMix->setConcentration(what, aDb->getConcentration(what));
      aMix->set(what, 0);
    }
    if (aDb->isEnabled(what) && aMix->isEnabled(what) && aMix->getConcentration(what) != 0) {
      float amount = aMix->get(what) * aMix->getConcentration(what) / aDb->getConcentration(what);
      aMix->setConcentration(what, aDb->getConcentration(what));
      aMix->set(what, amount);
    }
  }
  // merge solids
  for (int i = static_cast<int>(Additive::Value::lastLiquid) + 1; i < static_cast<int>(Additive::Value::Size); i++) {
    // just need to add enble flag
    Additive::Value what = static_cast<Additive::Value>(i);
    aMix->enable(what, ((aMix->isEnabled(what) && (aMix->get(what) != 0)) || aDb->isEnabled(what)));
  }
}
