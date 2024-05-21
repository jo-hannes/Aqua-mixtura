// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivewindow.h"

#include "../common/buttons.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

AdditiveWindow::AdditiveWindow(Additive* model, QWidget* parent) : QWidget{parent} {
  this->additive = model;

  layout = new QGridLayout();
  int row = 0;

  setWindowTitle("Aqua mixtura - " + tr("Zusatzstoffe"));

  // heading liquids
  QLabel* txtAcids = new QLabel("<b>" + tr("Säuren") + "</b>");
  layout->addWidget(txtAcids, row, 0, 1, 2, Qt::AlignLeft);
  QLabel* txtPercent = new QLabel("<b>%</b>");
  layout->addWidget(txtPercent, row, 2, Qt::AlignRight);
  QLabel* txtMl = new QLabel("<b>mL</b>");
  layout->addWidget(txtMl, row, 3, Qt::AlignRight);
  row++;

  // liquids
  for (int i = 0; i <= static_cast<int>(Additive::Value::lastLiquid); i++) {
    enabled[i] = new QCheckBox(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)]);
    concentrations[i] = new QDoubleSpinBox();
    amounts[i] = new QDoubleSpinBox();
    addAcid(row++, enabled[i], Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)],
            concentrations[i], amounts[i]);
    QObject::connect(enabled[i], &QCheckBox::stateChanged, this, [=](int checked) {
      bool en = checked == Qt::Checked;
      additive->enable(static_cast<Additive::Value>(i), en);
      amounts[i]->setEnabled(en);
      concentrations[i]->setEnabled(en);
    });
    QObject::connect(amounts[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      additive->set(static_cast<Additive::Value>(i), val);
    });
    QObject::connect(concentrations[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      additive->setConcentration(static_cast<Additive::Value>(i), val);
    });
  }

  // heading solids
  QLabel* txtSolid = new QLabel("<b>" + tr("Feststoffe") + "</b>");
  layout->addWidget(txtSolid, row, 0, 1, 3, Qt::AlignLeft);
  QLabel* txtG = new QLabel("<b>g</b>");
  txtG->setMaximumHeight(20);
  layout->addWidget(txtG, row, 3, Qt::AlignRight);
  row++;

  // solids
  for (int i = static_cast<int>(Additive::Value::lastLiquid) + 1; i < static_cast<int>(Additive::Value::Size); i++) {
    enabled[i] = new QCheckBox(Additive::strings[i][static_cast<uint>(Additive::StringIdx::Formula)]);
    amounts[i] = new QDoubleSpinBox();
    addSolid(row++, enabled[i], Additive::strings[i][static_cast<uint>(Additive::StringIdx::Description)], amounts[i]);
    QObject::connect(enabled[i], &QCheckBox::stateChanged, this, [=](int checked) {
      bool en = checked == Qt::Checked;
      additive->enable(static_cast<Additive::Value>(i), en);
      amounts[i]->setEnabled(en);
    });
    QObject::connect(amounts[i], &QDoubleSpinBox::valueChanged, this, [=](double val) {
      additive->set(static_cast<Additive::Value>(i), val);
    });
  }

  // buttons
  Buttons* buttons = new Buttons(tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnSave, &QPushButton::clicked, additive, &Additive::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &AdditiveWindow::cancel);
  layout->addWidget(buttons, row++, 0, 1, -1, Qt::AlignHCenter);

  this->setLayout(layout);

  update();
}

void AdditiveWindow::cancel() {
  additive->load();
  update();
}

void AdditiveWindow::update() {
  for (int i = 0; i < static_cast<int>(Additive::Value::Size); i++) {
    bool en = additive->isEnabled(static_cast<Additive::Value>(i));
    // qDebug() << "i: " << i << "en: " << en;
    enabled[i]->setCheckState(en ? Qt::Checked : Qt::Unchecked);
    amounts[i]->setValue(additive->get(static_cast<Additive::Value>(i)));
    amounts[i]->setEnabled(en);
    if (i <= static_cast<int>(Additive::Value::lastLiquid)) {
      concentrations[i]->setValue(additive->getConcentration(static_cast<Additive::Value>(i)));
      concentrations[i]->setEnabled(en);
    }
  }
}

void AdditiveWindow::addAcid(int row, QCheckBox* check, QString text, QDoubleSpinBox* percent, QDoubleSpinBox* ml) {
  layout->addWidget(check, row, 0, Qt::AlignLeft);
  QLabel* txt = new QLabel(text);
  layout->addWidget(txt, row, 1, Qt::AlignLeft);
  percent->setDecimals(0);
  percent->setMinimum(0);
  percent->setMaximum(100);
  layout->addWidget(percent, row, 2, Qt::AlignRight);
  ml->setDecimals(1);
  ml->setMinimum(0);
  ml->setMaximum(9999);
  layout->addWidget(ml, row, 3, Qt::AlignRight);
}

void AdditiveWindow::addSolid(int row, QCheckBox* check, QString text, QDoubleSpinBox* g) {
  layout->addWidget(check, row, 0, Qt::AlignLeft);
  QLabel* txt = new QLabel(text);
  layout->addWidget(txt, row, 1, 1, 2, Qt::AlignLeft);
  g->setDecimals(1);
  g->setMinimum(0);
  g->setMaximum(9999);
  layout->addWidget(g, row, 3, Qt::AlignRight);
}
