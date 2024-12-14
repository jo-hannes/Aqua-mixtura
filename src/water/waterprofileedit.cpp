// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "waterprofileedit.h"

#include "../common/buttons.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>

WaterProfileEdit::WaterProfileEdit(QWidget* parent) : QWidget{parent} {
  layout = new QGridLayout();
  int row = 0;

  auto* txtName = new QLabel(tr("Name"));
  txtName->setStyleSheet("font-weight: bold");
  txtName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtName, row, 0, Qt::AlignLeft);
  row++;
  name = new QLineEdit();
  name->setMinimumWidth(220);  // Force line edit to use the available space
  layout->addWidget(name, row, 0, 1, 3, Qt::AlignLeft);
  QObject::connect(name, &QLineEdit::textEdited, this, &WaterProfileEdit::valChangName);
  row++;

  // Kationen
  auto* txtKat = new QLabel(tr("Kationen"));
  txtKat->setStyleSheet("font-weight: bold");
  layout->addWidget(txtKat, row++, 0, 1, 3, Qt::AlignLeft);

  calcium = new QDoubleSpinBox();
  addEditableValue(row++, calcium, Water::Value::Calcium);
  QObject::connect(calcium, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeCalcium);

  magnesium = new QDoubleSpinBox();
  addEditableValue(row++, magnesium, Water::Value::Magnesium);
  QObject::connect(magnesium, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeMagnesium);

  natrium = new QDoubleSpinBox();
  addEditableValue(row++, natrium, Water::Value::Natrium);
  QObject::connect(natrium, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeNatrium);

  // Anionen
  auto* txtAni = new QLabel(tr("Anionen"));
  txtAni->setStyleSheet("font-weight: bold");
  layout->addWidget(txtAni, row++, 0, 1, 3, Qt::AlignLeft);

  hydrogencarbonat = new QDoubleSpinBox();
  addEditableValue(row++, hydrogencarbonat, Water::Value::Hydrogencarbonat);
  QObject::connect(hydrogencarbonat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeHydrogencarbonat);

  chlorid = new QDoubleSpinBox();
  addEditableValue(row++, chlorid, Water::Value::Chlorid);
  QObject::connect(chlorid, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeChlorid);

  sulfat = new QDoubleSpinBox();
  addEditableValue(row++, sulfat, Water::Value::Sulfat);
  QObject::connect(sulfat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeSulfat);

  phosphat = new QDoubleSpinBox();
  addEditableValue(row++, phosphat, Water::Value::Phosphat);
  QObject::connect(phosphat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangePhosphat);

  lactat = new QDoubleSpinBox();
  addEditableValue(row++, lactat, Water::Value::Lactat);
  QObject::connect(lactat, &QDoubleSpinBox::valueChanged, this, &WaterProfileEdit::valChangeLactat);

  auto* txtCalc = new QLabel(tr("Berechnet"));
  txtCalc->setStyleSheet("font-weight: bold");
  layout->addWidget(txtCalc, row++, 0, 1, 3, Qt::AlignLeft);

  restalkallität = new QLabel("");
  addCalculatedValue(row++, restalkallität, Water::Value::Restalkalitaet);

  gesammtHaerte = new QLabel("");
  addCalculatedValue(row++, gesammtHaerte, Water::Value::Gesamthaerte);

  carbonHaerte = new QLabel("");
  addCalculatedValue(row++, carbonHaerte, Water::Value::Carbonhaerte);

  caHaerte = new QLabel("");
  addCalculatedValue(row++, caHaerte, Water::Value::CaHaerte);

  mgHaerte = new QLabel();
  addCalculatedValue(row++, mgHaerte, Water::Value::MgHaerte);

  nichtcarbonHaerte = new QLabel();
  addCalculatedValue(row++, nichtcarbonHaerte, Water::Value::NichtCarbonhaerte);

  so4cl = new QLabel();
  addCalculatedValue(row++, so4cl, Water::Value::SO4ClVerhaeltnis);

  // buttons
  row++;
  layout->setRowStretch(row, 99);  // NOLINT(*-magic-numbers)
  row++;
  auto* btnBox = new Buttons(tr("Speichern"), tr("Abbrechen"));
  QObject::connect(btnBox->btnSave, &QPushButton::clicked, this, &WaterProfileEdit::save);
  QObject::connect(btnBox->btnCancel, &QPushButton::clicked, this, &WaterProfileEdit::cancel);
  layout->addWidget(btnBox, row, 0, 1, 3, Qt::AlignCenter);

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
  calcium->setValue(volatileProfile.get(Water::Value::Calcium));
  magnesium->setValue(volatileProfile.get(Water::Value::Magnesium));
  natrium->setValue(volatileProfile.get(Water::Value::Natrium));
  hydrogencarbonat->setValue(volatileProfile.get(Water::Value::Hydrogencarbonat));
  chlorid->setValue(volatileProfile.get(Water::Value::Chlorid));
  sulfat->setValue(volatileProfile.get(Water::Value::Sulfat));
  phosphat->setValue(volatileProfile.get(Water::Value::Phosphat));
  lactat->setValue(volatileProfile.get(Water::Value::Lactat));
  updateCalculated();
}

void WaterProfileEdit::updateCalculated() {
  restalkallität->setText(QString::number(volatileProfile.get(Water::Value::Restalkalitaet), 'f', 2));
  gesammtHaerte->setText(QString::number(volatileProfile.get(Water::Value::Gesamthaerte), 'f', 2));
  carbonHaerte->setText(QString::number(volatileProfile.get(Water::Value::Carbonhaerte), 'f', 2));
  caHaerte->setText(QString::number(volatileProfile.get(Water::Value::CaHaerte), 'f', 2));
  mgHaerte->setText(QString::number(volatileProfile.get(Water::Value::MgHaerte), 'f', 2));
  nichtcarbonHaerte->setText(QString::number(volatileProfile.get(Water::Value::NichtCarbonhaerte), 'f', 2));
  so4cl->setText(QString::number(volatileProfile.get(Water::Value::SO4ClVerhaeltnis), 'f', 2));
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
  volatileProfile.set(Water::Value::Calcium, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeMagnesium(double val) {
  volatileProfile.set(Water::Value::Magnesium, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeNatrium(double val) {
  volatileProfile.set(Water::Value::Natrium, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeHydrogencarbonat(double val) {
  volatileProfile.set(Water::Value::Hydrogencarbonat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeChlorid(double val) {
  volatileProfile.set(Water::Value::Chlorid, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeSulfat(double val) {
  volatileProfile.set(Water::Value::Sulfat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangePhosphat(double val) {
  volatileProfile.set(Water::Value::Phosphat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::valChangeLactat(double val) {
  volatileProfile.set(Water::Value::Lactat, val);
  changed = true;
  updateCalculated();
}

void WaterProfileEdit::addEditableValue(int row, QDoubleSpinBox* spinBox, Water::Value what) {
  spinBox->setDecimals(2);
  spinBox->setMinimum(0);
  spinBox->setMaximum(999);  // NOLINT(*-magic-numbers)
  auto* txt = new QLabel(Water::translatableStrings[static_cast<uint>(what)]);
  txt->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txt, row, 0, Qt::AlignLeft);
  layout->addWidget(spinBox, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel(Water::waterStrings[static_cast<uint>(what)][static_cast<uint>(Water::Idx::Unit)]), row,
                    2, Qt::AlignLeft);
}

void WaterProfileEdit::addCalculatedValue(int row, QLabel* value, Water::Value what) {
  auto* txt = new QLabel(Water::translatableStrings[static_cast<uint>(what)]);
  txt->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txt, row, 0, Qt::AlignLeft);
  layout->addWidget(value, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel(Water::waterStrings[static_cast<uint>(what)][static_cast<uint>(Water::Idx::Unit)]), row,
                    2, Qt::AlignLeft);
}
