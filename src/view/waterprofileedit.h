#ifndef WATERPROFILEEDIT_H
#define WATERPROFILEEDIT_H

#include "../model/waterprofile.h"

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

  void setProfile(const WaterProfile& profile);

 public slots:
  void updateAll();
  void updateCalculated();
  void save();
  void cancel();

  // value changes
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
  void saveProfile(WaterProfile& profile);

 private:
  void addEditableValue(int row, QDoubleSpinBox* spinBox, QString text, QString unit);
  void addCalculatedValue(int row, QLabel* value, QString text, QString unit);
  WaterProfile givenProfile, volatileProfile;

  QGridLayout* layout;
  QLineEdit* name;
  QDoubleSpinBox *calcium, *magnesium, *natrium;
  QDoubleSpinBox *hydrogencarbonat, *chlorid, *sulfat, *phosphat, *lactat;
  QLabel *restalkallität, *gesammtHaerte, *carbonHaerte, *caHaerte, *mgHaerte, *nichtcarbonHaerte, *so4cl;
};

#endif // WATERPROFILEEDIT_H
