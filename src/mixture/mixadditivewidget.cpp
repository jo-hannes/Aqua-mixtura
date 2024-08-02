// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixadditivewidget.h"

MixAdditiveWidget::MixAdditiveWidget(Additive* mixtureAdditive, AdditiveSettings& additiveCfg, QWidget* parent)
    : QFrame{parent}, aCfg(additiveCfg) {
  aMix = mixtureAdditive;

  QGridLayout* layout;
  layout = new QGridLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // create ui elements
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      percents[i] = new QLabel("", this);
    }
    amounts[i] = new QDoubleSpinBox(this);
    amounts[i]->setDecimals(1);
    amounts[i]->setMinimum(0);
    amounts[i]->setMaximum(9999);
    QObject::connect(amounts[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      valueChange(i, val);
    });
  }

  int row = 0;
  // heading
  layout->addWidget(new QLabel(tr("Zusatzstoffe"), this), row++, 0, 1, 3, Qt::AlignLeft);

  // heading liquids
  layout->addWidget(new QLabel("<b>" + tr("Säuren") + "</b>", this), row, 0, 1, 2, Qt::AlignLeft);
  layout->addWidget(new QLabel("<b>%</b>", this), row, 2, Qt::AlignRight);
  liquidUnit = new QLabel(this);
  layout->addWidget(liquidUnit, row, 3, Qt::AlignRight);
  row++;

  // liquids
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    QLabel* formula = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)], this);
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    QLabel* txt = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)], this);
    layout->addWidget(txt, row, 1, Qt::AlignLeft);
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
    QLabel* formula = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)], this);
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    QLabel* txt = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)], this);
    // layout->addWidget(txt, row, 1, 1, 2, Qt::AlignLeft);
    layout->addWidget(txt, row, 1, Qt::AlignLeft);
    layout->addWidget(amounts[i], row, 3, Qt::AlignRight);
    row++;
  }

  // Add expanding space
  layout->setRowStretch(row, 10);

  update();
  QObject::connect(&aCfg, &AdditiveSettings::dataModified, this, &MixAdditiveWidget::update);
}

void MixAdditiveWidget::update() {
  valChangeGuard = true;  // Disable valueChanges during ui only update
  // update values
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    Additive::Value what = static_cast<Additive::Value>(i);
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      double display = aMix->get(what) * 100 / aCfg.getConcentration(what) / aCfg.getDensity(what);
      percents[i]->setText(QString::number(aCfg.getConcentration(what), 'f', 0));
      amounts[i]->setValue(display);
    } else {
      amounts[i]->setValue(aMix->get(what));
    }
  }
  // update unit
  if (aCfg.getLiquidUnit() == AdditiveSettings::LiquidUnit::milliLiter) {
    liquidUnit->setText("<b>mL</b>");
  } else {
    liquidUnit->setText("<b>g</b>");
  }
  valChangeGuard = false;
}

void MixAdditiveWidget::valueChange(int idx, double val) {
  if (!valChangeGuard) {
    Additive::Value what = static_cast<Additive::Value>(idx);
    double newVal = val / 100 * aCfg.getConcentration(what) * aCfg.getDensity(what);
    aMix->set(what, newVal);
  }
}