// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATER_H
#define WATER_H

#include <QJsonObject>
#include <QString>

class Water {
 public:
  Water();
  Water(QString name, float menge = 0, float calzium = 0, float magnesium = 0, float natrium = 0,
        float hydrogencarbonat = 0, float chlorid = 0, float sulfat = 0, float phosphat = 0, float lactat = 0);

  // getter und setter
  QString getName() const;
  void setName(const QString& newName);
  float getVolume() const;                             /**< @brief get Water volume in liter */
  void setVolume(float newMenge);                      /**< @brief set Water volume in liter */
  float getCalzium() const;                            /**< @brief set Calzium in mg/l */
  void setCalzium(float newCalzium);                   /**< @brief get Calzium in mg/l */
  float getMagnesium() const;                          /**< @brief set Magnesium in mg/l */
  void setMagnesium(float newMagnesium);               /**< @brief get Magnesium in mg/l */
  float getNatrium() const;                            /**< @brief set Natrium in mg/l */
  void setNatrium(float newNatrium);                   /**< @brief get Natrium in mg/l */
  float getHydrogencarbonat() const;                   /**< @brief set Hydrogencarbonat in mg/l */
  void setHydrogencarbonat(float newHydrogencarbonat); /**< @brief get Hydrogencarbonat in mg/l */
  float getChlorid() const;                            /**< @brief set Chlorid in mg/l */
  void setChlorid(float newChlorid);                   /**< @brief get Chlorid in mg/l */
  float getSulfat() const;                             /**< @brief set Sulfat in mg/l */
  void setSulfat(float newSulfat);                     /**< @brief get Sulfat in mg/l */
  float getPhosphat() const;                           /**< @brief set Phosphat in mg/l */
  void setPhosphat(float newPhosphat);                 /**< @brief get Phosphat in mg/l */
  float getLactat() const;                             /**< @brief set Lactat in mg/l */
  void setLactat(float newLactat);                     /**< @brief get Lactat in mg/l */

  // calculations
  /**
   * @brief Berechnet die Gesammthärte
   * \f[
   * x [^{\circ}dH] = CaHaerte [^{\circ}dH] + MgHaerte [^{\circ}dH]
   * \f]
   */
  float getGesamthaerte() const;

  /**
   * @brief Berechnet die Calcium-Härte
   * \f[
   * x [^{\circ}dH] = 0,14 * Ca [mg/l]
   * \f]
   */
  float getCaHaerte() const;

  /**
   * @brief Berechnet die Magnesium-Härte
   * \f[
   * x [^{\circ}dH] = 0,23 * Mg [mg/l]
   * \f]
   */
  float getMgHaerte() const;

  /**
   * @brief Berechnet die Carbonhärte
   * \f{eqnarray*}{
   * x [^{\circ}dH] &=& Saeurekapazitaet * 2,8 \\
   * x [^{\circ}dH] &=& \frac{Hydrogencarbonat [mg/l]}{61,017}*2,8
   * \f}
   */
  float getCarbonhaerte() const;

  /**
   * @brief Berechnet die nicht Carbonhärte
   * \f[
   * x [^{\circ}dH] = GesammtHaerte [^{\circ}dH] - CarbonHaerte [^{\circ}dH]
   * \f]
   */
  float getNichtCarbonhaerte() const;

  /**
   * @brief Berechnet das Sulfat (SO4) zu Chlorid (Cl) Verhältnis
   * \f[
   * x = \frac{Sulfat (SO4)}{Chlorid (Cl)}
   * \f]
   */
  float getSO4ClVerhaeltnis() const;

  /**
   * @brief Berechnet die Restalkalität
   * \f[
   * x [^{\circ}dH] = CarbonHaerte [^{\circ}dH] - \frac{CaHaerte [^{\circ}dH]}{3,5} - \frac{MgHaerte [^{\circ}dH]}{7}
   * \f]
   */
  float getRestalkalitaet() const;

  // JSON conversion
  static Water fromJson(const QJsonObject& json); /**< @brief convert JSON to Water */
  QJsonObject toJson() const;                     /**< @brief convert this Water to JSON */
  QJsonObject profileToJson() const; /**< @brief convert this Water profile to JSON (A profile is without volume) */

  // Operators
  Water& operator+=(const Water& rhs);
  Water operator+(const Water& rhs);

 protected:
  QString name;
  float volume;           /**< Water volume in liter */
  float calzium;          /**< Calzium in milli Gramm pro Liter */
  float magnesium;        /**< Magnesium in milli Gramm pro Liter */
  float natrium;          /**< Natrium in milli Gramm pro Liter */
  float hydrogencarbonat; /**< Hydrogencarbonat in milli Gramm pro Liter */
  float chlorid;          /**< Chlorid in milli Gramm pro Liter */
  float sulfat;           /**< Sulfat in milli Gramm pro Liter */
  float phosphat;         /**< Phosphat in milli Gramm pro Liter */
  float lactat;           /**< Lactat in milli Gramm pro Liter */
};

#endif // WATER_H
