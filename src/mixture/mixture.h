// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTURE_H
#define MIXTURE_H

#include "../additive/additive.h"
#include "../common/meta.h"
#include "../malt/malts.h"
#include "../style/style.h"
#include "../water/watersources.h"

#include <QSharedPointer>

class Mixture : public Meta {
 public:
  Mixture();

  // JSON conversion
  Mixture(QString path);                  /**< @brief Create Mixture from JSON file */
  Mixture(const QJsonObject& json);       /**< @brief Create Mixture from JSON */
  bool fromJson(const QJsonObject& json); /**< @brief Update Mixture from JSON */
  QJsonObject toJson() const;             /**< @brief convert this Mixture to JSON */

  // load and save
  void resetPath();        /**< @brief Regenerate path from UUID */
  QString getPath() const; /**< @brief Get storage path */
  void load();             /**< @brief Load fixture from stored path */
  void save() const;       /**< @brief Save mixture and return storage path */

  Water calc();

  // private:
  WaterSources* waters{new WaterSources()};
  Additive* additive{new Additive()};
  Malts* malts{new Malts()};
  Style* style{new Style()};

 private:
  QString path;
};

#endif  // MIXTURE_H
