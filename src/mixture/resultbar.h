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
  void setLimits(float min, float max);

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
  void paintEvent(QPaintEvent* event) override;

 private:
  static inline int width = 122;
  static inline int height = 20;

  float lmin;
  float min;
  float target;
  float max;
  float lmax;
  float value;
  float gMax;
  bool style;
};

#endif  // RESULTBAR_H
