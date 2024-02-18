// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTURE_H
#define MIXTURE_H

#include "meta.h"
#include "water.h"

#include <QVector>

class Mixture : public Meta {
 public:
  Mixture();
  void AddWater(Water water);
  void ClearWater();
  void RemoveWaterAt(qsizetype i);

  Water calc();

 private:
  QVector<Water> waters;
};

#endif // MIXTURE_H
