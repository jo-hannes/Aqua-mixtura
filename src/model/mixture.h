// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MIXTURE_H
#define MIXTURE_H

#include "water.h"

#include <QVector>

class Mixture
{
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
