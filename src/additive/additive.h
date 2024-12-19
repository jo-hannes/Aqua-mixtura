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

  // clang-format off
  // NOLINTBEGIN(cert-err58-cpp): it is very unlikely and program will not run without this strings
  inline static const std::array<QString, static_cast<int>(Value::Size)> strJsonKey{
      "c3h6o3", "hcl", "h2so4", "h3po4", "caso4",      "cacl2",      "nacl", "nahco3", "mgcl2",      "mgso4",      "caco3"};
  inline static const std::array<QString, static_cast<int>(Value::Size)> strFormula{
      "C₃H₆O₃", "HCl", "H₂SO₄", "H₃PO₄", "CaSO₄ 2H₂O", "CaCl₂ 2H₂O", "NaCl", "NaHCO₃", "MgCl₂ 6H₂O", "MgSO₄ 7H₂O", "CaCO₃"};
  // NOLINTEND(cert-err58-cpp)
  // clang-format on

  /**
   * @brief Translatable Additive strings
   * These cant be const or translations wont work here.
   */
  inline static std::array<QString, static_cast<int>(Value::Size)> strTranslate;

  Additive();
  explicit Additive(const QJsonObject& json); /**< @brief Create Additive from JSON */

  // setter and getter
  [[nodiscard]] double get(Value what) const; /**< @brief get requested amount */
  void set(Value what, double value);         /**< @brief set given type with amount */

  // JSON conversion
  bool fromJson(const QJsonObject& json);   /**< @brief Update Additive from JSON */
  [[nodiscard]] QJsonObject toJson() const; /**< @brief convert this Additive to JSON */

  /**
   * @brief Additive::operator + calculates effect of additive to water values
   */
  Water operator+(const Water& rhs) const;

  [[nodiscard]] bool isChanged() const; /**< @brief True if changes not saved */

 signals:
  void dataModified();

 private:
  void setChanged(bool changed); /**< @brief Mark unsaved/saved */
  bool changed;                  /**< @brief True if changed but not saved */

  std::array<double, static_cast<int>(Value::Size)> amount{}; /**< @brief Effective amount in g */

  // clang-format off
  /**
   * @brief Calculation Matrix for effect of additives on water values
   *
   * Amount of mg added or removed per added g of additive
   */
  inline static const std::array<std::array<double, static_cast<int>(Water::Value::LastAnion) + 1>, static_cast<int>(Value::Size)> calculationMatrix = {{
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
  }};
  // clang-format on
};

#endif  // ADDITIVE_H
