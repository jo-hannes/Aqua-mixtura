// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATER_H
#define WATER_H

#include "waterprofile.h"

#include <QString>

class Water : public WaterProfile {
 public:
  Water();
  Water(WaterProfile &profile, float menge = 0);
  Water(QString name, float menge = 0, float calzium = 0, float magnesium = 0, float natrium = 0,
        float hydrogencarbonat = 0, float chlorid = 0, float sulfat = 0, float phosphat = 0, float lactat = 0);

  // getter und setter
  float getMenge() const;        /**< @brief get Wassermenge in Liter */
  void setMenge(float newMenge); /**< @brief set Wassermenge in Liter */

  // Operators
  Water& operator+=(const Water& rhs);
  Water operator+(const Water& rhs);

 protected:
  float menge;            /**< Wassermenge in mikro Liter */
};

#endif // WATER_H
