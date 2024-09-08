// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixturewindow.h"

#include "../common/buttons.h"
#include "../common/dialogs.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QVBoxLayout>

MixtureWindow::MixtureWindow(Mixture& mixture, WaterSources& waterDb, AdditiveSettings& additiveCfg, Malts& maltDb,
                             Styles& styleDb, Settings& settings, QWidget* parent)
    : QWidget{parent}, mix{mixture} {
  this->setWindowFlags(Qt::Window);

  // build ui elements
  // sub widgets
  mww = new MixWaterWidget(*mix.waters, waterDb, this);
  mmw = new MixMaltWidget(mix.malts, maltDb, this);
  maw = new MixAdditiveWidget(mix.additive, additiveCfg);
  mrw = new MixResultWidget(mix, styleDb, settings, this);
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

  nameEdit->setText(mix.getName());
  changed(false);

  loadGuard = false;

  // get changes
  QObject::connect(mix.waters, &WaterSources::dataChanged, this, &MixtureWindow::update);
  QObject::connect(mix.waters, &WaterSources::dataModified, this, &MixtureWindow::update);
  QObject::connect(mix.additive, &Additive::dataModified, this, &MixtureWindow::update);
  QObject::connect(mix.malts, &Malts::dataModified, this, &MixtureWindow::update);
  QObject::connect(mrw, &MixResultWidget::selectionChanged, this, &MixtureWindow::update);
}

void MixtureWindow::closeEvent(QCloseEvent* event) {
  if (unsavedChanges) {
    int ret = Dialogs::saveChanges(tr("Änderungen speichern?"),
                                   tr("Aufbereitung \"%1\" hat ungespeicherte Änderungen").arg(mix.getName()));
    switch (ret) {
      case QMessageBox::Save:
        save();  // save and close window
        break;
      case QMessageBox::Discard:
        load();
        break;
      case QMessageBox::Cancel:
        event->ignore();  // ignore event to keep window open
        return;
        break;
    }
  }
  event->accept();
}

void MixtureWindow::setName(QString name) {
  qDebug() << "MixtureWindow::setName " << name;
  mix.setName(name);
  changed(true);
}

void MixtureWindow::load() {
  loadGuard = true;
  qDebug() << "Loading mixture";
  mix.load();
  // update values if needed
  nameEdit->setText(mix.getName());
  maw->update();
  // update results
  mrw->update();
  mrw->updateStyles();
  // update change state and window title
  changed(false);
  loadGuard = false;
}

void MixtureWindow::save() {
  mix.save();
  changed(false);
}

void MixtureWindow::update() {
  mrw->update();
  changed(true);
}

void MixtureWindow::changed(bool changed) {
  unsavedChanges = changed;
  qDebug() << "MixtureWindow::changed; changed " << changed << " unsavedChanges " << unsavedChanges;
  if (unsavedChanges && !loadGuard) {
    this->setWindowTitle("* Aqua mixtura - " + tr("Aufbereitung: ") + mix.getName());
    qDebug() << "Update edit time";
    mix.updateEditTime();
  } else {
    this->setWindowTitle("Aqua mixtura - " + tr("Aufbereitung: ") + mix.getName());
  }
}
