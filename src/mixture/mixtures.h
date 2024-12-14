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
  explicit Mixtures(const QJsonObject& json, QObject* parent = nullptr); /**< @brief Create Mixtures from JSON */
  ~Mixtures();

  // JSON conversion
  bool fromJson(const QJsonObject& json);         /**< @brief Update Mixtures from JSON */
  [[nodiscard]] QJsonObject toJson() const;       /**< @brief convert this Mixtures to JSON */
  bool importMixture(const QString& path);        /**< @brief import and add a Mixture from JSON */
  bool exportMixture(const QString& path, int i); /**< @brief export a Mixture at index as JSON */

  Mixture& getMixture(int i);                  /**< @brief Get mixture at index */
  void updateMixture(Mixture& mixture, int i); /**< @brief Update mixture at index */
  void addMixture(Mixture& mixture);           /**< @brief Add a mixture */
  void deleteMixture(int i);                   /**< @brief Delete mixture at index */
  void show(int i);                            /**< @brief Show mixture window at index */
  bool closeAll();                             /**< @brief Try to close all mixture windows */

  // for QAbstractTableModel, see QT documentation for details
  // NOLINTBEGIN(modernize-use-nodiscard)
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  // NOLINTEND(modernize-use-nodiscard)

  // NOLINTBEGIN(*-non-private-member-variables-in-classes)
  // data bases are intended to be public
  WaterSources waterDb;         /**< @brief Model of water sources */
  AdditiveSettings additiveCfg; /**< @brief Additive config */
  Malts maltDb;                 /**< @brief Model of malts */
  Styles styleDb;               /**< @brief Model of styles */
  Settings settings;            /**< @brief Model of program settings */

  // NOLINTEND(*-non-private-member-variables-in-classes)

 public slots:
  void load();
  void save() const;

 private:
  QVector<MixtureWindow*> mixWindows; /**< @brief Array with pointer to mixture windows */
  QVector<Mixture> mixtures;
  Mixture noMixture;
};

#endif  // MIXTURES_H
