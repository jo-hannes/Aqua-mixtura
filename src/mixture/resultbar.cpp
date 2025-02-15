// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "resultbar.h"

#include <QPainter>

ResultBar::ResultBar(QWidget* parent) : QWidget{parent} {
  // setBackgroundRole(QPalette::Base);
  QPalette pal;
  pal.setColor(QPalette::Window, Qt::white);
  setPalette(pal);
  setAutoFillBackground(true);

  // This widget has a fixed size!
  setFixedSize(width, height);
}

void ResultBar::setLimits(double min, double max, bool negative, bool logarithmic) {
  this->lMin = min;
  this->lMax = max;
  this->negative = negative;
  this->logarithmic = logarithmic;
  update();
}

void ResultBar::setStyle(double min, double target, double max) {
  this->sMin = min;
  this->target = target;
  this->sMax = max;
  style = true;
  update();
}

void ResultBar::setNoStyle() {
  style = false;
  update();
}

void ResultBar::setValue(double val) {
  value = val;
}

void ResultBar::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  const QPen pen(Qt::NoPen);
  painter.setPen(pen);
  QBrush brush(Qt::red);

  // ensure valid gMin and gMax
  updateMinMax();

  if (style) {
    // just start with all red
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);
    // calculate start and end of yellow area
    int s = val2graph(lMin);
    int e = val2graph(lMax) - s;
    brush.setColor(Qt::yellow);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
    // calculate start and end of green area
    s = val2graph(sMin);
    e = val2graph(sMax) - s;
    brush.setColor(Qt::green);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
    // calculate location of dark green target indicator
    s = val2graph(target);
    brush.setColor(Qt::darkGreen);
    painter.setBrush(brush);
    painter.drawRect(s - 1, 0, 2, height);
  } else {
    // Show only limits
    // just start with all red
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);
    // calculate start and end of green area
    const int s = val2graph(lMin);
    const int e = val2graph(lMax) - s;
    // green till lmax
    brush.setColor(Qt::green);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
  }

  // print indicator
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  const int ind = val2graph(value);     // indicator position
  constexpr int indHalf = indSize / 2;  // Half size of indicator
  // check if we are out of range
  // QPoint points[3];
  QPolygon points;
  if (ind < 0) {
    // below range
    points << QPoint(0, height - indHalf);        // left indication tip
    points << QPoint(indSize, height - indSize);  // right top
    points << QPoint(indSize, height);            // right bottom

  } else if (ind > width) {
    // over range
    points << QPoint(width, height - indHalf);            // right indication tip
    points << QPoint(width - indSize, height - indSize);  // left top
    points << QPoint(width - indSize, height);            // left bottom
  } else {
    // in range
    points << QPoint(ind - indHalf, height);  // left bottom
    points << QPoint(ind, height - indSize);  // top indication tip
    points << QPoint(ind + indHalf, height);  // right bottom
  }
  painter.drawPolygon(points);
}

void ResultBar::updateMinMax() {
  double tMin = 0;
  double tMax = 0;
  double tDiff = 0;
  // first get min and max from limits and style
  if (style) {
    // first calculate max value displayed
    tMin = std::min(lMin, sMin);
    tMax = std::max(lMax, sMax);
  } else {
    tMin = lMin;
    tMax = lMax;
  }
  tDiff = tMax - tMin;

  // Add 10% on each side // TODO also 10% with logarithmic scale?
  gMin = tMin - 0.1 * tDiff;
  gMax = tMax + 0.1 * tDiff;
  // ne negative values without negaive set
  if (gMin < 0 && !negative) {
    gMin = 0;
  }
}

int ResultBar::val2graph(double value) const {
  // TODO add support for logarithmic scale
  const double gDiff = gMax - gMin;
  value -= gMin;
  return value / gDiff * width;  // NOLINT(*-narrowing-conversions): double to int is indented
}
