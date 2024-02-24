// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "additivewindow.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>

AdditiveWindow::AdditiveWindow(MainModel* model, QWidget* parent) : QWidget{parent} {
  this->model = model;
  tmpData = *model->additive;

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

  c3h6o3Check = new QCheckBox("C₃H₆O₃");
  c3h6o3Concentration = new QDoubleSpinBox();
  c3h6o3Volume = new QDoubleSpinBox();
  addAcid(row++, c3h6o3Check, tr("Milchsäure"), c3h6o3Concentration, c3h6o3Volume);
  QObject::connect(c3h6o3Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeC3h6o3Checked);
  QObject::connect(c3h6o3Concentration, &QDoubleSpinBox::valueChanged, this,
                   &AdditiveWindow::valChangeC3h6o3Concentration);
  QObject::connect(c3h6o3Volume, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeC3h6o3Volume);

  hclCheck = new QCheckBox("HCl");
  hclConcentration = new QDoubleSpinBox();
  hclVolume = new QDoubleSpinBox();
  addAcid(row++, hclCheck, tr("Salzsäure"), hclConcentration, hclVolume);
  QObject::connect(hclCheck, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeHclChecked);
  QObject::connect(hclConcentration, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeHclConcentration);
  QObject::connect(hclVolume, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeHclVolume);

  h2so4Check = new QCheckBox("H₂SO₄");
  h2so4Concentration = new QDoubleSpinBox();
  h2so4Volume = new QDoubleSpinBox();
  addAcid(row++, h2so4Check, tr("Schwefelsäure"), h2so4Concentration, h2so4Volume);
  QObject::connect(h2so4Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeH2so4Checked);
  QObject::connect(h2so4Concentration, &QDoubleSpinBox::valueChanged, this,
                   &AdditiveWindow::valChangeH2so4Concentration);
  QObject::connect(h2so4Volume, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeH2so4Volume);

  h3po4Check = new QCheckBox("H₃PO₄");
  h3po4Concentration = new QDoubleSpinBox();
  h3po4Volume = new QDoubleSpinBox();
  addAcid(row++, h3po4Check, tr("Phosphorsäure"), h3po4Concentration, h3po4Volume);
  QObject::connect(h3po4Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeH3po4Checked);
  QObject::connect(h3po4Concentration, &QDoubleSpinBox::valueChanged, this,
                   &AdditiveWindow::valChangeH3po4Concentration);
  QObject::connect(h3po4Volume, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeH3po4Volume);

  QLabel* txtSolid = new QLabel("<b>" + tr("Feststoffe") + "</b>");
  layout->addWidget(txtSolid, row, 0, 1, 3, Qt::AlignLeft);
  QLabel* txtG = new QLabel("<b>g</b>");
  txtG->setMaximumHeight(20);
  layout->addWidget(txtG, row, 3, Qt::AlignRight);
  row++;

  caso4Check = new QCheckBox("CaSO₄");
  caso4 = new QDoubleSpinBox();
  addSolid(row++, caso4Check, tr("Calciumsulfat (Braugips)"), caso4);
  QObject::connect(caso4Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeCaso4Checked);
  QObject::connect(caso4, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeCaso4);

  cacl2Check = new QCheckBox("CaCl₂");
  cacl2 = new QDoubleSpinBox();
  addSolid(row++, cacl2Check, tr("Calciumchlorid"), cacl2);
  QObject::connect(cacl2Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeCacl2Checked);
  QObject::connect(cacl2, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeCacl2);

  naclCheck = new QCheckBox("NaCl");
  nacl = new QDoubleSpinBox();
  addSolid(row++, naclCheck, tr("Natriumchlorid (Kochsalz)"), nacl);
  QObject::connect(naclCheck, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeNaclChecked);
  QObject::connect(nacl, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeNacl);

  nahco3Check = new QCheckBox("NaHCO₃");
  nahco3 = new QDoubleSpinBox();
  addSolid(row++, nahco3Check, tr("Natriumhydrogencarbonat (Natron)"), nahco3);
  QObject::connect(nahco3Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeNahco3Checked);
  QObject::connect(nahco3, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeNahco3);

  mgcl2Check = new QCheckBox("MgCl₂");
  mgcl2 = new QDoubleSpinBox();
  addSolid(row++, mgcl2Check, tr("Magnesiumchlorid"), mgcl2);
  QObject::connect(mgcl2Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeMgcl2Checked);
  QObject::connect(mgcl2, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeMgcl2);

  mgso4Check = new QCheckBox("MgSO₄");
  mgso4 = new QDoubleSpinBox();
  addSolid(row++, mgso4Check, tr("Magnesiumsulfat"), mgso4);
  QObject::connect(mgso4Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeMgso4Checked);
  QObject::connect(mgso4, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeMgso4);

  caco3Check = new QCheckBox("CaCO₃");
  caco3 = new QDoubleSpinBox();
  addSolid(row++, caco3Check, tr("Calciumcarbonat"), caco3);
  QObject::connect(caco3Check, &QCheckBox::stateChanged, this, &AdditiveWindow::valChangeCaco3Checked);
  QObject::connect(caco3, &QDoubleSpinBox::valueChanged, this, &AdditiveWindow::valChangeCaco3);

  // buttons
  QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
  QObject::connect(btnBox, &QDialogButtonBox::accepted, this, &AdditiveWindow::save);
  QObject::connect(btnBox, &QDialogButtonBox::rejected, this, &AdditiveWindow::cancel);
  layout->addWidget(btnBox, row++, 0, 1, -1, Qt::AlignHCenter);

  this->setLayout(layout);

  update();
}

void AdditiveWindow::update() {
  updateAcid(c3h6o3Check, c3h6o3Concentration, c3h6o3Volume, tmpData.getC3h6o3Concentration(),
             tmpData.getC3h6o3Volume());
  updateAcid(hclCheck, hclConcentration, hclVolume, tmpData.getHclConcentration(), tmpData.getHclVolume());
  updateAcid(h2so4Check, h2so4Concentration, h2so4Volume, tmpData.getH2so4Concentration(), tmpData.getH2so4Volume());
  updateAcid(h3po4Check, h3po4Concentration, h3po4Volume, tmpData.getH3po4Concentration(), tmpData.getH3po4Volume());

  updateSolid(caso4Check, caso4, tmpData.getCaso4());
  updateSolid(cacl2Check, cacl2, tmpData.getCacl2());
  updateSolid(naclCheck, nacl, tmpData.getNacl());
  updateSolid(nahco3Check, nahco3, tmpData.getNahco3());
  updateSolid(mgcl2Check, mgcl2, tmpData.getMgcl2());
  updateSolid(mgso4Check, mgso4, tmpData.getMgso4());
  updateSolid(caco3Check, caco3, tmpData.getCaco3());
}

void AdditiveWindow::save() {
  *model->additive = tmpData;
  model->saveAdditive();
}

void AdditiveWindow::cancel() {
  tmpData = *model->additive;
  update();
}

void AdditiveWindow::valChangeC3h6o3Checked(int checked) {
  tmpData.setC3h6o3Volume(updateValChecked(tmpData.getC3h6o3Volume(), checked));
  updateAcid(c3h6o3Check, c3h6o3Concentration, c3h6o3Volume, tmpData.getC3h6o3Concentration(),
             tmpData.getC3h6o3Volume());
}

void AdditiveWindow::valChangeC3h6o3Concentration(double val) {
  tmpData.setC3h6o3Concentration(val);
}

void AdditiveWindow::valChangeC3h6o3Volume(double val) {
  tmpData.setC3h6o3Volume(val);
}

void AdditiveWindow::valChangeHclChecked(int checked) {
  tmpData.setHclVolume(updateValChecked(tmpData.getHclVolume(), checked));
  updateAcid(hclCheck, hclConcentration, hclVolume, tmpData.getHclConcentration(), tmpData.getHclVolume());
}

void AdditiveWindow::valChangeHclConcentration(double val) {
  tmpData.setHclConcentration(val);
}

void AdditiveWindow::valChangeHclVolume(double val) {
  tmpData.setHclVolume(val);
}

void AdditiveWindow::valChangeH2so4Checked(int checked) {
  tmpData.setH2so4Volume(updateValChecked(tmpData.getH2so4Volume(), checked));
  updateAcid(h2so4Check, h2so4Concentration, h2so4Volume, tmpData.getH2so4Concentration(), tmpData.getH2so4Volume());
}

void AdditiveWindow::valChangeH2so4Concentration(double val) {
  tmpData.setH2so4Concentration(val);
}

void AdditiveWindow::valChangeH2so4Volume(double val) {
  tmpData.setH2so4Volume(val);
}

void AdditiveWindow::valChangeH3po4Checked(int checked) {
  tmpData.setH3po4Volume(updateValChecked(tmpData.getH3po4Volume(), checked));
  updateAcid(h3po4Check, h3po4Concentration, h3po4Volume, tmpData.getH3po4Concentration(), tmpData.getH3po4Volume());
}

void AdditiveWindow::valChangeH3po4Concentration(double val) {
  tmpData.setH3po4Concentration(val);
}

void AdditiveWindow::valChangeH3po4Volume(double val) {
  tmpData.setH3po4Volume(val);
}

void AdditiveWindow::valChangeCaso4Checked(int checked) {
  tmpData.setCaso4(updateValChecked(tmpData.getCaso4(), checked));
  updateSolid(caso4Check, caso4, tmpData.getCaso4());
}

void AdditiveWindow::valChangeCaso4(double val) {
  tmpData.setCaso4(val);
}

void AdditiveWindow::valChangeCacl2Checked(int checked) {
  tmpData.setCacl2(updateValChecked(tmpData.getCacl2(), checked));
  updateSolid(cacl2Check, cacl2, tmpData.getCacl2());
}

void AdditiveWindow::valChangeCacl2(double val) {
  tmpData.setCacl2(val);
}

void AdditiveWindow::valChangeNaclChecked(int checked) {
  tmpData.setNacl(updateValChecked(tmpData.getNacl(), checked));
  updateSolid(naclCheck, nacl, tmpData.getNacl());
}

void AdditiveWindow::valChangeNacl(double val) {
  tmpData.setNacl(val);
}

void AdditiveWindow::valChangeNahco3Checked(int checked) {
  tmpData.setNahco3(updateValChecked(tmpData.getNahco3(), checked));
  updateSolid(nahco3Check, nahco3, tmpData.getNahco3());
}

void AdditiveWindow::valChangeNahco3(double val) {
  tmpData.setNahco3(val);
}

void AdditiveWindow::valChangeMgcl2Checked(int checked) {
  tmpData.setMgcl2(updateValChecked(tmpData.getMgcl2(), checked));
  updateSolid(mgcl2Check, mgcl2, tmpData.getMgcl2());
}

void AdditiveWindow::valChangeMgcl2(double val) {
  tmpData.setMgcl2(val);
}

void AdditiveWindow::valChangeMgso4Checked(int checked) {
  tmpData.setMgso4(updateValChecked(tmpData.getMgso4(), checked));
  updateSolid(mgso4Check, mgso4, tmpData.getMgso4());
}

void AdditiveWindow::valChangeMgso4(double val) {
  tmpData.setMgso4(val);
}

void AdditiveWindow::valChangeCaco3Checked(int checked) {
  tmpData.setCaco3(updateValChecked(tmpData.getCaco3(), checked));
  updateSolid(caco3Check, caco3, tmpData.getCaco3());
}

void AdditiveWindow::valChangeCaco3(double val) {
  tmpData.setCaco3(val);
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
  layout->addWidget(g, row, 3, Qt::AlignRight);
}

void AdditiveWindow::updateAcid(QCheckBox* check, QDoubleSpinBox* spinPercent, QDoubleSpinBox* spinMl, float percent,
                                float ml) {
  if (ml < 0) {
    check->setCheckState(Qt::Unchecked);
    spinPercent->setValue(percent);
    spinPercent->setEnabled(false);
    spinMl->setValue(-ml);
    spinMl->setEnabled(false);
  } else {
    check->setCheckState(Qt::Checked);
    spinPercent->setValue(percent);
    spinPercent->setEnabled(true);
    spinMl->setValue(ml);
    spinMl->setEnabled(true);
  }
}

void AdditiveWindow::updateSolid(QCheckBox* check, QDoubleSpinBox* spinG, float g) {
  if (g < 0) {
    check->setCheckState(Qt::Unchecked);
    spinG->setValue(-g);
    spinG->setEnabled(false);
  } else {
    check->setCheckState(Qt::Checked);
    spinG->setValue(g);
    spinG->setEnabled(true);
  }
}

float AdditiveWindow::updateValChecked(float val, int checked) {
  switch (checked) {
    case Qt::Checked:
      // make sure vlaue is positive
      if (val < 0)
        return -val;
      return val;
      break;
    case Qt::Unchecked:
      // make sure val is negative
      if (val > 0)
        return -val;
      else if (val == 0)
        return -0.001;
      return val;
      break;
    default:
      return val;
  }
  return val;
}
