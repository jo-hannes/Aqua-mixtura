// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVE_H
#define ADDITIVE_H

#include "../common/meta.h"
#include "../water/water.h"

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

  // JSON conversion
  bool fromJson(const QJsonObject& json);            /**< @brief Update Additive from JSON */
  QJsonObject toJson() const;                        /**< @brief convert this Additive to JSON */

  /**
   * @brief Additive::operator + calculates effect of additive to water values
   */
  Water operator+(const Water& rhs);

 signals:
  void dataModified();

 private:
  float amount[static_cast<int>(Value::Size)]; /**< @brief Effective amount in g */

  float getWeight(Value what) const; /**< @brief Get net weight of additive in gramms */

  // clang-format off
  /**
   * @brief Calculation Matrix for effect of additives on water values
   *
   * Amount of mg added or removed per added g of additive
   */
  inline static const float calculationMatrix[static_cast<int>(Value::Size)][static_cast<int>(AM::WaterValue::LastAnion) + 1] = {
// Volume Calcium Magnesium Natrium Hydrogencarbonat Chlorid  Sulfat Phosphat Lactat
    {  0,      0,        0,      0,         -677.18,      0,      0,       0,  999}, // c3h6o3
    {  0,      0,        0,      0,        -1673.07, 972.60,      0,       0,    0}, // hcl
    {  0,      0,        0,      0,        -1243.88,      0, 979.59,       0,    0}, // h2so4
    {  0,      0,        0,      0,         -622.45,      0,      0,  972.37,    0}, // h3po4
    {  0, 272.60,        0,      0,               0,      0, 557.93,       0,    0}, // caso4
    {  0, 272.60,        0,      0,               0, 482.28,      0,       0,    0}, // cacl2
    {  0,      0,        0, 393.39,               0, 606.66,      0,       0,    0}, // nacl
    {  0,      0,        0, 273.66,          726.34,      0,      0,       0,    0}, // nahco3
    {  0,      0,    59.78,      0,               0, 174.39,      0,       0,    0}, // mgcl2
    {  0,      0,    98.59,      0,               0,      0, 389.73,       0,    0}, // mgso4
    {  0,    400,        0,      0,            1220,      0,      0,       0,    0}  // caco3
  };
  // clang-format on
};

#endif  // ADDITIVE_H
