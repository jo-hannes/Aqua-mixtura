// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVEWINDOW_H
#define ADDITIVEWINDOW_H

#include "../mainmodel.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QObject>
#include <QString>
#include <QWidget>

/**
 * @brief Window for managing your additions
 */
class AdditiveWindow : public QWidget
{
  Q_OBJECT

 public:
  /**
   * @brief Constructor
   * @param model Pointer to main model for accessing data
   * @param parent
   */
  explicit AdditiveWindow(MainModel* model, QWidget *parent = nullptr);

 public slots:
  void update(); /**< @brief Update all values */
  void save();   /**< @brief Save changes */
  void cancel(); /**< @brief Revert changes */

  // slots for ui interaction
  void valChangeC3h6o3Checked(int checked);
  void valChangeC3h6o3Concentration(double val);
  void valChangeC3h6o3Volume(double val);
  void valChangeHclChecked(int checked);
  void valChangeHclConcentration(double val);
  void valChangeHclVolume(double val);
  void valChangeH2so4Checked(int checked);
  void valChangeH2so4Concentration(double val);
  void valChangeH2so4Volume(double val);
  void valChangeH3po4Checked(int checked);
  void valChangeH3po4Concentration(double val);
  void valChangeH3po4Volume(double val);
  void valChangeCaso4Checked(int checked);
  void valChangeCaso4(double val);
  void valChangeCacl2Checked(int checked);
  void valChangeCacl2(double val);
  void valChangeNaclChecked(int checked);
  void valChangeNacl(double val);
  void valChangeNahco3Checked(int checked);
  void valChangeNahco3(double val);
  void valChangeMgcl2Checked(int checked);
  void valChangeMgcl2(double val);
  void valChangeMgso4Checked(int checked);
  void valChangeMgso4(double val);
  void valChangeCaco3Checked(int checked);
  void valChangeCaco3(double val);

 private:
  void addAcid(int row, QCheckBox* check, QString text, QDoubleSpinBox* percent, QDoubleSpinBox* ml);
  void addSolid(int row, QCheckBox* check, QString text, QDoubleSpinBox* g);
  void updateAcid(QCheckBox* check, QDoubleSpinBox* spinPercent, QDoubleSpinBox* spinMl, float percent, float ml);
  void updateSolid(QCheckBox* check, QDoubleSpinBox* spinG, float g);
  float updateValChecked(float val, int checked);

  Additive tmpData;

  MainModel* model;    /**< @brief Pointer to main model for accessing data */
  QGridLayout* layout; /**< @brief Main layout */

  // Pointer to UI elements
  QDoubleSpinBox *c3h6o3Volume, *c3h6o3Concentration;
  QDoubleSpinBox *hclVolume, *hclConcentration;
  QDoubleSpinBox *h2so4Volume, *h2so4Concentration;
  QDoubleSpinBox *h3po4Volume, *h3po4Concentration;
  QDoubleSpinBox *caso4, *cacl2, *nacl, *nahco3, *mgcl2, *mgso4, *caco3;
  QCheckBox *c3h6o3Check, *hclCheck, *h2so4Check, *h3po4Check, *caso4Check, *cacl2Check, *naclCheck, *nahco3Check,
      *mgcl2Check, *mgso4Check, *caco3Check;
};

#endif // ADDITIVEWINDOW_H
