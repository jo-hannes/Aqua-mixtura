// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef MALTPHEDIT_H
#define MALTPHEDIT_H

#include <QDoubleSpinBox>
#include <QFrame>
#include <QLabel>

class MaltPhEdit : public QFrame {
  Q_OBJECT
 public:
  explicit MaltPhEdit(QWidget* parent = nullptr);

  void setPh(double newPh);
  void setEbc(double newEbc);
  void setData(double newPh, double newEbc);
  [[nodiscard]] double pH() const;

 private slots:
  void usePhMalt();
  void usePhCaramalz();
  void usePhRoestmalz();

 private:
  double ebc{1};

  QLabel *phMalt, *phCaramalz, *phRoestmalz, *ebcLabel;
  QDoubleSpinBox* phSpinBox;
};

#endif  // MALTPHEDIT_H
