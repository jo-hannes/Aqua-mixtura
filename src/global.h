// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

/**
 * Aqua Mixtura Globals
 */
namespace AM {
/**
 * @brief Enum to select a specific water value
 * Need to be sorted
 * 1. Volume
 * 2. Kationen
 * 3. Anionen
 * 4. Calculated
 */
enum class WaterValue {
  Volume = 0,
  Calcium,
  Magnesium,
  Natrium,
  LastCation = Natrium,
  Hydrogencarbonat,
  Chlorid,
  Sulfat,
  Phosphat,
  Lactat,
  LastAnion = Lactat,
  Restalkalitaet,
  Gesamthaerte,
  Carbonhaerte,
  NichtCarbonhaerte,
  CaHaerte,
  MgHaerte,
  SO4ClVerhaeltnis,
  Size
};

/**
 * @brief Index for 2nd dimension of waterStrings
 */
enum WaterIdx { JsonKey = 0, Unit };

// clang-format off

// Need to match order of WaterValue enum
// Strings are: json key, unit, ...
inline const QString waterStrings[static_cast<int>(WaterValue::Size)][2] = {
    {"Volume",            "l"},
    {"Calcium",           "mg/l"},
    {"Magnesium",         "mg/l"},
    {"Natrium",           "mg/l"},
    {"Hydrogencarbonat",  "mg/l"},
    {"Chlorid",           "mg/l"},
    {"Sulfat",            "mg/l"},
    {"Phosphat",          "mg/l"},
    {"Lactat",            "mg/l"},
    {"Restalkalitaet",    "°dH"},
    {"Gesamthaerte",      "°dH"},
    {"Carbonhaerte",      "°dH"},
    {"NichtCarbonhaerte", "°dH"},
    {"CaHaerte",          "°dH"},
    {"MgHaerte",          "°dH"},
    {"SO4ClVerhaeltnis",  ""}
  };
// clang-format on

}  // namespace AM

class Global
{
 public:
  Global();
};

#endif // GLOBAL_H
