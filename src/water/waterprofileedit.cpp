// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "waterprofileedit.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>

WaterProfileEdit::WaterProfileEdit(QWidget* parent) : QWidget{parent} {
  layout = new QGridLayout();
  changed = false;
  int row = 0;

  QLabel* txtName = new QLabel(tr("Name"));
  txtName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtName, row, 0, Qt::AlignLeft);
  name = new QLineEdit();
  layout->addWidget(name, row, 1, 1, 2, Qt::AlignRight);
  QObject::connect(name, &QLineEdit::textEdited, this, &WaterProfileEdit::valChangName);
  row++;

  // Kationen
  QLabel* txtKat = new QLabel(tr("Kationen"));
  txtKat->setStyleSheet("font-weight: bold");
  layout->addWidget(txtKat, row++, 0, 1, 3, Qt::AlignLeft);

  calcium = new QDoubleSpinBox();
  addEditableValue(row++, calcium, tr("Calcium"), "mg/l");
  QObject::connect(calcium, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeCalcium);

  magnesium = new QDoubleSpinBox();
  addEditableValue(row++, magnesium, tr("Magnesium"), "mg/l");
  QObject::connect(magnesium, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeMagnesium);

  natrium = new QDoubleSpinBox();
  addEditableValue(row++, natrium, tr("Natrium"), "mg/l");
  QObject::connect(natrium, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeNatrium);

  // Anionen
  QLabel* txtAni = new QLabel(tr("Anionen"));
  txtAni->setStyleSheet("font-weight: bold");
  layout->addWidget(txtAni, row++, 0, 1, 3, Qt::AlignLeft);

  hydrogencarbonat = new QDoubleSpinBox();
  addEditableValue(row++, hydrogencarbonat, tr("Hydrogencarbonat"), "mg/l");
  QObject::connect(hydrogencarbonat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeHydrogencarbonat);

  chlorid = new QDoubleSpinBox();
  addEditableValue(row++, chlorid, tr("Chlorid"), "mg/l");
  QObject::connect(chlorid, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeChlorid);

  sulfat = new QDoubleSpinBox();
  addEditableValue(row++, sulfat, tr("Sulfat"), "mg/l");
  QObject::connect(sulfat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeSulfat);

  phosphat = new QDoubleSpinBox();
  addEditableValue(row++, phosphat, tr("Phosphat"), "mg/l");
  QObject::connect(phosphat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangePhosphat);

  lactat = new QDoubleSpinBox();
  addEditableValue(row++, lactat, tr("Lactat"), "mg/l");
  QObject::connect(lactat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeLactat);

  QLabel* txtCalc = new QLabel(tr("Berechnet"));
  txtCalc->setStyleSheet("font-weight: bold");
  layout->addWidget(txtCalc, row++, 0, 1, 3, Qt::AlignLeft);

  restalkallität = new QLabel("");
  addCalculatedValue(row++, restalkallität, tr("Restalaklität"), "°dH");

  gesammtHaerte = new QLabel("");
  addCalculatedValue(row++, gesammtHaerte, tr("Gesammthärte"), "°dH");

  carbonHaerte = new QLabel("");
  addCalculatedValue(row++, carbonHaerte, tr("Carbonhärte"), "°dH");

  caHaerte = new QLabel("");
  addCalculatedValue(row++, caHaerte, tr("Ca-Härte"), "°dH");

  mgHaerte = new QLabel();
  addCalculatedValue(row++, mgHaerte, tr("Mg-Härte"), "°dH");

  nichtcarbonHaerte = new QLabel();
  addCalculatedValue(row++, nichtcarbonHaerte, tr("Nichtcarbonhärte"), "°dH");

  so4cl = new QLabel();
  addCalculatedValue(row++, so4cl, tr("SO4/Cl-Verhältnis"), "");

  // buttons
  row++;
  QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
  QObject::connect(btnBox, &QDialogButtonBox::accepted, this, &WaterProfileEdit::save);
  QObject::connect(btnBox, &QDialogButtonBox::rejected, this, &WaterProfileEdit::cancel);
  layout->addWidget(btnBox, row, 0, 3, 0, Qt::AlignCenter);

  this->setLayout(layout);
}

void WaterProfileEdit::setProfile(const Water& profile) {
  givenProfile = profile;
  volatileProfile = profile;
  updateAll();
  changed = false;
}

bool WaterProfileEdit::isChanged() const {
  return changed;
}

void WaterProfileEdit::updateAll() {
  name->setText(volatileProfile.getName());
  calcium->setValue(volatileProfile.get(AM::WaterValue::Calcium));
  magnesium->setValue(volatileProfile.get(AM::WaterValue::Magnesium));
  natrium->setValue(volatileProfile.get(AM::WaterValue::Natrium));
  hydrogencarbonat->setValue(volatileProfile.get(AM::WaterValue::Hydrogencarbonat));
  chlorid->setValue(volatileProfile.get(AM::WaterValue::Chlorid));
  sulfat->setValue(volatileProfile.get(AM::WaterValue::Sulfat));
  phosphat->setValue(volatileProfile.get(AM::WaterValue::Phosphat));
  lactat->setValue(volatileProfile.get(AM::WaterValue::Lactat));
  updateCalculated();
}

void WaterProfileEdit::updateCalculated() {
  restalkallität->setText(QString::number(volatileProfile.get(AM::WaterValue::Restalkalitaet), 'f', 2));
  gesammtHaerte->setText(QString::number(volatileProfile.get(AM::WaterValue::Gesamthaerte), 'f', 2));
  carbonHaerte->setText(QString::number(volatileProfile.get(AM::WaterValue::Carbonhaerte), 'f', 2));
  caHaerte->setText(QString::number(volatileProfile.get(AM::WaterValue::CaHaerte), 'f', 2));
  mgHaerte->setText(QString::number(volatileProfile.get(AM::WaterValue::MgHaerte), 'f', 2));
  nichtcarbonHaerte->setText(QString::number(volatileProfile.get(AM::WaterValue::NichtCarbonhaerte), 'f', 2));
  so4cl->setText(QString::number(volatileProfile.get(AM::WaterValue::SO4ClVerhaeltnis), 'f', 2));
}

void WaterProfileEdit::save() {
  givenProfile = volatileProfile;
  emit saveProfile(volatileProfile);
  changed = false;
}

void WaterProfileEdit::cancel() {
  volatileProfile = givenProfile;
  updateAll();
  changed = false;
}

void WaterProfileEdit::valChangName(const QString& name) {
  volatileProfile.setName(name);
  changed = true;
}

void WaterProfileEdit::valChangeCalcium(double val) {
  volatileProfile.set(AM::WaterValue::Calcium, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeMagnesium(double val) {
  volatileProfile.set(AM::WaterValue::Magnesium, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeNatrium(double val) {
  volatileProfile.set(AM::WaterValue::Natrium, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeHydrogencarbonat(double val) {
  volatileProfile.set(AM::WaterValue::Hydrogencarbonat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeChlorid(double val) {
  volatileProfile.set(AM::WaterValue::Chlorid, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeSulfat(double val) {
  volatileProfile.set(AM::WaterValue::Sulfat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangePhosphat(double val) {
  volatileProfile.set(AM::WaterValue::Phosphat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeLactat(double val) {
  volatileProfile.set(AM::WaterValue::Lactat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::addEditableValue(int row, QDoubleSpinBox* spinBox, QString text, QString unit) {
  spinBox->setDecimals(2);
  spinBox->setMinimum(0);
  spinBox->setMaximum(999);  // TODO what to use as maximum?
  QLabel* txt = new QLabel(text);
  txt->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txt, row, 0, Qt::AlignLeft);
  layout->addWidget(spinBox, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel(unit), row, 2, Qt::AlignRight);
}

void WaterProfileEdit::addCalculatedValue(int row, QLabel* value, QString text, QString unit) {
  QLabel* txt = new QLabel(text);
  txt->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txt, row, 0, Qt::AlignLeft);
  layout->addWidget(value, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel(unit), row, 2, Qt::AlignRight);
}
