// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "maltphedit.h"

#include "../model/malt.h"

#include <QGridLayout>
#include <QPushButton>

MaltPhEdit::MaltPhEdit(QWidget *parent)
    : QWidget{parent}
{
  QGridLayout* mainLayout = new QGridLayout(this);
  // malt names
  QLabel* txtMalt = new QLabel(tr("Malze <120 EBC:"));
  mainLayout->addWidget(txtMalt, 0, 0, Qt::AlignRight);
  QLabel* txtCaramalz = new QLabel(tr("Caramalz:"));
  mainLayout->addWidget(txtCaramalz, 1, 0, Qt::AlignRight);
  QLabel* txtRoestmalz = new QLabel(tr("Röstmalz:"));
  mainLayout->addWidget(txtRoestmalz, 2, 0, Qt::AlignRight);
  // calculated ph values
  phMalt = new QLabel();
  mainLayout->addWidget(phMalt, 0, 1, Qt::AlignLeft);
  phCaramalz = new QLabel();
  mainLayout->addWidget(phCaramalz, 1, 1, Qt::AlignLeft);
  phRoestmalz = new QLabel();
  mainLayout->addWidget(phRoestmalz, 2, 1, Qt::AlignLeft);
  // buttons to use calculated values
  QPushButton* btnMalt = new QPushButton();
  btnMalt->setToolTip(tr("Wert übernehmen"));
  btnMalt->setIcon(QIcon(":/icons/arrow-down-right.svg"));
  QObject::connect(btnMalt, &QPushButton::clicked, this, &MaltPhEdit::usePhMalt);
  mainLayout->addWidget(btnMalt, 0, 2);
  QPushButton* btnCaramalz = new QPushButton();
  btnCaramalz->setToolTip(tr("Wert übernehmen"));
  btnCaramalz->setIcon(QIcon(":/icons/arrow-right.svg"));
  QObject::connect(btnCaramalz, &QPushButton::clicked, this, &MaltPhEdit::usePhCaramalz);
  mainLayout->addWidget(btnCaramalz, 1, 2);
  QPushButton* btnRoestmalz = new QPushButton();
  btnRoestmalz->setToolTip(tr("Wert übernehmen"));
  btnRoestmalz->setIcon(QIcon(":/icons/arrow-up-right.svg"));
  QObject::connect(btnRoestmalz, &QPushButton::clicked, this, &MaltPhEdit::usePhRoestmalz);
  mainLayout->addWidget(btnRoestmalz, 2, 2);
  // labels
  QLabel* txtEbc = new QLabel("EBC:");
  mainLayout->addWidget(txtEbc, 0, 3, Qt::AlignRight);
  QLabel* txtPh = new QLabel("pH:");
  mainLayout->addWidget(txtPh, 1, 3, Qt::AlignRight);
  // values
  ebcLabel = new QLabel();
  mainLayout->addWidget(ebcLabel, 0, 4, Qt::AlignLeft);
  phSpinBox = new QDoubleSpinBox();
  phSpinBox->setMinimum(0);
  phSpinBox->setMaximum(14);
  phSpinBox->setDecimals(2);
  phSpinBox->setSingleStep(0.1);
  mainLayout->addWidget(phSpinBox, 1, 4, Qt::AlignLeft);

  this->setWindowFlags(Qt::Sheet);  // TODO check ui on other systems than macOS. Maybe use Qt::Tool on other systems
}

void MaltPhEdit::setPh(float newPh) {
  phSpinBox->setValue(newPh);
}

void MaltPhEdit::setEbc(float newEbc) {
  ebc = newEbc;
  // update ebc label
  ebcLabel->setText(QString::number(ebc, 'f', 2));
  // update calcualted pH values
  phMalt->setText(QString::number(Malt::calcualtePhUpTo200Ebc(ebc), 'f', 2));
  phCaramalz->setText(QString::number(Malt::calcualtePhCaramalz(ebc), 'f', 2));
  phRoestmalz->setText(QString::number(Malt::calcualtePhRoestmalz(), 'f', 2));
}

void MaltPhEdit::setData(float newPh, float newEbc) {
  setPh(newPh);
  setEbc(newEbc);
}

float MaltPhEdit::pH() const {
  phSpinBox->interpretText();
  return phSpinBox->value();
}

void MaltPhEdit::usePhMalt() {
  setPh(Malt::calcualtePhUpTo200Ebc(ebc));
}

void MaltPhEdit::usePhCaramalz() {
  setPh(Malt::calcualtePhCaramalz(ebc));
}

void MaltPhEdit::usePhRoestmalz() {
  setPh(Malt::calcualtePhRoestmalz());
}
