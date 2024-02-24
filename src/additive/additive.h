// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef ADDITIVE_H
#define ADDITIVE_H

#include <QJsonObject>

class Additive {
 public:
  Additive();

  // setter and getter
  float getCaso4() const;
  void setCaso4(float newCaso4);
  float getCacl2() const;
  void setCacl2(float newCacl2);
  float getNacl() const;
  void setNacl(float newNacl);
  float getNahco3() const;
  void setNahco3(float newNahco3);
  float getMgcl2() const;
  void setMgcl2(float newMgcl2);
  float getMgso4() const;
  void setMgso4(float newMgso4);
  float getCaco3() const;
  void setCaco3(float newCaco3);
  float getC3h6o3Volume() const;
  void setC3h6o3Volume(float newC3h6o3Volume);
  float getC3h6o3Concentration() const;
  void setC3h6o3Concentration(float newC3h6o3Concentration);
  float getHclVolume() const;
  void setHclVolume(float newHclVolume);
  float getHclConcentration() const;
  void setHclConcentration(float newHclConcentration);
  float getH2so4Volume() const;
  void setH2so4Volume(float newH2so4Volume);
  float getH2so4Concentration() const;
  void setH2so4Concentration(float newH2so4Concentration);
  float getH3po4Volume() const;
  void setH3po4Volume(float newH3po4Volume);
  float getH3po4Concentration() const;
  void setH3po4Concentration(float newH3po4Concentration);

  // JSON conversion
  bool fromJson(const QJsonObject& json);            /**< @brief convert JSON to Additive */
  QJsonObject toJson() const;                        /**< @brief convert this Additive to JSON */

 private:
  float caso4;  /**< @brief CaSO4  in gram. Calziumsulfat, Braugips */
  float cacl2;  /**< @brief CaCl2  in gram. Calziumchlorid */
  float nacl;   /**< @brief NaCl   in gram. Natriumchlorid, Kochsalz */
  float nahco3; /**< @brief NaHCO3 in gram. Natriumhydrogencarbonat */
  float mgcl2;  /**< @brief MgCl2  in gram. Magnesiumchlorid */
  float mgso4;  /**< @brief MgSO4  in gram. Magnesiumsulfat */
  float caco3;  /**< @brief CaCO3  in gram. Calciumcarbonat */

  float c3h6o3Volume;        /**< @brief C3H6O3 volume in ml. Milchsäure */
  float c3h6o3Concentration; /**< @brief C3H6O3 concentration in %. Milchsäure */
  float hclVolume;           /**< @brief HCl    volume in ml. Salzsäure */
  float hclConcentration;    /**< @brief HCl    concentration in %. Salzsäure */
  float h2so4Volume;         /**< @brief H2SO4    volume in ml. Schwefelsäure */
  float h2so4Concentration;  /**< @brief H2SO4    concentration in %. Schwefelsäure */
  float h3po4Volume;         /**< @brief H3PO4    volume in ml. Phosphorsäure */
  float h3po4Concentration;  /**< @brief H3PO4    concentration in %. Phosphorsäure */
};

#endif  // ADDITIVE_H
