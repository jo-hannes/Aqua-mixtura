// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef SETTINGSTABLEDELEGATE_H
#define SETTINGSTABLEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

/**
 * @brief Delegate for better editing of settings
 */
class SettingsTableDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  SettingsTableDelegate(QObject* parent = nullptr);
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif  // SETTINGSTABLEDELEGATE_H
