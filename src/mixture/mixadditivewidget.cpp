// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixadditivewidget.h"

MixAdditiveWidget::MixAdditiveWidget(Additive* mixtureAdditive, AdditiveSettings& additiveCfg, QWidget* parent)
    : QFrame{parent}, aMix(mixtureAdditive), aCfg(additiveCfg) {
  auto* layout = new QGridLayout(this);
  this->setLayout(layout);
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // create ui elements
  for (auto& p : percents) {
    p = new QLabel("", this);
  }
  for (auto& a : amounts) {
    a = new QDoubleSpinBox(this);
    a->setDecimals(1);
    a->setMinimum(0);
    a->setMaximum(9999);  // NOLINT(*-magic-numbers)
  }
  // connect
  for (uint i = 0; i < amounts.size(); i++) {
    QObject::connect(amounts.at(i), &QDoubleSpinBox::valueChanged, this, [this, i](double val) {
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
  for (uint i = 0; i < percents.size(); i++) {
    auto* formula = new QLabel(Additive::strFormula.at(i), this);
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    auto* txt = new QLabel(Additive::strTranslate.at(i), this);
    layout->addWidget(txt, row, 1, Qt::AlignLeft);
    layout->addWidget(percents.at(i), row, 2, Qt::AlignRight);
    layout->addWidget(amounts.at(i), row, 3, Qt::AlignRight);
    row++;
  }

  // heading solids
  layout->addWidget(new QLabel("<b>" + tr("Feststoffe") + "</b>", this), row, 0, 1, 3, Qt::AlignLeft);
  layout->addWidget(new QLabel("<b>g</b>", this), row, 3, Qt::AlignRight);
  row++;

  // solids
  for (uint i = percents.size(); i < amounts.size(); i++) {
    auto* formula = new QLabel(Additive::strFormula.at(i), this);
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    auto* txt = new QLabel(Additive::strTranslate.at(i), this);
    // layout->addWidget(txt, row, 1, 1, 2, Qt::AlignLeft);
    layout->addWidget(txt, row, 1, Qt::AlignLeft);
    layout->addWidget(amounts.at(i), row, 3, Qt::AlignRight);
    row++;
  }

  // Add expanding space
  layout->setRowStretch(row, 10);  // NOLINT(*-magic-numbers)

  update();
  QObject::connect(&aCfg, &AdditiveSettings::dataModified, this, &MixAdditiveWidget::update);
}

void MixAdditiveWidget::update() {
  valChangeGuard = true;  // Disable valueChanges during ui only update
  // update values
  for (uint i = 0; i < amounts.size(); i++) {
    auto what = static_cast<Additive::Value>(i);
    if (i < percents.size()) {
      const double display = aMix->get(what) * 100 / aCfg.getConcentration(what) / aCfg.getDensity(what);
      percents.at(i)->setText(QString::number(aCfg.getConcentration(what), 'f', 0));
      amounts.at(i)->setValue(display);
    } else {
      amounts.at(i)->setValue(aMix->get(what));
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

void MixAdditiveWidget::valueChange(uint idx, double val) {
  if (!valChangeGuard) {
    auto what = static_cast<Additive::Value>(idx);
    const double newVal = val / 100 * aCfg.getConcentration(what) * aCfg.getDensity(what);
    aMix->set(what, newVal);
  }
}
