#include "malttabledelegate.h"

#include "maltphedit.h"

#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>

MaltTableDelegate::MaltTableDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

QWidget* MaltTableDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */,
                                         const QModelIndex& index) const {
  switch (index.column()) {
    case 1: {
      // mass
      QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(999);
      editor->setDecimals(2);
      editor->setSingleStep(0.1);
      return editor;
    }
    case 2: {
      // ebc
      QSpinBox* editor = new QSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(9999);
      return editor;
    }
    case 3: {
      // ph
      QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
      editor->setFrame(false);
      editor->setMinimum(0);
      editor->setMaximum(14);
      editor->setDecimals(2);
      editor->setSingleStep(0.1);
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
    case 2: {
      // ebc
      float value = index.model()->data(index, Qt::DisplayRole).toFloat();
      QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
      spinBox->setValue(value);
      break;
    }
    case 1:  // mass
    case 3: {
      // ph
      float value = index.model()->data(index, Qt::DisplayRole).toFloat();
      QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->setValue(value);
      break;
    }
    case 0:  // name
    default: {
      QString value = index.model()->data(index, Qt::DisplayRole).toString();
      QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
      lineEdit->setText(value);
      break;
    }
  }
}

void MaltTableDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
  switch (index.column()) {
    case 2: {
      // ebc
      QSpinBox* spinBox = static_cast<QSpinBox*>(editor);
      spinBox->interpretText();
      float value = spinBox->value();
      model->setData(index, value, Qt::EditRole);
      break;
    }
    case 1:  // mass
    case 3: {
      // ph
      QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
      doubleSpinBox->interpretText();
      float value = doubleSpinBox->value();
      model->setData(index, value, Qt::EditRole);
      break;
    }
    case 0:  // name
    default: {
      QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
      QString value = lineEdit->text();
      model->setData(index, value, Qt::EditRole);
      break;
    }
  }
}

void MaltTableDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                                             const QModelIndex& /* index */) const {
  editor->setGeometry(option.rect);
}
