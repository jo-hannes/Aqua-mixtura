// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "mixture/mixtures.h"

#include <QJsonObject>
#include <QObject>

/**
 * @brief MainModel class holding the whole data model
 */
class MainModel : public QObject {
  Q_OBJECT

 public:
  MainModel();

 public slots:
  void load(); /**< @brief Load all data from JSON files */
  void save(); /**< @brief Save all data to JSON files */

 public:
  Mixtures mixtures; /**< @brief Model of mixtures */
};

#endif  // MAINMODEL_H
