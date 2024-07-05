// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTURES_H
#define MIXTURES_H

#include "mixture.h"
#include "mixturewindow.h"

#include "../additive/additivesettings.h"
#include "../malt/malts.h"
#include "../settings/settings.h"
#include "../style/styles.h"
#include "../water/watersources.h"

#include <QAbstractTableModel>
#include <QObject>
#include <QVector>

class Mixtures : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit Mixtures(QObject* parent = nullptr);
  Mixtures(const QJsonObject& json, QObject* parent = nullptr); /**< @brief Create Mixtures from JSON */
  ~Mixtures();

  // JSON conversion
  bool fromJson(const QJsonObject& json);               /**< @brief Update Mixtures from JSON */
  QJsonObject toJson() const;                           /**< @brief convert this Mixtures to JSON */
  bool importMixture(const QString& path);              /**< @brief import and add a Mixture from JSON */
  bool exportMixture(const QString& path, qsizetype i); /**< @brief export a Mixture at index as JSON */

  Mixture& getMixture(qsizetype i);                  /**< @brief Get mixture at index */
  void updateMixture(Mixture& mixture, qsizetype i); /**< @brief Update mixture at index */
  void addMixture(Mixture& mixture);                 /**< @brief Add a mixture */
  void deleteMixture(qsizetype i);                   /**< @brief Delete mixture at index */
  void show(qsizetype i);                            /**< @brief Show mixture window at index */

  // for QAbstractTableModel, see QT documentation for details
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  // data bases
  WaterSources* waterDb;        /**< @brief Model of water sources */
  AdditiveSettings additiveCfg; /**< @brief Additive config */
  Malts* maltDb;                /**< @brief Model of malts */
  Styles* styleDb;              /**< @brief Model of styles */
  Settings settings;            /**< @brief Model of programm settings */

 public slots:
  void load();
  void save();

 private:
  QVector<MixtureWindow*> mixWindows; /**< @brief Array with pointer to mixture windows */
  QVector<Mixture> mixtures;
  Mixture noMixture;
};

#endif  // MIXTURES_H
