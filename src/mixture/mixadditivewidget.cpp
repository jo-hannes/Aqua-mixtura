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
  for (auto& s : lStrike) {
    s = new QLabel();
  }
  for (auto& s : lSparging) {
    s = new QLabel();
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
  row++;

  // liquids
  for (uint i = 0; i < percents.size(); i++) {
    auto* formula = new QLabel(Additive::strFormula.at(i), this);
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    auto* txt = new QLabel(Additive::strTranslate.at(i), this);
    layout->addWidget(txt, row, 1, Qt::AlignLeft);
    layout->addWidget(percents.at(i), row, 2, Qt::AlignLeft);
    layout->addWidget(amounts.at(i), row, 3, Qt::AlignRight);
    layout->addWidget(lStrike.at(i), row, 4, Qt::AlignRight);
    layout->addWidget(lSparging.at(i), row, 5, Qt::AlignRight);
    row++;
  }

  // heading solids
  layout->addWidget(new QLabel("<b>" + tr("Feststoffe") + "</b>", this), row, 0, 1, 3, Qt::AlignLeft);
  row++;

  // solids
  for (uint i = percents.size(); i < amounts.size(); i++) {
    amounts.at(i)->setSuffix("g");  // Solids will always hafe g as suffix
    auto* formula = new QLabel(Additive::strFormula.at(i), this);
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    auto* txt = new QLabel(Additive::strTranslate.at(i), this);
    layout->addWidget(txt, row, 1, 1, 2, Qt::AlignLeft);
    // layout->addWidget(txt, row, 1, Qt::AlignLeft);
    layout->addWidget(amounts.at(i), row, 3, Qt::AlignRight);
    layout->addWidget(lStrike.at(i), row, 4, Qt::AlignRight);
    layout->addWidget(lSparging.at(i), row, 5, Qt::AlignRight);
    row++;
  }

  // Add expanding space
  layout->setRowStretch(row, 10);  // NOLINT(*-magic-numbers)

  update();
  QObject::connect(&aCfg, &AdditiveSettings::dataModified, this, &MixAdditiveWidget::update);
}

void MixAdditiveWidget::setTotalWater(double volume) {
  if (total != 0) {
    const double factor = volume / total;
    for (uint i = 0; i < amounts.size(); i++) {
      auto what = static_cast<Additive::Value>(i);
      aMix->set(what, aMix->get(what) * factor);
    }
  }
  total = volume;
  update();
}

void MixAdditiveWidget::setStrikeWater(double volume) {
  strike = volume;
  update();
}

void MixAdditiveWidget::setSpargingWater(double volume) {
  sparging = volume;
  update();
}

void MixAdditiveWidget::update() {
  valChangeGuard = true;  // Disable valueChanges during ui only update
  // first get liquid unit
  QString lu;
  if (aCfg.getLiquidUnit() == AdditiveSettings::LiquidUnit::milliLiter) {
    lu = "mL";
  } else {
    lu = "g";
  }
  // update values
  for (uint i = 0; i < amounts.size(); i++) {
    auto what = static_cast<Additive::Value>(i);
    const double display = aMix->get(what) * 100 / aCfg.getConcentration(what) / aCfg.getDensity(what);
    const double dStrike = display / total * strike;
    const double dSparging = display / total * sparging;
    amounts.at(i)->setValue(display);
    if (i < percents.size()) {
      percents.at(i)->setText(QString::number(aCfg.getConcentration(what), 'f', 0) + "%");
      amounts.at(i)->setSuffix(lu);
      lStrike.at(i)->setText(QString::number(dStrike, 'f', 1) + lu);
      lSparging.at(i)->setText(QString::number(dSparging, 'f', 1) + lu);
    } else {
      lStrike.at(i)->setText(QString::number(dStrike, 'f', 1) + "g");
      lSparging.at(i)->setText(QString::number(dSparging, 'f', 1) + "g");
    }
  }
  valChangeGuard = false;
}

void MixAdditiveWidget::valueChange(uint idx, double val) {
  if (!valChangeGuard) {
    auto what = static_cast<Additive::Value>(idx);
    const double newVal = val / 100 * aCfg.getConcentration(what) * aCfg.getDensity(what);
    const double dStrike = val / total * strike;
    const double dSparging = val / total * sparging;
    aMix->set(what, newVal);
    // update strike and sparging
    QString unit = "g";
    if (idx < percents.size() && aCfg.getLiquidUnit() == AdditiveSettings::LiquidUnit::milliLiter) {
      unit = "mL";
    }
    lStrike.at(idx)->setText(QString::number(dStrike, 'f', 1) + unit);
    lSparging.at(idx)->setText(QString::number(dSparging, 'f', 1) + unit);
  }
}
