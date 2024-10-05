// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "styletabledelegate.h"

#include <QApplication>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QStyleOptionButton>

StyleTableDelegate::StyleTableDelegate(QObject* parent) : QStyledItemDelegate{parent} {}

QWidget* StyleTableDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */,
                                          const QModelIndex& index) const {
  switch (index.column()) {
    case 0: {
      // Limit (bool)
      QCheckBox* editor = new QCheckBox(parent);
      return editor;
    }
    default: {
      // Use double spin box for all other
      QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(-999);
      editor->setMaximum(999);
      editor->setDecimals(2);
      editor->setSingleStep(1);
      return editor;
    }
  }
}

void StyleTableDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
  switch (index.column()) {
    case 0: {
      // Limit (bool)
      QCheckBox* checkBox = static_cast<QCheckBox*>(editor);
      checkBox->setChecked(index.data().toBool());
      break;
    }
    default: {
      float value = index.model()->data(index, Qt::DisplayRole).toFloat();
      QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->setValue(value);
      break;
    }
  }
}

void StyleTableDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
  switch (index.column()) {
    case 0: {
      // Limit (bool)
      QCheckBox* checkBox = static_cast<QCheckBox*>(editor);
      bool checked = checkBox->isChecked();
      model->setData(index, checked, Qt::EditRole);
      break;
    }
    default: {
      QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->interpretText();
      float value = doubleSpinBox->value();
      model->setData(index, value, Qt::EditRole);
      break;
    }
  }
}

void StyleTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
  switch (index.column()) {
    case 0: {
      // Limit (bool)
      bool checked = index.data().toBool();
      QStyleOptionButton cb;
      cb.rect = option.rect;
      cb.state = checked ? QStyle::State_On : QStyle::State_Off;
      QApplication::style()->drawControl(QStyle::CE_CheckBox, &cb, painter);
      break;
    }
    default: {
      QStyledItemDelegate::paint(painter, option, index);
      break;
    }
  }
}
