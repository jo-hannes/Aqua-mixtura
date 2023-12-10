// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATER_H
#define WATER_H

#include <QJsonObject>
#include <QString>

class Water {
 public:
  /**
   * @brief The Value enum
   * Need to be sorted
   * 1. Volume
   * 2. Kationen
   * 3. Anionen
   * 4. Calculated
   */
  enum class Type {
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
    TypeSize
  };

  Water();
  Water(QString name, float volume = 0, float calzium = 0, float magnesium = 0, float natrium = 0,
        float hydrogencarbonat = 0, float chlorid = 0, float sulfat = 0, float phosphat = 0, float lactat = 0);

  // getter und setter
  QString getName() const;
  void setName(const QString& newName);
  float get(Type what) const;                          /**< @brief get requested value */
  void set(Type what, float value);                    /**< @brief set given type with value */

  // JSON conversion
  static Water fromJson(const QJsonObject& json); /**< @brief convert JSON to Water */
  QJsonObject toJson() const;                     /**< @brief convert this Water to JSON */
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

  QString name;
  float values[static_cast<int>(Type::LastAnion) + 1];

  // strings
  // Need to match order of Value enum
  // Strings are: unit, json key. For values not storred in json leave key empty
  QString strings[static_cast<int>(Type::TypeSize)][2] = {{"l", "Volume"},
                                                          {"mg/l", "Calzium"},
                                                          {"mg/l", "Magnesium"},
                                                          {"mg/l", "Natrium"},
                                                          {"mg/l", "Hydrogencarbonat"},
                                                          {"mg/l", "Chlorid"},
                                                          {"mg/l", "Sulfat"},
                                                          {"mg/l", "Phosphat"},
                                                          {"mg/l", "Lactat"},
                                                          {"°dH", ""},
                                                          {"°dH", ""},
                                                          {"°dH", ""},
                                                          {"°dH", ""},
                                                          {"°dH", ""},
                                                          {"", ""}};
};

#endif // WATER_H
