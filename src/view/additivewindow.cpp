// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivewindow.h"

#include <QCheckBox>
#include <QLabel>
#include <QPushButton>

AdditiveWindow::AdditiveWindow(MainModel* model, QWidget* parent) : QWidget{parent} {
  this->model = model;

  layout = new QGridLayout();
  int row = 1;

  // Überschrift1
  QLabel* txtAcids = new QLabel("<b>" + tr("Säuren") + "</b>");
  layout->addWidget(txtAcids, row, 0, 1, 2, Qt::AlignLeft);
  QLabel* txtPercent = new QLabel("<b>%</b>");
  layout->addWidget(txtPercent, row, 2, Qt::AlignRight);
  QLabel* txtMl = new QLabel("<b>mL</b>");
  txtMl->setMaximumHeight(20);  // Limit heigh of headline. TODO: This is only a workaround
  layout->addWidget(txtMl, row, 3, Qt::AlignRight);
  row++;

  c3h6o3Volume = new QDoubleSpinBox();
  c3h6o3Concentration = new QDoubleSpinBox();
  addAcid(row++, c3h6o3Concentration, c3h6o3Volume, "C₃H₆O₃", tr("Milchsäure"));

  hclVolume = new QDoubleSpinBox();
  hclConcentration = new QDoubleSpinBox();
  addAcid(row++, hclConcentration, hclVolume, "HCl", tr("Salzsäure"));

  h2so4Volume = new QDoubleSpinBox();
  h2so4Concentration = new QDoubleSpinBox();
  addAcid(row++, h2so4Concentration, h2so4Volume, "H₂SO₄", tr("Schwefelsäure"));

  h3po4Volume = new QDoubleSpinBox();
  h3po4Concentration = new QDoubleSpinBox();
  addAcid(row++, h3po4Concentration, h3po4Volume, "H₃PO₄", tr("Phosphorsäure"));

  QLabel* txtSolid = new QLabel("<b>" + tr("Feststoffe") + "</b>");
  layout->addWidget(txtSolid, row, 0, 1, 3, Qt::AlignLeft);
  QLabel* txtG = new QLabel("<b>g</b>");
  txtG->setMaximumHeight(20);
  layout->addWidget(txtG, row, 3, Qt::AlignRight);
  row++;

  caso4 = new QDoubleSpinBox();
  addSolid(row++, caso4, "CaSO₄", tr("Calciumsulfat (Braugips)"));

  cacl2 = new QDoubleSpinBox();
  addSolid(row++, cacl2, "CaCl₂", tr("Calciumchlorid"));

  nacl = new QDoubleSpinBox();
  addSolid(row++, nacl, "NaCl", tr("Natriumchlorid (Kochsalz)"));

  nahco3 = new QDoubleSpinBox();
  addSolid(row++, nahco3, "NaHCO₃", tr("Natriumhydrogencarbonat (Natron)"));

  mgcl2 = new QDoubleSpinBox();
  addSolid(row++, mgcl2, "MgCl₂", tr("Magnesiumchlorid"));

  mgso4 = new QDoubleSpinBox();
  addSolid(row++, mgso4, "MgSO₄", tr("Magnesiumsulfat"));

  caco3 = new QDoubleSpinBox();
  addSolid(row++, caco3, "CaCO₃", tr("Calciumcarbonat"));

  QLabel* spacer = new QLabel();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer, row++, 0, 1, -1, Qt::AlignCenter);

  this->setLayout(layout);
}

void AdditiveWindow::save() {
  qDebug() << "TODO AdditiveWindow::save";
}

void AdditiveWindow::cancel() {
  qDebug() << "TODO AdditiveWindow::cancel";
}

void AdditiveWindow::addAcid(int row, QDoubleSpinBox* percent, QDoubleSpinBox* ml, QString formula, QString text) {
  QCheckBox* box = new QCheckBox(formula);
  layout->addWidget(box, row, 0, Qt::AlignLeft);
  QLabel* txt = new QLabel(text);
  qDebug() << txt->size();
  layout->addWidget(txt, row, 1, Qt::AlignLeft);
  percent->setDecimals(0);
  percent->setMinimum(0);
  percent->setMaximum(100);
  layout->addWidget(percent, row, 2, Qt::AlignRight);
  ml->setDecimals(1);
  ml->setMinimum(0);
  layout->addWidget(ml, row, 3, Qt::AlignRight);
}

void AdditiveWindow::addSolid(int row, QDoubleSpinBox* g, QString formula, QString text) {
  QCheckBox* box = new QCheckBox(formula);
  layout->addWidget(box, row, 0, Qt::AlignLeft);
  QLabel* txt = new QLabel(text);
  layout->addWidget(txt, row, 1, 1, 2, Qt::AlignLeft);
  g->setDecimals(1);
  g->setMinimum(0);
  layout->addWidget(g, row, 3, Qt::AlignRight);
}
