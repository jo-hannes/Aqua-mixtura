// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLETABLEDELEGATE_H
#define STYLETABLEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/**
 * @brief Delegate for improved edition of beer styles
 */
class StyleTableDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  StyleTableDelegate(QObject* parent = nullptr);
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif  // STYLETABLEDELEGATE_H
