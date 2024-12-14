// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MALTTABLEDELEGATE_H
#define MALTTABLEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/**
 * @brief Delegate to use a spin box for entering the EBC value of a malt
 */
class MaltTableDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  explicit MaltTableDelegate(QObject* parent = nullptr);
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
  void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const override;
};

#endif  // MALTTABLEDELEGATE_H
