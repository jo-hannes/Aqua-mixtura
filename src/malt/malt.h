// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MALT_H
#define MALT_H

#include "../common/meta.h"

#include <QJsonObject>
#include <QString>

class Malt : public Meta {
  // Malzberechnung kommt später!
 public:
  /**
   * @brief Constructor for Malt
   * @param name Malt name
   * @param mass Malt amount in kg
   * @param ebc Malt color in EBC
   * @param ph pH of malt in purified water
   */
  Malt(QString name = "", float mass = 0, float ebc = 0, float ph = 7);
  Malt(const QJsonObject& json); /**< @brief Create Malt from JSON */

  // getter und setter
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
  bool fromJson(const QJsonObject& json);        /**< @brief Update Malt from JSON */
  QJsonObject toJson() const;                    /**< @brief convert this Malt to JSON */

 private:
  float mass;   /**< Amount of malt in kg */
  float ebc;    /**< Color of malt in EBC */
  float ph;     /**< pH of malt in purified water */
};

#endif // MALT_H
