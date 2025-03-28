// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixresultwidget.h"

#include <QGridLayout>

MixResultWidget::MixResultWidget(Mixture& mixture, Styles& styleDb, Settings& settings, QWidget* parent)
    : QFrame{parent}, mix{mixture}, sDb{styleDb}, lim{settings} {
  mixStyle.fromJson(mix.style->toJson());  // Make backup of style from mixture

  // set frame
  this->setFrameStyle(QFrame::Panel | QFrame::Plain);
  this->setLineWidth(2);

  // build ui
  auto* layout = new QGridLayout();
  this->setLayout(layout);

  int row = 0;
  layout->addWidget(new QLabel(tr("Ergebnis")), row, 0, Qt::AlignLeft);

  styleSelect = new QComboBox();
  // NOLINTNEXTLINE(*-magic-numbers)
  styleSelect->setMinimumContentsLength(30);  // Use available space
  styleSelect->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);

  layout->addWidget(styleSelect, ++row, 0, 1, 4, Qt::AlignLeft);
  QObject::connect(styleSelect, &QComboBox::activated, this, &MixResultWidget::selectStyle);

  // values
  for (uint i = 0; i < vals.size(); i++) {
    ++row;
    // Description
    layout->addWidget(new QLabel(Water::strTranslate.at(i)), row, 0);
    // value
    vals.at(i) = new QLabel();
    layout->addWidget(vals.at(i), row, 1, Qt::AlignRight);
    // Unit
    layout->addWidget(new QLabel(Water::strUnit.at(i)), row, 2, Qt::AlignLeft);
    // Bars
    if (i != static_cast<int>(Water::Value::Volume)) {
      bars.at(i) = new ResultBar();
      layout->addWidget(bars.at(i), row, 3);
    }
  }
  layout->setRowStretch(++row, 99);  // NOLINT(*-magic-numbers)

  styleIdx = 0;  // Select style of mixture
  updateStyles();
  update();

  // get changes
  QObject::connect(&lim, &Settings::dataModified, this, &MixResultWidget::update);
  QObject::connect(&sDb, &Styles::dataModified, this, &MixResultWidget::updateStyles);
}

bool MixResultWidget::isChanged() const {
  return styleIdx != 0;
}

void MixResultWidget::update() {
  const auto [water, mashPh] = mix.calc();
  for (uint i = 0; i < vals.size(); i++) {
    const auto what = static_cast<Water::Value>(i);
    const double value = (what == Water::Value::MashPh) ? mashPh : water.get(what);
    vals.at(i)->setText(QString::number(value, 'f', 2));
    // Update Bars
    if (what == Water::Value::Volume) {
      continue;  // Skip volume, it has no bar
    }
    bars.at(i)->setLimits(lim.getMin(what), lim.getMax(what), lim.isNegativeAllowed(what),
                          lim.isLogarithmicScale(what));
    if (mix.style->isLimited(what)) {
      bars.at(i)->setStyle(mix.style->get(what, Style::Limit::Min), mix.style->get(what, Style::Limit::Target),
                           mix.style->get(what, Style::Limit::Max));
    } else {
      bars.at(i)->setNoStyle();
    }
    bars.at(i)->setValue(value);
  }
}

void MixResultWidget::updateStyles() {
  // rebuild list with styles
  styleSelect->clear();
  // first add style from mixture
  const QString self = "[Mix] " + mixStyle.getName();
  styleSelect->addItem(self);
  // add styles form database
  for (int i = 0; i < sDb.rowCount(QModelIndex()); i++) {
    styleSelect->addItem(sDb.getStyle(i)->getName());
  }

  // restore selection
  if (styleIdx == 0) {
    // special case style of mixture
    styleSelect->setCurrentIndex(styleIdx);
    return;
  }

  // get uuid of current style
  const QString uuid = mix.style->getUuid();
  if (styleIdx > 0 && styleIdx < styleSelect->count() && uuid == sDb.getStyle(styleIdx - 1)->getUuid()) {
    // idx still valid and points to the same style
    styleSelect->setCurrentIndex(styleIdx);
    update();
    return;
  }

  // try to find mix by uuid
  // Also check mixStyle
  if (uuid == mixStyle.getUuid()) {
    styleIdx = 0;
    styleSelect->setCurrentIndex(styleIdx);
    update();
    return;
  }
  for (int i = 0; i < sDb.rowCount(QModelIndex()); i++) {
    if (uuid == sDb.getStyle(i)->getUuid()) {
      styleIdx = i + 1;
      styleSelect->setCurrentIndex(styleIdx);
      update();
      return;
    }
  }

  // Currently selected style no where found => style was deleted
  styleSelect->addItem("[Deleted] " + mix.style->getName());
  styleIdx = styleSelect->count() - 1;
  styleSelect->setCurrentIndex(styleIdx);
}

void MixResultWidget::selectStyle(int index) {
  styleIdx = index;
  // We make a copy of the style on selection
  // For getting changes, the user needs to re select the same style

  // check if mixture style is selectd
  if (index == 0) {
    mix.style->fromJson(mixStyle.toJson());
    update();
  } else if (--index < sDb.rowCount(QModelIndex())) {
    mix.style->fromJson(sDb.getStyle(index)->toJson());
    update();
  }

  // delete selection for possible deleted old style
  const int lastIdx = styleSelect->count() - 1;
  if (lastIdx > styleIdx && lastIdx > sDb.rowCount(QModelIndex())) {
    styleSelect->removeItem(lastIdx);
  }
  emit selectionChanged();
}
