// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef META_H
#define META_H

#include <QDateTime>
#include <QJsonObject>
#include <QString>

/**
 * @brief Class with common meta data
 */
class Meta {
 public:
  Meta(QString name = "");
  QString getName() const;                /**< @brief Get name of object */
  QDateTime getCreationTime() const;      /**< @brief Get object creation time */
  QDateTime getModificationTime() const;  /**< @brief Get last modification time */
  void setName(const QString& newName);   /**< @brief Set object name */
  void edited();                          /**< @brief Update edit time to now */
  void fromJson(const QJsonObject& json); /**< @brief Read meta data from JSON */
  void toJson(QJsonObject& json) const;   /**< @brief Append meta data to JSON */

 private:
  QString name;     /**< @brief Name of object */
  QDateTime create; /**< @brief Creation date of object */
  QDateTime edit;   /**< @brief Last time object was edited/saved */
  // Maybe add author name
  // Maybe add UUID
};

#endif  // META_H
