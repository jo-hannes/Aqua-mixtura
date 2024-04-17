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

  style = false;
}

void ResultBar::setLimits(float min, float max) {
  lmin = min;
  lmax = max;
  update();
}

void ResultBar::setStyle(float min, float target, float max) {
  this->min = min;
  this->target = target;
  this->max = max;
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

  if (style) {
    // first calculate max value displayed
    if (max > lmax) {
      gMax = max * 1.1;
    } else {
      gMax = lmax * 1.1;
    }
    // just start with all red
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);
    // calculate start and end of yellow area
    float s = lmin / gMax * width;
    float e = lmax / gMax * width - s;
    brush.setColor(Qt::yellow);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
    // calculate start and end of green area
    s = min / gMax * width;
    e = max / gMax * width - s;
    brush.setColor(Qt::green);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
    // calculate location of dark green target indicator
    s = target / gMax * width;
    int i = round(s);
    brush.setColor(Qt::darkGreen);
    painter.setBrush(brush);
    painter.drawRect(i - 1, 0, 2, height);
  } else {
    // Show only limits
    // first calculate max value displayed
    gMax = lmax * 1.1;  // add 10% over limit
    // just start with all red
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width, height);
    // calculate start and end of green area
    float s = lmin / gMax * width;
    float e = lmax / gMax * width - s;
    // green till lmax
    brush.setColor(Qt::green);
    painter.setBrush(brush);
    painter.drawRect(s, 0, e, height);
  }

  // print indicator
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  float ind = value / gMax * width;
  QPoint points[3] = {QPoint(ind - 5, height), QPoint(ind, height / 2), QPoint(ind + 5, height)};
  painter.drawPolygon(points, 3);
}
