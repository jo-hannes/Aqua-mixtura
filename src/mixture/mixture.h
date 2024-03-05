// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTURE_H
#define MIXTURE_H

#include "../common/meta.h"
#include "../water/water.h"

#include <QVector>

class Mixture : public Meta {
 public:
  Mixture(QString name = "");

  // JSON conversion
  Mixture(const QJsonObject& json);       /**< @brief Create Mixture from JSON */
  bool fromJson(const QJsonObject& json); /**< @brief Update Mixture from JSON */
  QJsonObject toJson() const;             /**< @brief convert this Mixture to JSON */

  // TODO rethink the following functions
  void AddWater(Water water);
  void ClearWater();
  void RemoveWaterAt(qsizetype i);

  Water calc();

 private:
  QVector<Water> waters;
};

#endif // MIXTURE_H
