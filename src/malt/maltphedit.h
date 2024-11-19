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

  // QSize sizeHint() const override;

  void setPh(float newPh);
  void setEbc(float newEbc);
  void setData(float newPh, float newEbc);
  float pH() const;

 private slots:
  void usePhMalt();
  void usePhCaramalz();
  void usePhRoestmalz();

 private:
  float ebc;

  QLabel *phMalt, *phCaramalz, *phRoestmalz, *ebcLabel;
  QDoubleSpinBox* phSpinBox;
};

#endif  // MALTPHEDIT_H
