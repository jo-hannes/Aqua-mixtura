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
  explicit Styles(const QJsonObject& json); /**< @brief Create Styles from JSON */
  ~Styles();

  // JSON conversion
  bool fromJson(const QJsonObject& json);                   /**< @brief Update Styles from JSON */
  [[nodiscard]] QJsonObject toJson() const;                 /**< @brief convert Styles to JSON */
  bool importStyle(const QString& path);                    /**< @brief import and add a Style from JSON */
  [[nodiscard]] bool exportStyle(const QString& path, qsizetype i) const; /**< @brief export a Style at index as JSON */

  Style* getStyle(qsizetype i);  /**< @brief Get style at index */
  void addStyle(Style* style);   /**< @brief Add a style */
  void deleteStyle(int i);       /**< @brief Delete style at index */

  [[nodiscard]] bool isChanged() const; /**< @brief True if changes not saved */

  // for QAbstractListModel, see QT documentation for details
  // NOLINTBEGIN(modernize-use-nodiscard)
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  // NOLINTEND(modernize-use-nodiscard)

 public slots:
  void load();
  void save();

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark unsaved/saved */
  bool changed{false};           /**< @brief True if changed but not saved */

  void clear(); /**< @brief Clear/remove all Styles*/

  QVector<Style*> styles;
  Style* noStyle; /**< @brief Empty style used to return no style on error */
};

#endif  // STYLES_H
