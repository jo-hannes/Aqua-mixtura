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
  // NOLINTNEXTLINE(*-magic-numbers)
  explicit Malt(QString name = "", double mass = 0, double ebc = 0, double ph = 7);
  explicit Malt(const QJsonObject& json); /**< @brief Create Malt from JSON */

  // getter und setter
  [[nodiscard]] double getMass() const; /**< @brief get malt amount in kg */
  void setMass(double newMass);         /**< @brief set malt amount in kg */
  [[nodiscard]] double getEbc() const;  /**< @brief get malt color in EBC */
  void setEbc(double newEbc);           /**< @brief set malt color in EBC */
  [[nodiscard]] double getPh() const;   /**< @brief get pH of malt in purified water */
  void setPh(double newPh);             /**< @brief set pH of malt in purified water */

  enum type { upto200ebc, caramalz, roestmalz };

  // calculations
  /**
   * @brief Calclates the pH in purified water based on ebc
   *
   * All calculations are based on https://maischemalzundmehr.de/index.php?inhaltmitte=exp_maischph
   */
  static double calculatePh(double ebc, type type);

  /**
   * @brief Calclates the pH in purified water based on ebc for malts up to 200 EBC
   * \f[
   * x = -0,255 * ln(Malzfarbe [EBC]) + 6,156
   * \f]
   */
  static double calcualtePhUpTo200Ebc(double ebc);

  /**
   * @brief Calclates the pH in purified water based on ebc for Carmalz
   * \f[
   * x = -0,289 * ln(Malzfarbe [EBC]) + 6,069
   * \f]
   */
  static double calcualtePhCaramalz(double ebc);

  /**
   * @brief Calclates the pH in purified water based on ebc for Röstamlz
   * \f[
   * x = 4,65
   * \f]
   */
  static double calcualtePhRoestmalz();

  // JSON conversion
  bool fromJson(const QJsonObject& json);   /**< @brief Update Malt from JSON */
  [[nodiscard]] QJsonObject toJson() const; /**< @brief convert this Malt to JSON */

 private:
  double mass; /**< Amount of malt in kg */
  double ebc;  /**< Color of malt in EBC */
  double ph;   /**< pH of malt in purified water */
};

#endif  // MALT_H
