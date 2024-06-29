// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef SETTINGS_H
#define SETTINGS_H

#include "limits.h"

#include "../common/meta.h"

#include <QJsonObject>
#include <QObject>

/**
 * @brief Programm global settings
 */
class Settings : public QObject, public Meta {
  Q_OBJECT
 public:
  explicit Settings(QObject* parent = nullptr);

  Settings(const QJsonObject& json);      /**< @brief Construct Settings from JSON */
  bool fromJson(const QJsonObject& json); /**< @brief Update Settings from JSON */
  QJsonObject toJson() const;             /**< @brief convert Settings to JSON */

 public slots:
  void load(); /**< @brief Load all data from JSON files */
  void save(); /**< @brief Save all data to JSON files */

 public:
  Limits waterSettings;
};

#endif  // SETTINGS_H
