// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef RESULTBAR_H
#define RESULTBAR_H

#include <QObject>
#include <QWidget>

/**
 * @brief Show results as bar graph
 */
class ResultBar : public QWidget {
  Q_OBJECT
 public:
  explicit ResultBar(QWidget* parent = nullptr);

  /**
   * @brief Set absoulte limits
   * @param min Minimum value
   * @param max Maximum value
   */
  void setLimits(double min, double max, bool negative, bool logarithmic);

  /**
   * @brief Set style limits
   * @param min Minimum recommended value
   * @param target Reccomended target value
   * @param max Maximum recommended value
   */
  void setStyle(double min, double target, double max);

  /**
   * @brief Disable style baed limit for this value
   */
  void setNoStyle();

  /**
   * @brief Set current value
   * @param val Value
   */
  void setValue(double val);

 signals:

 protected:
  // Overwrite qt paintEvent
  void paintEvent(QPaintEvent* event) override;

 private:
  void updateMinMax();                             /**< @brief Update gMin and gMax */
  [[nodiscard]] int val2graph(double value) const; /**< @brief trans form value into graph coordinate */

  // Some constants
  static const int width = 122;  /**< @brief width of result bar in pixel */
  static const int height = 20;  /**< @brief height of result bar in pixel */
  static const int indSize = 10; /**< @brief size of value indicator in pixel */

  double gMin{0};          /**< @brief minimum value displayed in graph */
  double lMin{0};          /**< @brief minimum from settings */
  double sMin{0};          /**< @brief minimum from style */
  double target{0};        /**< @brief target from style */
  double sMax{0};          /**< @brief maximum from style */
  double lMax{0};          /**< @brief maximum from settings */
  double gMax{0};          /**< @brief maximum value displayed in graph */
  double value{0};         /**< @brief value to be displayed */
  bool negative{false};    /**< @brief negative values are valid */
  bool logarithmic{false}; /**< @brief logarithmic scale */
  bool style{false};       /**< @brief Indicates if a style is set */
};

#endif  // RESULTBAR_H
