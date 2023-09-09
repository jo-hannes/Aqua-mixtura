// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mixture.h"

Mixture::Mixture()
{
  
}

void Mixture::AddWater(Water water) {
  waters.append(water);
}

void Mixture::ClearWater() {
  waters.clear();
}

void Mixture::RemoveWaterAt(qsizetype i) {
  waters.removeAt(i);
}

Water Mixture::calc() {
  // start with empty water
  Water result("Result");

  // mix water sources
  for (Water& w : waters) {
    result += w;
  }

  // TODO other calculation

  return result;
}
