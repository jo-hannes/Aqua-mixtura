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
  enum class StringIdx { JsonKey = 0, Formula };

  /**
   * @brief Additive specific strings
   */
  inline static const QString strings[static_cast<int>(Value::Size)][2] = {
      {"c3h6o3", "C₃H₆O₃"},    {"hcl", "HCl"},          {"h2so4", "H₂SO₄"}, {"h3po4", "H₃PO₄"},
      {"caso4", "CaSO₄ 2H₂O"}, {"cacl2", "CaCl₂ 2H₂O"}, {"nacl", "NaCl"},   {"nahco3", "NaHCO₃"},
      {"mgcl2", "MgCl₂ 6H₂O"}, {"mgso4", "MgSO₄ 7H₂O"}, {"caco3", "CaCO₃"},
  };

  /**
   * @brief Translatable Additive strings
   * These cant be const or translations wont work here.
   */
  inline static QString translatableStrings[static_cast<int>(Value::Size)];

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
  Water operator+(const Water& rhs) const;

  bool isChanged() const; /**< @brief True if changes not saved */

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark unsaved/saved */
  bool changed;                  /**< @brief True if changed but not saved */

  float amount[static_cast<int>(Value::Size)]; /**< @brief Effective amount in g */

  // clang-format off
  /**
   * @brief Calculation Matrix for effect of additives on water values
   *
   * Amount of mg added or removed per added g of additive
   */
  inline static const float calculationMatrix[static_cast<int>(Value::Size)][static_cast<int>(Water::Value::LastAnion) + 1] = {
// Volume Calcium Magnesium Natrium Hydrogencarbonat Chlorid  Sulfat Phosphat  Lactat
    {  0,      0,        0,      0,         -677.37,      0,      0,       0, 988.81}, // c3h6o3
    {  0,      0,        0,      0,        -1673.60, 972.35,      0,       0,      0}, // hcl
    {  0,      0,        0,      0,        -1244.31,      0, 979.44,       0,      0}, // h2so4
    {  0,      0,        0,      0,         -622.65,      0,      0,  989.71,      0}, // h3po4
    {  0, 232.78,        0,      0,               0,      0, 557.91,       0,      0}, // caso4
    {  0, 272.60,        0,      0,               0, 482.25,      0,       0,      0}, // cacl2
    {  0,      0,        0, 393.39,               0, 606.61,      0,       0,      0}, // nacl
    {  0,      0,        0, 273.67,          726.33,      0,      0,       0,      0}, // nahco3
    {  0,      0,   119.55,      0,               0, 348.75,      0,       0,      0}, // mgcl2
    {  0,      0,    98.61,      0,               0,      0, 389.71,       0,      0}, // mgso4
    {  0, 400.42,        0,      0,         1219.22,      0,      0,       0,      0}  // caco3
  };
  // clang-format on
};

#endif  // ADDITIVE_H
