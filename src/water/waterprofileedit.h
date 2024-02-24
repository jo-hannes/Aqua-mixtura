// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef WATERPROFILEEDIT_H
#define WATERPROFILEEDIT_H

#include "water.h"

#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

class WaterProfileEdit : public QWidget
{
  Q_OBJECT

 public:
  explicit WaterProfileEdit(QWidget* parent = nullptr);

  void setProfile(const Water& profile);
  bool isChanged() const;

 public slots:
  void updateAll();        /**< @brief Update all values in view */
  void updateCalculated(); /**< @brief Update only calculated values in view */
  void save();             /**< @brief Save profile */
  void cancel();           /**< @brief Cancel edit, resets all values */

  // slots for value changes
  void valChangName(const QString& name);
  void valChangeCalcium(double val);
  void valChangeMagnesium(double val);
  void valChangeNatrium(double val);
  void valChangeHydrogencarbonat(double val);
  void valChangeChlorid(double val);
  void valChangeSulfat(double val);
  void valChangePhosphat(double val);
  void valChangeLactat(double val);

 signals:
  void saveProfile(Water& profile); /**< @brief Send signal with profile we want to save */

 private:
  void addEditableValue(int row, QDoubleSpinBox* spinBox, QString text, QString unit);
  void addCalculatedValue(int row, QLabel* value, QString text, QString unit);
  bool changed;
  Water givenProfile, volatileProfile;

  QGridLayout* layout;
  QLineEdit* name;
  QDoubleSpinBox *calcium, *magnesium, *natrium;
  QDoubleSpinBox *hydrogencarbonat, *chlorid, *sulfat, *phosphat, *lactat;
  QLabel *restalkallität, *gesammtHaerte, *carbonHaerte, *caHaerte, *mgHaerte, *nichtcarbonHaerte, *so4cl;
};

#endif // WATERPROFILEEDIT_H
