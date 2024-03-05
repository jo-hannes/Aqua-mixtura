// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLES_H
#define STYLES_H

#include "style.h"

#include <QAbstractListModel>
#include <QString>
#include <QVector>

class Styles : public QAbstractListModel {
  Q_OBJECT

 public:
  Styles();
  Styles(const QJsonObject& json); /**< @brief Create Styles from JSON */
  ~Styles();

  // JSON conversion
  bool fromJson(const QJsonObject& json);             /**< @brief Update Styles from JSON */
  QJsonObject toJson() const;                         /**< @brief convert Styles to JSON */
  bool importStyle(const QString& path);              /**< @brief import and add a Style from JSON */
  bool exportStyle(const QString& path, qsizetype i); /**< @brief export a Style at index as JSON */

  Style* getStyle(qsizetype i);  /**< @brief Get style at index */
  void addStyle(Style* style);   /**< @brief Add a style */
  void deleteStyle(qsizetype i); /**< @brief Delete style at index */
  void setSaved();               /**< @brief Mark all styles as saved */

  // for QAbstractListModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
  Qt::ItemFlags flags(const QModelIndex& index) const;

 signals:
  void unsavedStyles(bool unsaved); /**< @brief Signal emitted if save state of Styles change */

 private:
  void clear();                  /**< @brief Clear/remove all Styles*/
  void setUnsaved(bool unsaved); /**< @brief Mark styles unsaved/saved */

  QVector<Style*> styles;
  bool unsavedChanges; /**< @brief True if styles where changed but not saved */
  Style* noStyle;      /**< @brief Empty style used to return no style on error */
};

#endif // STYLES_H
