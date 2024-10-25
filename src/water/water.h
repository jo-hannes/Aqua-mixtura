// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATER_H
#define WATER_H

#include "../common/meta.h"

#include <QJsonObject>
#include <QString>

class Water : public Meta {
 public:
  /**
   * @brief Enum to select a specific water value
   * Need to be sorted
   * 1. Volume
   * 2. Kationen
   * 3. Anionen
   * 4. Calculated
   */
  enum class Value {
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
  enum class Idx { JsonKey = 0, Unit };

  // clang-format off
  
  // Need to match order of WaterValue enum
  // Strings are: json key, unit, ...
  inline static const QString waterStrings[static_cast<int>(Value::Size)][2] = {
      {"Volume",            "l"},
      {"ca",                "mg/l"},
      {"mg",                "mg/l"},
      {"na",                "mg/l"},
      {"hco3",              "mg/l"},
      {"cl",                "mg/l"},
      {"so4",               "mg/l"},
      {"h2po4",             "mg/l"},
      {"c3h5o3",            "mg/l"},
      {"Restalkalitaet",    "°dH"},
      {"Gesamthaerte",      "°dH"},
      {"Carbonhaerte",      "°dH"},
      {"NichtCarbonhaerte", "°dH"},
      {"CaHaerte",          "°dH"},
      {"MgHaerte",          "°dH"},
      {"SO4ClVerhaeltnis",  ""}
  };
  // clang-format on

  /**
   * @brief Translatable Water strings
   */
  inline static QString translatableStrings[static_cast<int>(Value::Size)];

  Water();
  Water(QString name, float volume = 0, float calzium = 0, float magnesium = 0, float natrium = 0,
        float hydrogencarbonat = 0, float chlorid = 0, float sulfat = 0, float phosphat = 0, float lactat = 0);
  Water(const QJsonObject& json); /**< @brief Create Water from JSON */

  // getter und setter
  float get(Value what) const;       /**< @brief get requested value */
  void set(Value what, float value); /**< @brief set given type with value */

  // JSON conversion
  bool fromJson(const QJsonObject& json); /**< @brief Update Water from JSON */
  QJsonObject toJson() const;             /**< @brief convert this Water to JSON */
  QJsonObject profileToJson() const; /**< @brief convert this Water profile to JSON (A profile is without volume) */

  // Operators
  Water& operator+=(const Water& rhs);
  Water operator+(const Water& rhs);

 private:
  // calculations
  /**
   * @brief Berechnet die Gesammthärte
   * \f[
   * x [^{\circ}dH] = CaHaerte [^{\circ}dH] + MgHaerte [^{\circ}dH]
   * \f]
   */
  float calculateGesamthaerte() const;

  /**
   * @brief Berechnet die Calcium-Härte
   * \f[
   * x [^{\circ}dH] = 0,14 * Ca [mg/l]
   * \f]
   */
  float calculateCaHaerte() const;

  /**
   * @brief Berechnet die Magnesium-Härte
   * \f[
   * x [^{\circ}dH] = 0,23 * Mg [mg/l]
   * \f]
   */
  float calculateMgHaerte() const;

  /**
   * @brief Berechnet die Carbonhärte
   * \f{eqnarray*}{
   * x [^{\circ}dH] &=& Saeurekapazitaet * 2,8 \\
   * x [^{\circ}dH] &=& \frac{Hydrogencarbonat [mg/l]}{61,017}*2,8
   * \f}
   */
  float calculateCarbonhaerte() const;

  /**
   * @brief Berechnet die nicht Carbonhärte
   * \f[
   * x [^{\circ}dH] = GesammtHaerte [^{\circ}dH] - CarbonHaerte [^{\circ}dH]
   * \f]
   */
  float calculateNichtCarbonhaerte() const;

  /**
   * @brief Berechnet das Sulfat (SO4) zu Chlorid (Cl) Verhältnis
   * \f[
   * x = \frac{Sulfat (SO4)}{Chlorid (Cl)}
   * \f]
   */
  float calculateSO4ClVerhaeltnis() const;

  /**
   * @brief Berechnet die Restalkalität
   * \f[
   * x [^{\circ}dH] = CarbonHaerte [^{\circ}dH] - \frac{CaHaerte [^{\circ}dH]}{3,5} - \frac{MgHaerte [^{\circ}dH]}{7}
   * \f]
   */
  float calculateRestalkalitaet() const;

  float values[static_cast<int>(Value::LastAnion) + 1];
};

#endif  // WATER_H
