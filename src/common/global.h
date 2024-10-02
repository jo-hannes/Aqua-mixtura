// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
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
enum WaterIdx { JsonKey = 0, Unit, Description };

// clang-format off

// Need to match order of WaterValue enum
// Strings are: json key, unit, ...
inline static const QString waterStrings[static_cast<int>(WaterValue::Size)][3] = {
    {"Volume",            "l",    QObject::tr("Volumen")},
    {"ca",                "mg/l", QObject::tr("Calcium")},
    {"mg",                "mg/l", QObject::tr("Magnesium")},
    {"na",                "mg/l", QObject::tr("Natrium")},
    {"hco3",              "mg/l", QObject::tr("Hydrogencarbonat")},
    {"cl",                "mg/l", QObject::tr("Chlorid")},
    {"so4",               "mg/l", QObject::tr("Sulfat")},
    {"h2po4",             "mg/l", QObject::tr("Phosphat")},
    {"c3h5o3",            "mg/l", QObject::tr("Lactat")},
    {"Restalkalitaet",    "°dH",  QObject::tr("Restalkalität")},
    {"Gesamthaerte",      "°dH",  QObject::tr("Gesamthärte")},
    {"Carbonhaerte",      "°dH",  QObject::tr("Carbonhärte")},
    {"NichtCarbonhaerte", "°dH",  QObject::tr("Nichtcarbonhärte")},
    {"CaHaerte",          "°dH",  QObject::tr("Ca-Härte")},
    {"MgHaerte",          "°dH",  QObject::tr("Mg-Härte")},
    {"SO4ClVerhaeltnis",  "",     QObject::tr("SO₄/Cl-Verhältnis")}
  };
// clang-format on

}  // namespace AM

class Global
{
 public:
  Global();
};

#endif // GLOBAL_H
