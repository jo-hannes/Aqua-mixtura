// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef META_H
#define META_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QUuid>

/**
 * @brief Class with common meta data
 */
class Meta {
 public:
  explicit Meta(QString name = "");
  explicit Meta(const QJsonObject& json);              /**< @brief Create Meta from JSON */
  [[nodiscard]] QString getName() const;               /**< @brief Get name of object */
  [[nodiscard]] QDateTime getCreationTime() const;     /**< @brief Get object creation time */
  [[nodiscard]] QDateTime getModificationTime() const; /**< @brief Get last modification time */
  [[nodiscard]] QString getUuid() const;               /**< @brief Get UUID as string */
  QString newUuid();                                   /**< @brief Generate new UUID and return it as string */
  void setName(const QString& newName);                /**< @brief Set object name */
  void updateEditTime();                               /**< @brief Update edit time to now */
  void updateCreationTime();                           /**< @brief Update creation time to now */
  bool fromJson(const QJsonObject& json);              /**< @brief Update meta data from JSON */
  void toJson(QJsonObject& json) const;                /**< @brief Append meta data to JSON */

 private:
  QString name;     /**< @brief Name of object */
  QDateTime create; /**< @brief Creation date of object */
  QDateTime edit;   /**< @brief Last time object was edited/saved */
  QUuid uuid;       /**< @brief Uniq id of object */
  // Maybe add author name
};

#endif  // META_H
