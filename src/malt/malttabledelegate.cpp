// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "malttabledelegate.h"

#include "maltphedit.h"

#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>

MaltTableDelegate::MaltTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

QWidget* MaltTableDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */,
                                         const QModelIndex& index) const {
  switch (index.column()) {
    case 3: {
      // mass
      auto* editor = new QDoubleSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(999);  // NOLINT(*-magic-numbers)
      editor->setDecimals(2);
      editor->setSingleStep(0.1);
      return editor;
    }
    case 1: {
      // ebc
      auto* editor = new QSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(9999);  // NOLINT(*-magic-numbers)
      return editor;
    }
    case 2: {
      // ph
      auto* editor = new MaltPhEdit(parent);
      return editor;
    }
    case 0:  // name
    default: {
      return new QLineEdit(parent);
    }
  }
}

void MaltTableDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
  switch (index.column()) {
    case 3: {
      // mass
      const double value = index.model()->data(index, Qt::DisplayRole).toDouble();
      auto* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->setValue(value);
      break;
    }
    case 1: {
      // ebc
      const int value = index.model()->data(index, Qt::DisplayRole).toInt();
      auto* spinBox = dynamic_cast<QSpinBox*>(editor);
      spinBox->setValue(value);
      break;
    }
    case 2: {
      // ph
      const double ph = index.model()->data(index, Qt::DisplayRole).toDouble();
      const QModelIndex ebcIdx = index.siblingAtColumn(1);
      const double ebc = ebcIdx.model()->data(ebcIdx, Qt::DisplayRole).toDouble();
      auto* phEdit = dynamic_cast<MaltPhEdit*>(editor);
      phEdit->setData(ph, ebc);
      break;
    }
    case 0:  // name
    default: {
      const QString value = index.model()->data(index, Qt::DisplayRole).toString();
      auto* lineEdit = dynamic_cast<QLineEdit*>(editor);
      lineEdit->setText(value);
      break;
    }
  }
}

void MaltTableDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
  switch (index.column()) {
    case 3: {
      // mass
      auto* doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->interpretText();
      const double value = doubleSpinBox->value();
      model->setData(index, value, Qt::EditRole);
      break;
    }
    case 1: {
      // ebc
      auto* spinBox = dynamic_cast<QSpinBox*>(editor);
      spinBox->interpretText();
      const int value = spinBox->value();
      model->setData(index, value, Qt::EditRole);
      break;
    }
    case 2: {
      // ph
      auto* phEdit = dynamic_cast<MaltPhEdit*>(editor);
      const double value = phEdit->pH();
      model->setData(index, value, Qt::EditRole);
      break;
    }
    case 0:  // name
    default: {
      auto* lineEdit = dynamic_cast<QLineEdit*>(editor);
      const QString value = lineEdit->text();
      model->setData(index, value, Qt::EditRole);
      break;
    }
  }
}

void MaltTableDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                             const QModelIndex& /* index */) const {
  editor->setGeometry(option.rect);
}
