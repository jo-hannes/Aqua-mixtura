// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include "additive/additive.h"
#include "malt/malts.h"
#include "mixture/mixtures.h"
#include "style/styles.h"
#include "water/watersources.h"

#include <QJsonObject>

/**
 * @brief MainModel class holding the whole data model
 */
class MainModel
{
 public:
  MainModel();

  void load();         /**< @brief Load all data from JSON files */
  void save();         /**< @brief Save all data to JSON files */
  void loadSources();  /**< @brief Load water sources data from JSON file */
  void saveSources();  /**< @brief Save water sources data to JSON file */
  void loadAdditive(); /**< @brief Load additive data from JSON file */
  void saveAdditive(); /**< @brief Save additive data to JSON file */
  void loadMalts();    /**< @brief Load malts data from JSON file */
  void saveMalts();    /**< @brief Save malts data to JSON file */
  void loadStyles();   /**< @brief Load styles data from JSON file */
  void saveStyles();   /**< @brief Save styles data to JSON file */
  void loadMixtures(); /**< @brief Load mixtures data from JSON file */
  void saveMixtures(); /**< @brief Save mixtures data to JSON file */

  WaterSources* sources; /**< @brief Model of water sources */
  Additive* additive;    /**< @brief Model of additives */
  Malts* malts;          /**< @brief Model of malts */
  Styles* styles;        /**< @brief Model of styles */
  Mixtures* mixtures;    /**< @brief Model of mixtures */

 private:
  QString sourcesFile;  /**< @brief Path to water sources JSON */
  QString additiveFile; /**< @brief Path to additive JSON */
  QString maltsFile;    /**< @brief Path to malts JSON */
  QString stylesFile;   /**< @brief Path to styles JSON */
  QString mixturesFile; /**< @brief Path to mixtures JSON */
};

#endif // MAINMODEL_H
