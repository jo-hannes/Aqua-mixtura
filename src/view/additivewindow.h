// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVEWINDOW_H
#define ADDITIVEWINDOW_H

#include "../mainmodel.h"

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
  void addAcid(int row, QDoubleSpinBox* percent, QDoubleSpinBox* ml, QString formula, QString text);
  void addSolid(int row, QDoubleSpinBox* g, QString formula, QString text);

  MainModel* model; /**< @brief Pointer to main model for accessing data */
  QGridLayout* layout;

  QDoubleSpinBox *c3h6o3Volume, *c3h6o3Concentration;
  QDoubleSpinBox *hclVolume, *hclConcentration;
  QDoubleSpinBox *h2so4Volume, *h2so4Concentration;
  QDoubleSpinBox *h3po4Volume, *h3po4Concentration;
  QDoubleSpinBox *caso4, *cacl2, *nacl, *nahco3, *mgcl2, *mgso4, *caco3;
};

#endif // ADDITIVEWINDOW_H
