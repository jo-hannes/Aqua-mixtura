// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "waterprofileview.h"

#include <QGridLayout>

WaterProfileView::WaterProfileView(QWidget* parent) : QWidget(parent) {
  QGridLayout* layout = new QGridLayout();

  // Header
  int row = 0;
  name = new QLabel();
  amount = new QLabel();
  unit = new QLabel("L");
  layout->addWidget(name, row, 0, Qt::AlignLeft);
  layout->addWidget(amount, row, 1, Qt::AlignRight);
  layout->addWidget(unit, row, 2, Qt::AlignRight);

  // Kationen
  ++row;
  QLabel* txtKat = new QLabel(tr("Kationen"));
  txtKat->setStyleSheet("font-weight: bold");
  layout->addWidget(txtKat, row, 0, 1, 3, Qt::AlignLeft);

  ++row;
  QLabel* txtCa = new QLabel(tr("Calcium"));
  txtCa->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtCa, row, 0, Qt::AlignLeft);
  calcium = new QLabel();
  layout->addWidget(calcium, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtMg = new QLabel(tr("Magnesium"));
  txtMg->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtMg, row, 0, Qt::AlignLeft);
  magnesium = new QLabel();
  layout->addWidget(magnesium, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtNa = new QLabel(tr("Natrium"));
  txtNa->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtNa, row, 0, Qt::AlignLeft);
  natrium = new QLabel();
  layout->addWidget(natrium, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  // Anionen
  ++row;
  QLabel* txtAni = new QLabel(tr("Anionen"));
  txtAni->setStyleSheet("font-weight: bold");
  layout->addWidget(txtAni, row, 0, 1, 3, Qt::AlignLeft);

  ++row;
  QLabel* txtHyd = new QLabel(tr("Hydrogencarbonat"));
  txtHyd->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtHyd, row, 0, Qt::AlignLeft);
  hydrogencarbonat = new QLabel();
  layout->addWidget(hydrogencarbonat, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtCl = new QLabel(tr("Chlorid"));
  txtCl->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtCl, row, 0, Qt::AlignLeft);
  chlorid = new QLabel();
  layout->addWidget(chlorid, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtSl = new QLabel(tr("Sulfat"));
  txtSl->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtSl, row, 0, Qt::AlignLeft);
  sulfat = new QLabel();
  layout->addWidget(sulfat, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtP = new QLabel(tr("Phosphat"));
  txtP->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtP, row, 0, Qt::AlignLeft);
  phosphat = new QLabel();
  layout->addWidget(phosphat, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtLa = new QLabel(tr("Lactat"));
  txtLa->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtLa, row, 0, Qt::AlignLeft);
  lactat = new QLabel();
  layout->addWidget(lactat, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("mg/l"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtCalc = new QLabel(tr("Berechnet"));
  txtCalc->setStyleSheet("font-weight: bold");
  layout->addWidget(txtCalc, row, 0, 1, 3, Qt::AlignLeft);

  ++row;
  QLabel* txtRa = new QLabel(tr("Restalaklität"));
  txtRa->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtRa, row, 0, Qt::AlignLeft);
  restalkallität = new QLabel();
  layout->addWidget(restalkallität, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("°dH"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtGh = new QLabel(tr("Gesammthärte"));
  txtGh->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtGh, row, 0, Qt::AlignLeft);
  gesammtHaerte = new QLabel();
  layout->addWidget(gesammtHaerte, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("°dH"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtCh = new QLabel(tr("Carbonhärte"));
  txtCh->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtCh, row, 0, Qt::AlignLeft);
  carbonHaerte = new QLabel();
  layout->addWidget(carbonHaerte, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("°dH"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtCah = new QLabel(tr("Ca-Härte"));
  txtCah->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtCah, row, 0, Qt::AlignLeft);
  caHaerte = new QLabel();
  layout->addWidget(caHaerte, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("°dH"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtMgh = new QLabel(tr("Mg-Härte"));
  txtMgh->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtMgh, row, 0, Qt::AlignLeft);
  mgHaerte = new QLabel();
  layout->addWidget(mgHaerte, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("°dH"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtNch = new QLabel(tr("Nichtcarbonhärte"));
  txtNch->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtNch, row, 0, Qt::AlignLeft);
  nichtcarbonHaerte = new QLabel();
  layout->addWidget(nichtcarbonHaerte, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel("°dH"), row, 2, Qt::AlignRight);

  ++row;
  QLabel* txtSo4Cl = new QLabel(tr("SO4/Cl-Verhältnis"));
  txtSo4Cl->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  layout->addWidget(txtSo4Cl, row, 0, Qt::AlignLeft);
  so4cl = new QLabel();
  layout->addWidget(so4cl, row, 1, Qt::AlignRight);
  layout->addWidget(new QLabel(""), row, 2, Qt::AlignRight);
  this->setLayout(layout);
}

WaterProfileView::WaterProfileView(const Water& water, bool showHeader, QWidget* parent) : WaterProfileView(parent) {
  this->setWater(water);
  this->showHeader(showHeader);
}

void WaterProfileView::setProfile(const Water& profile) {
  name->setText(profile.getName());
  amount->setText("");
  calcium->setText(QString::number(profile.getCalzium(), 'f', 2));
  magnesium->setText(QString::number(profile.getMagnesium(), 'f', 2));
  natrium->setText(QString::number(profile.getNatrium(), 'f', 2));
  hydrogencarbonat->setText(QString::number(profile.getHydrogencarbonat(), 'f', 2));
  chlorid->setText(QString::number(profile.getChlorid(), 'f', 2));
  sulfat->setText(QString::number(profile.getSulfat(), 'f', 2));
  phosphat->setText(QString::number(profile.getPhosphat(), 'f', 2));
  lactat->setText(QString::number(profile.getLactat(), 'f', 2));
  restalkallität->setText(QString::number(profile.getRestalkalitaet(), 'f', 2));
  gesammtHaerte->setText(QString::number(profile.getGesamthaerte(), 'f', 2));
  carbonHaerte->setText(QString::number(profile.getCarbonhaerte(), 'f', 2));
  caHaerte->setText(QString::number(profile.getCaHaerte(), 'f', 2));
  mgHaerte->setText(QString::number(profile.getMgHaerte(), 'f', 2));
  nichtcarbonHaerte->setText(QString::number(profile.getNichtCarbonhaerte(), 'f', 2));
  so4cl->setText(QString::number(profile.getSO4ClVerhaeltnis(), 'f', 2));
}

void WaterProfileView::setWater(const Water& water) {
  name->setText(water.getName());
  amount->setText(QString::number(water.getVolume(), 'f', 2));
  calcium->setText(QString::number(water.getCalzium(), 'f', 2));
  magnesium->setText(QString::number(water.getMagnesium(), 'f', 2));
  natrium->setText(QString::number(water.getNatrium(), 'f', 2));
  hydrogencarbonat->setText(QString::number(water.getHydrogencarbonat(), 'f', 2));
  chlorid->setText(QString::number(water.getChlorid(), 'f', 2));
  sulfat->setText(QString::number(water.getSulfat(), 'f', 2));
  phosphat->setText(QString::number(water.getPhosphat(), 'f', 2));
  lactat->setText(QString::number(water.getLactat(), 'f', 2));
  restalkallität->setText(QString::number(water.getRestalkalitaet(), 'f', 2));
  gesammtHaerte->setText(QString::number(water.getGesamthaerte(), 'f', 2));
  carbonHaerte->setText(QString::number(water.getCarbonhaerte(), 'f', 2));
  caHaerte->setText(QString::number(water.getCaHaerte(), 'f', 2));
  mgHaerte->setText(QString::number(water.getMgHaerte(), 'f', 2));
  nichtcarbonHaerte->setText(QString::number(water.getNichtCarbonhaerte(), 'f', 2));
  so4cl->setText(QString::number(water.getSO4ClVerhaeltnis(), 'f', 2));
}

void WaterProfileView::showHeader(bool show) {
  name->setVisible(show);
  amount->setVisible(show);
  unit->setVisible(show);
}
