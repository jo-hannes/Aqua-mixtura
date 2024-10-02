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

  negative = false;
  logarithmic = false;
  style = false;
}

void ResultBar::setLimits(float min, float max, bool negative, bool logarithmic) {
  this->lMin = min;
  this->lMax = max;
  this->negative = negative;
  this->logarithmic = logarithmic;
  update();
}

void ResultBar::setStyle(float min, float target, float max) {
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

void ResultBar::setValue(float val) {
  value = val;
}

void ResultBar::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  QPen pen(Qt::NoPen);
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
    float s = val2graph(lMin);
    float e = val2graph(lMax) - s;
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
    int i = round(s);
    brush.setColor(Qt::darkGreen);
    painter.setBrush(brush);
    painter.drawRect(i - 1, 0, 2, height);
  } else {
    // Show only limits
    // just start with all red
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);
    // calculate start and end of green area
    float s = val2graph(lMin);
    float e = val2graph(lMax) - s;
    // green till lmax
    brush.setColor(Qt::green);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
  }

  // print indicator
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  float ind = val2graph(value);
  // check if we are out of range
  QPoint points[3];
  if (ind < 0) {
    // below range
    points[0] = QPoint(0, height - 5);
    points[1] = QPoint(10, height - 10);
    points[2] = QPoint(10, height);

  } else if (ind > width) {
    // over range
    points[0] = QPoint(width, height - 5);
    points[1] = QPoint(width - 10, height - 10);
    points[2] = QPoint(width - 10, height);
  } else {
    // in range
    points[0] = QPoint(ind - 5, height);
    points[1] = QPoint(ind, height / 2);
    points[2] = QPoint(ind + 5, height);
  }
  painter.drawPolygon(points, 3);
}

void ResultBar::updateMinMax() {
  float tMin, tMax, tDiff;
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

float ResultBar::val2graph(float value) {
  // TODO add support for logarithmic scale
  float gDiff = gMax - gMin;
  value -= gMin;
  return value / gDiff * width;
}
