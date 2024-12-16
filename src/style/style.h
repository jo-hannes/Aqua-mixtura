// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef STYLE_H
#define STYLE_H

#include "../common/meta.h"
#include "../water/water.h"

#include <QAbstractTableModel>
#include <QJsonObject>
#include <QString>

/**
 * @brief Class for a single beer style
 */
class Style : public QAbstractTableModel, public Meta {
  Q_OBJECT

 public:
  /**
   * @brief Type of limits
   */
  enum class Limit { Min, Target, Max, Size };

  explicit Style(QString name = "");

  // JSON conversion
  explicit Style(const QJsonObject& json);  /**< @brief Construct Style from JSON */
  bool fromJson(const QJsonObject& json);   /**< @brief Update Style from JSON */
  [[nodiscard]] QJsonObject toJson() const; /**< @brief convert Style to JSON */

  [[nodiscard]] Style* copy() const; /**< @brief create a copy*/

  // getter und setter
  [[nodiscard]] double get(Water::Value what, Limit limit) const; /**< @brief get requested value */
  void set(Water::Value what, Limit limit, double value);         /**< @brief set given type with value */
  [[nodiscard]] bool isLimited(Water::Value what) const;          /**< @brief is value limited */
  void limit(Water::Value what, bool limit);                      /**< @brief Limit value */

  [[nodiscard]] bool isChanged() const; /**< @brief True if changes not saved */
  void setChanged(bool changed);        /**< @brief Set changed state */

  // for QAbstractTableModel, see QT documentation for details
  // NOLINTBEGIN(modernize-use-nodiscard)
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  // NOLINTEND(modernize-use-nodiscard)

 private:
  bool changed{false}; /**< @brief True if changed but not saved */

  std::array<bool, static_cast<int>(Water::Value::Size)> limited{};
  std::array<std::array<double, static_cast<int>(Limit::Size)>, static_cast<int>(Water::Value::Size)> limits{};
  // NOLINTNEXTLINE(cert-err58-cpp): it is very unlikely and programm will not run without this strings
  static const inline std::array<QString, static_cast<int>(Limit::Size)> strJsonKey{"Min", "Target", "Max"};
};

#endif  // STYLE_H
