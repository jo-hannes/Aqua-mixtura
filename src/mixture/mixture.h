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
#include <utility>

class Mixture : public Meta {
 public:
  Mixture() = default;

  // JSON conversion
  explicit Mixture(QString path);            /**< @brief Create Mixture from JSON file */
  explicit Mixture(const QJsonObject& json); /**< @brief Create Mixture from JSON */
  bool fromJson(const QJsonObject& json);    /**< @brief Update Mixture from JSON */
  [[nodiscard]] QJsonObject toJson() const;  /**< @brief convert this Mixture to JSON */

  // load and save
  void resetPath();                      /**< @brief Regenerate path from UUID */
  [[nodiscard]] QString getPath() const; /**< @brief Get storage path */
  void load();                           /**< @brief Load fixture from stored path */
  void save() const;                     /**< @brief Save mixture and return storage path */

  [[nodiscard]] std::pair<Water, double> calc() const;

  // NOLINTBEGIN(*-non-private-member-variables-in-classes)
  // they are intended to be public
  WaterSources* waters{new WaterSources()};
  Additive* additive{new Additive()};
  Malts* malts{new Malts()};
  Style* style{new Style()};
  // NOLINTEND(*-non-private-member-variables-in-classes)

 private:
  QString path;
};

#endif  // MIXTURE_H
