// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "model/additive.h"
#include "model/watersources.h"

#include <QJsonObject>

class MainModel
{
 public:
  MainModel();

  void saveSources();
  void saveSources(const QString& path);

  void saveAdditive();
  void saveAdditive(const QString& path);

  WaterSources* sources;
  Additive* additive;
  // malts (for a later version)
  // (beer)styles
  // mixtures
 private:

  QString configDir;
  QString sourcesFile;
  QString additiveFile;
};

#endif // MAINMODEL_H
