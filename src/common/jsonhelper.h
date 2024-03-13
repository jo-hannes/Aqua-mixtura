// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QJsonObject>

/**
 * @brief Class with common JSON functions
 */
class JsonHelper
{
 public:
  JsonHelper() = delete;

  /**
   * @brief loadFile Load JSON from File
   * @param path Path to file
   * @return loaded JSON Object
   */
  static QJsonObject loadFile(const QString& path);

  /**
   * @brief saveFile Save JSON to file
   * @param path Path to file
   * @param json JSON data
   * @return Returns true on success otherwise false
   */
  static bool saveFile(const QString& path, const QJsonObject& json);

  /**
   * @brief Merge two JSON objects into one
   * @param into Other object will be merged into this object
   * @param from Data from this object is merged into other object
   */
  static void mergeJson(QJsonObject& into, const QJsonObject from);
};

#endif // JSONHELPER_H
