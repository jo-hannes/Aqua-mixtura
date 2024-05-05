// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVE_H
#define ADDITIVE_H

#include "../common/meta.h"

#include <QJsonObject>
#include <QObject>

/**
 * @brief Water additives
 */
class Additive : public QObject, public Meta {
  Q_OBJECT

 public:

  /**
   * @brief Index for additive type
   */
  enum class Value {
    c3h6o3 = 0,
    hcl,
    h2so4,
    h3po4,
    lastLiquid = h3po4,
    caso4,
    cacl2,
    nacl,
    nahco3,
    mgcl2,
    mgso4,
    caco3,
    Size
  };

  /**
   * @brief Index for 2nd dimension of strings
   */
  enum class StringIdx { JsonKey = 0, Formula, Description };

  /**
   * @brief Additive specific strings
   */
  inline static const QString strings[static_cast<int>(Value::Size)][3] = {
      {"c3h6o3", "C₃H₆O₃", QObject::tr("Milchsäure")},
      {"hcl", "HCl", QObject::tr("Salzsäure")},
      {"h2so4", "H₂SO₄", QObject::tr("Schwefelsäure")},
      {"h3po4", "H₃PO₄", QObject::tr("Phosphorsäure")},
      {"caso4", "CaSO₄", QObject::tr("Calciumsulfat (Braugips)")},
      {"cacl2", "CaCl₂", QObject::tr("Calciumchlorid")},
      {"nacl", "NaCl", QObject::tr("Natriumchlorid (Kochsalz)")},
      {"nahco3", "NaHCO₃", QObject::tr("Natriumhydrogencarbonat (Natron)")},
      {"mgcl2", "MgCl₂", QObject::tr("Magnesiumchlorid")},
      {"mgso4", "MgSO₄", QObject::tr("Magnesiumsulfat")},
      {"caco3", "CaCO₃", QObject::tr("Calciumcarbonat")},
  };

  Additive();
  Additive(const QJsonObject& json); /**< @brief Create Additive from JSON */

  // setter and getter
  float get(Value what) const;                    /**< @brief get requested amount */
  void set(Value what, float value);              /**< @brief set given type with amount */
  float getConcentration(Value what) const;       /**< @brief get requested concentration */
  void setConcentration(Value what, float value); /**< @brief set given type with concentration */
  bool isEnabled(Value what) const;               /**< @brief get enable status */
  void enable(Value what, bool enable);           /**< @brief set enable status */

  // JSON conversion
  bool fromJson(const QJsonObject& json);            /**< @brief Update Additive from JSON */
  QJsonObject toJson() const;                        /**< @brief convert this Additive to JSON */

 signals:
  void dataModified();

 private:
  bool enabled[static_cast<int>(Value::Size)];                  /**< @brief Enable status */
  float amount[static_cast<int>(Value::Size)];                  /**< @brief Amount in g or ml */
  float concentration[static_cast<int>(Value::lastLiquid) + 1]; /**< @brief Concentration in % */
};

#endif  // ADDITIVE_H
