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
  void save();   /**< @brief Save changes */
  void cancel(); /**< @brief Revert changes */

 private:
  void addAcid(int row, QCheckBox* check, QString text, QDoubleSpinBox* percent, QDoubleSpinBox* ml);
  void addSolid(int row, QCheckBox* check, QString text, QDoubleSpinBox* g);

  MainModel* model; /**< @brief Pointer to main model for accessing data */
  QGridLayout* layout;

  QDoubleSpinBox *c3h6o3Volume, *c3h6o3Concentration;
  QDoubleSpinBox *hclVolume, *hclConcentration;
  QDoubleSpinBox *h2so4Volume, *h2so4Concentration;
  QDoubleSpinBox *h3po4Volume, *h3po4Concentration;
  QDoubleSpinBox *caso4, *cacl2, *nacl, *nahco3, *mgcl2, *mgso4, *caco3;
  QCheckBox *c3h6o3Check, *hclCheck, *h2so4Check, *h3po4Check, *caso4Check, *cacl2Check, *naclCheck, *nahco3Check,
      *mgcl2Check, *mgso4Check, *caco3Check;
};

#endif // ADDITIVEWINDOW_H
