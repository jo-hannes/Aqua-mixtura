// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERSOURCESTABLEDELEGATE_H
#define WATERSOURCESTABLEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/**
 * @brief Delegate for improved edition of water sources
 */
class WaterSourcesTableDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  WaterSourcesTableDelegate(QObject* parent = nullptr);
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};

#endif  // WATERSOURCESTABLEDELEGATE_H
