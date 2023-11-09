// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MALT_H
#define MALT_H

#include <QJsonObject>
#include <QString>

class Malt
{
  // Malzberechnung kommt später!
 public:
  Malt() {}
  Malt(QString name, float mass, float ebc, float ph) : name(name) {
    setMass(mass);
    setEbc(ebc);
    setPh(ph);
  }

  // getter und setter
  QString getName() const;
  void setName(const QString& newName);
  float getMass() const;       /**< @brief get malt amount in kg */
  void setMass(float newMass); /**< @brief set malt amount in kg */
  float getEbc() const;        /**< @brief get malt color in EBC */
  void setEbc(float newEbc);   /**< @brief set malt color in EBC */
  float getPh() const;         /**< @brief get pH of malt in purified water */
  void setPh(float newPh);     /**< @brief set pH of malt in purified water */

  enum type { upto200ebc, caramalz, roestmalz };

  // calculations
  /**
   * @brief Calclates the pH in purified water based on ebc
   *
   * All calculations are based on https://maischemalzundmehr.de/index.php?inhaltmitte=exp_maischph
   */
  static float calculatePh(float ebc, type type);

  /**
   * @brief Calclates the pH in purified water based on ebc for malts up to 200 EBC
   * \f[
   * x = -0,255 * ln(Malzfarbe [EBC]) + 6,156
   * \f]
   */
  static float calcualtePhUpTo200Ebc(float ebc);

  /**
   * @brief Calclates the pH in purified water based on ebc for Carmalz
   * \f[
   * x = -0,289 * ln(Malzfarbe [EBC]) + 6,069
   * \f]
   */
  static float calcualtePhCaramalz(float ebc);

  /**
   * @brief Calclates the pH in purified water based on ebc for Röstamlz
   * \f[
   * x = 4,65
   * \f]
   */
  static float calcualtePhRoestmalz();

  // JSON conversion
  static Malt fromJson(const QJsonObject& json); /**< @brief convert JSON to Malt */
  QJsonObject toJson() const;                    /**< @brief convert this Malt to JSON */

 private:
  QString name; /**< Name of malt */
  float mass;   /**< Amount of malt in kg */
  float ebc;    /**< Color of malt in EBC */
  float ph;     /**< pH of malt in purified water */
};

#endif // MALT_H
