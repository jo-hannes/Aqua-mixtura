// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "model/watersources.h"

#include <QJsonObject>

class MainModel
{
 public:
  MainModel();

  void saveSources();
  void saveSources(const QString& path);

  WaterSources* sources;
  // additives
  // malts (for a later version)
  // (beer)styles
  // mixtures
 private:
  QJsonObject loadJson(const QString& path);                   /**< @brief Load JSON from file*/
  bool saveJson(const QString& path, const QJsonObject& json); /**< @brief Save JSON to file */

  QString configDir;
  QString sourcesFile;
};

#endif // MAINMODEL_H
