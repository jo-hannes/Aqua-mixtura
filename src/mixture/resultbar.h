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
  void setLimits(float min, float max, bool negative, bool logarithmic);

  /**
   * @brief Set style limits
   * @param min Minimum recommended value
   * @param target Reccomended target value
   * @param max Maximum recommended value
   */
  void setStyle(float min, float target, float max);

  /**
   * @brief Disable style baed limit for this value
   */
  void setNoStyle(void);

  /**
   * @brief Set current value
   * @param val Value
   */
  void setValue(float val);

 signals:

 protected:
  // Overwrite qt paintEvent
  void paintEvent(QPaintEvent* event) override;

 private:
  void updateMinMax(void);      /**< @brief Update gMin and gMax */
  float val2graph(float value); /**< @brief trans form value into graph coordinate */

  static inline int width = 122; /**< @brief width of result bar in pixel */
  static inline int height = 20; /**< @brief height of result bar in pixel */

  float gMin;       /**< @brief minimum value displayed in graph */
  float lMin;       /**< @brief minimum from settings */
  float sMin;       /**< @brief minimum from style */
  float target;     /**< @brief target from style */
  float sMax;       /**< @brief maximum from style */
  float lMax;       /**< @brief maximum from settings */
  float gMax;       /**< @brief maximum value displayed in graph */
  float value;      /**< @brief value to be displayed */
  bool negative;    /**< @brief negative values are valid */
  bool logarithmic; /**< @brief logarithmic scale */
  bool style;       /**< @brief Indicates if a style is set */
};

#endif  // RESULTBAR_H
