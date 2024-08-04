// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixturewindow.h"

#include "../common/buttons.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

MixtureWindow::MixtureWindow(Mixture& mixture, WaterSources* waterDb, AdditiveSettings& additiveCfg, Malts* maltDb,
                             Styles* styleDb, Limits* limits, QWidget* parent)
    : QWidget{parent}, mix{mixture} {
  this->setWindowFlags(Qt::Window);

  // build ui elements
  // sub widgets
  mww = new MixWaterWidget(mix.waters, waterDb, this);
  mmw = new MixMaltWidget(mix.malts, maltDb, this);
  maw = new MixAdditiveWidget(mix.additive, additiveCfg);
  mrw = new MixResultWidget(mix, styleDb, limits, this);
  QObject::connect(mix.waters, &WaterSources::dataChanged, mrw, &MixResultWidget::update);
  QObject::connect(mix.additive, &Additive::dataModified, mrw, &MixResultWidget::update);
  QObject::connect(limits, &Limits::dataModified, mrw, &MixResultWidget::update);
  // Name edit
  QFrame* nameFrame = new QFrame();
  nameFrame->setFrameStyle(QFrame::Panel | QFrame::Plain);
  nameFrame->setLineWidth(2);
  QHBoxLayout* nameLayout = new QHBoxLayout();
  nameFrame->setLayout(nameLayout);
  QLabel* nameLabel = new QLabel(tr("Name:"));
  nameLayout->addWidget(nameLabel);
  nameEdit = new QLineEdit();
  QObject::connect(nameEdit, &QLineEdit::textEdited, this, &MixtureWindow::setName);
  nameLayout->addWidget(nameEdit);
  // buttons
  Buttons* buttons = new Buttons(tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnSave, &QPushButton::clicked, this, &MixtureWindow::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &MixtureWindow::load);

  // Sticking it together
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  QHBoxLayout* layout = new QHBoxLayout();
  mainLayout->addLayout(layout);
  mainLayout->addWidget(buttons, 0, Qt::AlignHCenter);

  // column 1 of layout
  QVBoxLayout* col1 = new QVBoxLayout();
  layout->addLayout(col1);
  col1->addWidget(nameFrame);
  col1->addWidget(mww);
  col1->addWidget(mmw);

  layout->addWidget(maw);
  layout->addWidget(mrw);

  this->setWindowTitle("Aqua mixtura - " + tr("Aufbereitung: ") + mix.getName());
  nameEdit->setText(mix.getName());
}

void MixtureWindow::setName(QString name) {
  mix.setName(name);
  this->setWindowTitle("Aqua mixtura - " + tr("Aufbereitung: ") + mix.getName());
  nameEdit->setText(mix.getName());
}

void MixtureWindow::load() {
  mix.load();
  // update values if needed
  this->setWindowTitle("Aqua mixtura - " + tr("Aufbereitung: ") + mix.getName());
  nameEdit->setText(mix.getName());
  maw->update();
  // update results
  mrw->update();
}

void MixtureWindow::save() {
  mix.save();
}
