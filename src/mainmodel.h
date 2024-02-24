// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "additive/additive.h"
#include "malt/malts.h"
#include "mixture/mixtures.h"
#include "style/styles.h"
#include "water/watersources.h"

#include <QJsonObject>

class MainModel
{
 public:
  MainModel();

  void saveSources();
  void saveSources(const QString& path);

  void saveAdditive();
  void saveAdditive(const QString& path);

  void saveMalts();
  void saveMalts(const QString& path);

  void loadStyles();
  void saveStyles();
  void saveStyles(const QString& path);

  void loadMixtures();
  void saveMixtures();

  WaterSources* sources;
  Additive* additive;
  Malts* malts;
  Styles* styles;
  Mixtures* mixtures;

 private:
  QString configDir;
  QString sourcesFile;
  QString additiveFile;
  QString maltsFile;
  QString stylesFile;
  QString mixturesFile;
};

#endif // MAINMODEL_H
