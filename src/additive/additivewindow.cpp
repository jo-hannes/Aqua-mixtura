// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivewindow.h"

#include "../common/buttons.h"
#include "../common/dialogs.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

AdditiveWindow::AdditiveWindow(AdditiveSettings& model, QWidget* parent) : QWidget{parent}, additive(model) {
  int row = 0;
  auto* layout = new QGridLayout();

  setWindowTitle("Aqua-mixtura - " + tr("Zusatzstoffe"));

  // heading liquids
  auto* txtAcids = new QLabel("<b>" + tr("Säuren") + "</b>");
  layout->addWidget(txtAcids, row, 0, 1, 3, Qt::AlignLeft);
  auto* txtPercent = new QLabel("<b>%</b>");
  layout->addWidget(txtPercent, row, 2, Qt::AlignRight);
  row++;

  // liquid concentrations
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    auto* formula = new QLabel(Additive::strFormula.at(i));
    layout->addWidget(formula, row, 0, Qt::AlignLeft);
    auto* txt = new QLabel(Additive::strTranslate.at(i));
    layout->addWidget(txt, row, 1, Qt::AlignLeft);
    concentrations[i] = new QDoubleSpinBox();
    concentrations[i]->setDecimals(0);
    concentrations[i]->setMinimum(1);
    concentrations[i]->setMaximum(100);  // NOLINT(*-magic-numbers)
    concentrations[i]->setValue(additive.getConcentration(static_cast<Additive::Value>(i)));
    layout->addWidget(concentrations[i], row, 2, Qt::AlignRight);

    QObject::connect(concentrations[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      additive.setConcentration(static_cast<Additive::Value>(i), val);
    });
    row++;
  }

  // unit
  auto* txtUnit = new QLabel("<b>" + tr("Einheit") + "</b>");
  layout->addWidget(txtUnit, row, 0, 1, 2, Qt::AlignLeft);
  unitSelect = new QComboBox();
  unitSelect->clear();
  unitSelect->addItem("g");
  unitSelect->addItem("ml");
  layout->addWidget(unitSelect, row, 2, Qt::AlignRight);
  QObject::connect(unitSelect, &QComboBox::activated, this, &AdditiveWindow::selectUnit);
  row++;

  // buttons
  auto* buttons = new Buttons(tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnSave, &QPushButton::clicked, &additive, &AdditiveSettings::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &AdditiveWindow::cancel);
  layout->addWidget(buttons, row++, 0, 1, -1, Qt::AlignHCenter);

  this->setLayout(layout);

  update();
}

void AdditiveWindow::closeEvent(QCloseEvent* event) {
  if (additive.isChanged()) {
    const int ret = Dialogs::saveChanges(tr("Änderungen speichern?"), tr("Additive haben ungespeicherte Änderungen"));
    switch (ret) {
      case QMessageBox::Save:
        additive.save();  // save and close window
        break;
      case QMessageBox::Discard:
        cancel();  // undo changes and close window
        break;
      case QMessageBox::Cancel:
      default:
        event->ignore();  // ignore event to keep window open
        return;
        break;
    }
  }
  event->accept();
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
