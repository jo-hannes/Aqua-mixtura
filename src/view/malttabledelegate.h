#ifndef MALTTABLEDELEGATE_H
#define MALTTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>

/**
 * @brief Delegate to use a spin box for entering the EBC value of a malt
 */
class MaltTableDelegate : public QStyledItemDelegate {
  Q_OBJECT
 public:
  MaltTableDelegate(QObject* parent = nullptr);
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
  void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const override;
};

#endif  // MALTTABLEDELEGATE_H
