// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "model/watersources.h"

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
  QString configDir;
  QString sourcesFile;
};

#endif // MAINMODEL_H
