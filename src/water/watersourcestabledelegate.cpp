// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "watersourcestabledelegate.h"

#include <QDoubleSpinBox>

WaterSourcesTableDelegate::WaterSourcesTableDelegate(QObject* parent) : QStyledItemDelegate{parent} {}

QWidget* WaterSourcesTableDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                                 const QModelIndex& index) const {
  switch (index.column()) {
    case 2: {
      // Volume
      QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(999);
      editor->setDecimals(2);
      editor->setSingleStep(0.1);
      return editor;
    }
    default: {
      return QStyledItemDelegate::createEditor(parent, option, index);
    }
  }
}

void WaterSourcesTableDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
  switch (index.column()) {
    case 2: {
      // Volume
      float value = index.model()->data(index, Qt::DisplayRole).toFloat();
      QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->setValue(value);
      break;
    }
    default: {
      QStyledItemDelegate::setEditorData(editor, index);
      break;
    }
  }
}

void WaterSourcesTableDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                             const QModelIndex& index) const {
  switch (index.column()) {
    case 2: {
      // Volume
      QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->interpretText();
      float value = doubleSpinBox->value();
      model->setData(index, value, Qt::EditRole);
      break;
    }
    default: {
      QStyledItemDelegate::setModelData(editor, model, index);
      break;
    }
  }
}
