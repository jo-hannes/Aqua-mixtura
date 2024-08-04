// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivewindow.h"

#include "../common/buttons.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

AdditiveWindow::AdditiveWindow(AdditiveSettings& model, QWidget* parent) : QWidget{parent}, additive(model) {
  int row = 0;
  QGridLayout* layout = new QGridLayout();

  setWindowTitle("Aqua mixtura - " + tr("Zusatzstoffe"));

  // heading liquids
  QLabel* txtAcids = new QLabel("<b>" + tr("Säuren") + "</b>");
  layout->addWidget(txtAcids, row, 0, 1, 2, Qt::AlignLeft);
  QLabel* txtPercent = new QLabel("<b>%</b>");
  layout->addWidget(txtPercent, row, 1, Qt::AlignRight);
  row++;

  // liquid concentrations
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    QLabel* txt = new QLabel(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)]);
    layout->addWidget(txt, row, 0, Qt::AlignLeft);
    concentrations[i] = new QDoubleSpinBox();
    concentrations[i]->setDecimals(0);
    concentrations[i]->setMinimum(1);
    concentrations[i]->setMaximum(100);
    layout->addWidget(concentrations[i], row, 1, Qt::AlignRight);

    QObject::connect(concentrations[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      additive.setConcentration(static_cast<Additive::Value>(i), val);
    });
    row++;
  }

  // unit
  QLabel* txtUnit = new QLabel("<b>" + tr("Einheit") + "</b>");
  layout->addWidget(txtUnit, row, 0, Qt::AlignLeft);
  unitSelect = new QComboBox();
  unitSelect->clear();
  unitSelect->addItem("g");
  unitSelect->addItem("ml");
  layout->addWidget(unitSelect, row, 1, Qt::AlignRight);
  QObject::connect(unitSelect, &QComboBox::activated, this, &AdditiveWindow::selectUnit);
  row++;

  // buttons
  Buttons* buttons = new Buttons(tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnSave, &QPushButton::clicked, &additive, &AdditiveSettings::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &AdditiveWindow::cancel);
  layout->addWidget(buttons, row++, 0, 1, -1, Qt::AlignHCenter);

  this->setLayout(layout);

  update();
}

void AdditiveWindow::cancel() {
  additive.load();
  update();
}

void AdditiveWindow::selectUnit(int index) {
  if (index == 1) {
    additive.setLiquidUnit(AdditiveSettings::LiquidUnit::milliLiter);
  } else {
    additive.setLiquidUnit(AdditiveSettings::LiquidUnit::gramm);
  }
}

void AdditiveWindow::update() {
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    concentrations[i]->setValue(additive.getConcentration(static_cast<Additive::Value>(i)));
  }
  if (additive.getLiquidUnit() == AdditiveSettings::LiquidUnit::milliLiter) {
    unitSelect->setCurrentIndex(1);
  } else {
    unitSelect->setCurrentIndex(0);
  }
}
