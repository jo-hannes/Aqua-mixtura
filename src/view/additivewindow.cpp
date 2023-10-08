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
  c3h6o3Check = new QCheckBox("C₃H₆O₃");
  addAcid(row++, c3h6o3Check, tr("Milchsäure"), c3h6o3Concentration, c3h6o3Volume);

  hclVolume = new QDoubleSpinBox();
  hclConcentration = new QDoubleSpinBox();
  hclCheck = new QCheckBox("HCl");
  addAcid(row++, hclCheck, tr("Salzsäure"), hclConcentration, hclVolume);

  h2so4Volume = new QDoubleSpinBox();
  h2so4Concentration = new QDoubleSpinBox();
  h2so4Check = new QCheckBox("H₂SO₄");
  addAcid(row++, h2so4Check, tr("Schwefelsäure"), h2so4Concentration, h2so4Volume);

  h3po4Volume = new QDoubleSpinBox();
  h3po4Concentration = new QDoubleSpinBox();
  h3po4Check = new QCheckBox("H₃PO₄");
  addAcid(row++, h3po4Check, tr("Phosphorsäure"), h3po4Concentration, h3po4Volume);

  QLabel* txtSolid = new QLabel("<b>" + tr("Feststoffe") + "</b>");
  layout->addWidget(txtSolid, row, 0, 1, 3, Qt::AlignLeft);
  QLabel* txtG = new QLabel("<b>g</b>");
  txtG->setMaximumHeight(20);
  layout->addWidget(txtG, row, 3, Qt::AlignRight);
  row++;

  caso4 = new QDoubleSpinBox();
  caso4Check = new QCheckBox("CaSO₄");
  addSolid(row++, caso4Check, tr("Calciumsulfat (Braugips)"), caso4);

  cacl2 = new QDoubleSpinBox();
  cacl2Check = new QCheckBox("CaCl₂");
  addSolid(row++, cacl2Check, tr("Calciumchlorid"), cacl2);

  nacl = new QDoubleSpinBox();
  naclCheck = new QCheckBox("NaCl");
  addSolid(row++, naclCheck, tr("Natriumchlorid (Kochsalz)"), nacl);

  nahco3 = new QDoubleSpinBox();
  nahco3Check = new QCheckBox("NaHCO₃");
  addSolid(row++, nahco3Check, tr("Natriumhydrogencarbonat (Natron)"), nahco3);

  mgcl2 = new QDoubleSpinBox();
  mgcl2Check = new QCheckBox("MgCl₂");
  addSolid(row++, mgcl2Check, tr("Magnesiumchlorid"), mgcl2);

  mgso4 = new QDoubleSpinBox();
  mgso4Check = new QCheckBox("MgSO₄");
  addSolid(row++, mgso4Check, tr("Magnesiumsulfat"), mgso4);

  caco3 = new QDoubleSpinBox();
  caco3Check = new QCheckBox("CaCO₃");
  addSolid(row++, caco3Check, tr("Calciumcarbonat"), caco3);

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
  layout->addWidget(ml, row, 3, Qt::AlignRight);
}

void AdditiveWindow::addSolid(int row, QCheckBox* check, QString text, QDoubleSpinBox* g) {
  layout->addWidget(check, row, 0, Qt::AlignLeft);
  QLabel* txt = new QLabel(text);
  layout->addWidget(txt, row, 1, 1, 2, Qt::AlignLeft);
  g->setDecimals(1);
  g->setMinimum(0);
  layout->addWidget(g, row, 3, Qt::AlignRight);
}
