# Berechnung der Zusatzstoffe

## Berechnung des Wasserprofils

In der calculationMatrix wird hinterlegt um wie viel mg sich das Wasserprofil verändert pro Gramm vom Zusatzstoff.

Der Effekt auf das Volumen wird ignoriert, da dieser im normalfall minimal und vernachlässigbar ist.

Zum Vergleich werden auch die werte folgender Tools durch reverse Engineering ermittelt:
1. https://hobbybrauer.de/forum/viewtopic.php?p=399250#p399250
2. https://hobbybrauer.de/forum/viewtopic.php?p=507536#p507536
3. https://www.maischemalzundmehr.de/index.php?inhaltmitte=toolswasserrechner

Alle Werte sind in mg/g. Also mg an zusätzlichen Stoff im Wasser pro g an Zusatzstoff.

### Milchsäure C₃H₆O₃

Mit einem $pK_s = 3,90$ ist Milchsäure eine starke Säure.
Folglich wird jedes Milchsäure Molekül ein $H^+$ abgeben, welches mit dem Hydrogencarbonat reagiert.
Bei dieser Reaktion wird das Hydrogencarbonat abgebaut.

$$ C₃H₆O₃ = 90,078 \frac{g}{mol} $$
$$ C₃H₅O₃ = 89,07 \frac{g}{mol} $$
$$ HCO₃ = 61,016 \frac{g}{mol} $$

$$ C₃H₅O₃ = \frac{1000 \cdot mg}{g} \cdot \frac{Na}{NaHCO₃} = 1000\frac{mg}{g} \cdot \frac{89,07 \frac{g}{mol}}{90,078 \frac{g}{mol}} = 988,81 \frac{mg}{g} $$
$$ HCO₃ = \frac{1000 \cdot mg}{g} \cdot \frac{HCO₃}{NaHCO₃} = 1000\frac{mg}{g} \cdot \frac{61,016 \frac{g}{mol}}{90,078 \frac{g}{mol}} = 677,37 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 0         | 0       | -680.32          | 0       | 0      | 0        | 999    |
|         2. | 0       | 0         | 0       | -677.18          | 0       | 0      | -        | -      |
| Berechnung | 0       | 0         | 0       | -677,37          | 0       | 0      | 0        | 988,81 |
|       Wert | 0       | 0         | 0       | -677.37          | 0       | 0      | 0        | 988,81 |

### Salzsäure HCl

Mit einem $pK_s = -6$ ist Salzsäure eine sehr starke Säure.
Folglich wird jedes Salzsäure Molekül ein $H^+$ abgeben, welches mit dem Hydrogencarbonat reagiert.
Bei dieser Reaktion wird das Hydrogencarbonat abgebaut.

$$ HCl = 36,458 \frac{g}{mol} $$
$$ Cl = 35,45 \frac{g}{mol} $$
$$ HCO₃ = 61,016 \frac{g}{mol} $$

$$ Cl = \frac{1000 \cdot mg}{g} \cdot \frac{Cl}{HCl} = 1000\frac{mg}{g} \cdot \frac{35,45 \frac{g}{mol}}{36,458 \frac{g}{mol}} = 972,35 \frac{mg}{g} $$
$$ HCO₃ = \frac{1000 \cdot mg}{g} \cdot \frac{HCO₃}{HCl} = 1000\frac{mg}{g} \cdot \frac{61,016 \frac{g}{mol}}{36,458 \frac{g}{mol}} = 1673,60 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 0         | 0       | -1685.60         | 1005,55 | 0      | 0        | 0      |
|         2. | 0       | 0         | 0       | -1673.07         | 972,60  | 0      | -        | -      |
| Berechnung | 0       | 0         | 0       | -1673,60         | 972,35  | 0      | 0        | 0      |
|       Wert | 0       | 0         | 0       | -1673.60         | 972,35  | 0      | 0        | 0      |

### Schwefelsäure H₂SO₄

Schwefelsäure besitzt folgend $pK_s$ Werte:

| Formel    | $pK_s$ | Stärke     |
| :-------- | :----- | :--------- |
| $H₂SO₄$   | -3     | sehr stark |
| $HSO_4^-$ | 1,9    | stark      |

Es ist davon aus zu gehen das jedes Schwefelsäure Molekül zwei $H^+$ abgibt, welche mit dem Hydrogencarbonat reagieren.
Bei dieser Reaktion wird das Hydrogencarbonat abgebaut.

$$ H₂SO₄ = 98,072 \frac{g}{mol} $$
$$ SO₄ = ( 32,06 + 4 \cdot 15,999 ) \frac{g}{mol} = 96,056 \frac{g}{mol} $$
$$ HCO₃ = 61,016 \frac{g}{mol} $$

$$ SO₄ = \frac{1000 \cdot mg}{g} \cdot \frac{SO₄}{H₂SO₄} = 1000\frac{mg}{g} \cdot \frac{96,056 \frac{g}{mol}}{98,072 \frac{g}{mol}} = 979,44 \frac{mg}{g} $$
$$ HCO₃ = \frac{1000 \cdot mg}{g} \cdot \frac{2 \cdot HCO₃}{H₂SO₄} = 1000\frac{mg}{g} \cdot \frac{122,032 \frac{g}{mol}}{98,072 \frac{g}{mol}} = 1244,31 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 0         | 0       | -1246.38         | 0       | 972,32 | 0        | 0      |
|         2. | 0       | 0         | 0       | -1243.88         | 0       | 979,59 | -        | -      |
| Berechnung | 0       | 0         | 0       | -1244,31         | 0       | 979,44 | 0        | 0      |
|       Wert | 0       | 0         | 0       | -1244,31         | 0       | 979,44 | 0        | 0      |

### Phosphorsäure H₃PO₄

Phosphorsäure besitzt folgend $pK_s$ Werte:

|         | $pK_s$ | Stärke      |
| :------ | :----- | :---------- |
| $pK_s1$ | 2,16   | stark       |
| $pK_s2$ | 7,21   | mittelstark |
| $pK_s3$ | 12,32  | schwach     |

Phosphorsäure ist eine eher schwache Säure, von daher ist davon aus zu gehen das jedes Phosphorsäure Molekül ein $H^+$ abgibt, welches mit dem Hydrogencarbonat reagiert.
Bei dieser Reaktion wird das Hydrogencarbonat abgebaut.

$$ H₃PO₄ = 97,993761998 \frac{g}{mol} $$
$$ H₂PO₄ = 96,985761998 \frac{g}{mol} $$
$$ HCO₃ = 61,016 \frac{g}{mol} $$

$$ H₂PO₄ = \frac{1000 \cdot mg}{g} \cdot \frac{H₂PO₄}{H₃PO₄} = 1000\frac{mg}{g} \cdot \frac{96,985761998 \frac{g}{mol}}{97,993761998 \frac{g}{mol}} = 989,71 \frac{mg}{g} $$
$$ HCO₃ = \frac{1000 \cdot mg}{g} \cdot \frac{HCO₃}{H₃PO₄} = 1000\frac{mg}{g} \cdot \frac{61,016 \frac{g}{mol}}{97,993761998 \frac{g}{mol}} = 622,65 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 0         | 0       | -623,22          | 0       | 0      | 972,37   | 0      |
|         2. | 0       | 0         | 0       | -622,45          | 0       | 0      | -        | -      |
| Berechnung | 0       | 0         | 0       | -622,65          | 0       | 0      | 989,71   | 0      |
|       Wert | 0       | 0         | 0       | -622,65          | 0       | 0      | 989,71   | 0      |

### Calciumsulfat (Braugips) CaSO₄ + 2 H₂O

Wir gehen immer von Calciumsulfat-Dihydrat aus.

$$ CaSO₄ + 2 \cdot H₂O = 172,17 \frac{g}{mol} $$
$$ Ca = 40,078 \frac{g}{mol} $$
$$ SO₄ = ( 32,06 + 4 \cdot 15,999 ) \frac{g}{mol} = 96,056 \frac{g}{mol} $$

$$ Ca = \frac{1000 \cdot mg}{g} \cdot \frac{Ca}{CaSO₄ + 2 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{40,078 \frac{g}{mol}}{172,17 \frac{g}{mol}} = 232,78 \frac{mg}{g} $$
$$ SO₄ = \frac{1000 \cdot mg}{g} \cdot \frac{SO₄}{CaSO₄ + 2 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{96,056 \frac{g}{mol}}{172,17 \frac{g}{mol}} = 557,91 \frac{mg}{g} $$


|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 232,78  | 0         | 0       | 0                | 0       | 557,93 | 0        | 0      |
|         2. | 232,78  | 0         | 0       | 0                | 0       | 557,93 | 0        | -      |
|         3. | 232,8   | 0         | 0       | -                | 0       | 558    | -        | -      |
| Berechnung | 232,78  | 0         | 0       | 0                | 0       | 557,91 | 0        | 0      |
|       Wert | 232,78  | 0         | 0       | 0                | 0       | 557,91 | 0        | 0      |

### Calciumchlorid CaCl₂ + 2 H₂O

Wir gehen immer von Calciumchlorid-Dihydrat aus.

$$ CaCl₂ + 2 \cdot H₂O = 147,02 \frac{g}{mol} $$
$$ Ca = 40,078 \frac{g}{mol} $$
$$ Cl₂ = ( 2 \cdot 35,45 ) \frac{g}{mol} = 70,90 \frac{g}{mol} $$

$$ Ca = \frac{1000 \cdot mg}{g} \cdot \frac{Ca}{CaCl₂ + 2 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{40,078 \frac{g}{mol}}{147,02 \frac{g}{mol}} = 272,60 \frac{mg}{g} $$
$$ Cl₂ = \frac{1000 \cdot mg}{g} \cdot \frac{Cl₂}{CaCl₂ + 2 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{70,90 \frac{g}{mol}}{147,02 \frac{g}{mol}} = 482,25 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 272,60  | 0         | 0       | 0                | 482,28  | 0      | 0        | 0      |
|         2. | 272,60  | 0         | 0       | 0                | 482,28  | 0      | 0        | -      |
|         3. | 272,7   | 0         | 0       | -                | 482,2   | 0      | -        | -      |
| Berechnung | 272,60  | 0         | 0       | 0                | 482,25  | 0      | 0        | 0      |
|       Wert | 272,60  | 0         | 0       | 0                | 482,25  | 0      | 0        | 0      |

### Natriumchlorid (Kochsalz) NaCl

$$ NaCl = 58,44 \frac{g}{mol} $$
$$ Na = 22,98976928 \frac{g}{mol} $$
$$ Cl = 35,45 \frac{g}{mol} $$

$$ Na = \frac{1000 \cdot mg}{g} \cdot \frac{Na}{NaCl} = 1000\frac{mg}{g} \cdot \frac{22,98976928 \frac{g}{mol}}{58,44 \frac{g}{mol}} = 393,39 \frac{mg}{g} $$
$$ Cl = \frac{1000 \cdot mg}{g} \cdot \frac{Cl}{NaCl} = 1000\frac{mg}{g} \cdot \frac{35,45 \frac{g}{mol}}{58,44 \frac{g}{mol}} = 606,61 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 0         | 393,39  | 0                | 606,66  | 0      | 0        | 0      |
|         2. | 0       | 0         | 393,39  | 0                | 606,66  | 0      | 0        | -      |
|         3. | 0       | 0         | 393,4   | -                | 606,7   | 0      | -        | -      |
| Berechnung | 0       | 0         | 393,39  | 0                | 606,61  | 0      | 0        | 0      |
|       Wert | 0       | 0         | 393,39  | 0                | 606,61  | 0      | 0        | 0      |

### Natriumhydrogencarbonat (Natron) NaHCO₃

$$ NaHCO₃ = 84,00576928 \frac{g}{mol} $$
$$ Na = 22,98976928 \frac{g}{mol} $$
$$ HCO₃ = 61,016 \frac{g}{mol} $$

$$ Na = \frac{1000 \cdot mg}{g} \cdot \frac{Na}{NaHCO₃} = 1000\frac{mg}{g} \cdot \frac{22,98976928 \frac{g}{mol}}{84,00576928 \frac{g}{mol}} = 273,67 \frac{mg}{g} $$
$$ HCO₃ = \frac{1000 \cdot mg}{g} \cdot \frac{HCO₃}{NaHCO₃} = 1000\frac{mg}{g} \cdot \frac{61,016 \frac{g}{mol}}{84,00576928 \frac{g}{mol}} = 726,33 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 0         | 273,66  | 726,34           | 0       | 0      | 0        | 0      |
|         2. | 0       | 0         | 273,66  | 726,34           | 0       | 0      | -        | -      |
|         3. | 0       | 0         | 273,7   | -                | 0       | 0      | -        | -      |
| Berechnung | 0       | 0         | 273,67  | 726,33           | 0       | 0      | 0        | 0      |
|       Wert | 0       | 0         | 273,67  | 726,33           | 0       | 0      | 0        | 0      |

### Magnesiumchlorid MgCl₂ + 6 H₂O

Wir gehen immer von Magnesiumchlorid Hexahydrat aus.

$$ MgCl₂ + 6 \cdot H₂O = 203,3 \frac{g}{mol} $$
$$ Mg = 24,305 \frac{g}{mol} $$
$$ Cl₂ = ( 2 \cdot 35,45 ) \frac{g}{mol} = 70,90 \frac{g}{mol} $$

$$ Mg = \frac{1000 \cdot mg}{g} \cdot \frac{Mg}{MgCl₂ + 6 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{24,305 \frac{g}{mol}}{203,3 \frac{g}{mol}} = 119,55 \frac{mg}{g} $$
$$ Cl₂ = \frac{1000 \cdot mg}{g} \cdot \frac{Cl₂}{MgCl₂ + 6 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{70,90 \frac{g}{mol}}{203,3 \frac{g}{mol}} = 348,75 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 119,53    | 0       | 0                | 174.39  | 0      | 0        | 0      |
|         2. | 0       | 98,59     | 0       | 0                | 174.39  | 0      | -        | -      |
| Berechnung | 0       | 119,55    | 0       | 0                | 348,75  | 0      | 0        | 0      |
|       Wert | 0       | 119,55    | 0       | 0                | 348,75  | 0      | 0        | 0      |

TODO: Faktor zwei zwischen meiner Berechnung und den anderen bei Chlorid! Wie kommt das zustande?

### Magnesiumsulfat MgSO₄ + 7 H₂O

Wir gehen immer von Magnesiumsulfat Heptahydrat aus.

$$ MgSO₄ + 7 \cdot H₂O = 246,48 \frac{g}{mol} $$
$$ Mg = 24,305 \frac{g}{mol} $$
$$ SO₄ = ( 32,06 + 4 \cdot 15,999 ) \frac{g}{mol} = 96,056 \frac{g}{mol} $$

$$ Mg = \frac{1000 \cdot mg}{g} \cdot \frac{Mg}{MgSO₄ + 7 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{24,305 \frac{g}{mol}}{246,48 \frac{g}{mol}} = 98,61 \frac{mg}{g} $$
$$ SO₄ = \frac{1000 \cdot mg}{g} \cdot \frac{SO₄}{MgSO₄ + 7 \cdot H₂O} = 1000\frac{mg}{g} \cdot \frac{96,056 \frac{g}{mol}}{246,48 \frac{g}{mol}} = 389,71 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 0       | 98,59     | 0       | 0                | 0       | 389,73 | 0        | 0      |
|         2. | 0       | 98,59     | 0       | 0                | 0       | 389,73 | 0        | -      |
|         3. | 0       | 98,6      | 0       | -                | 0       | 389,7  | -        | -      |
| Berechnung | 0       | 98,61     | 0       | 0                | 0       | 389,71 | 0        | 0      |
|       Wert | 0       | 98,61     | 0       | 0                | 0       | 389,71 | 0        | 0      |

### Calciumcarbonat CaCO₃

Calciumcarbonat wird nicht einfach gelöst sondern reagiert mit dem Brauwasser nach folgender Formel:
 
$$ CaCO₃ + CO₂ + H₂O \Rightarrow Ca(HCO₃)₂ $$

Es entstehen also aus einem Calciumcarbonat Molekül ein Calcium und zwei Hydrogencarbonat Moleküle

$$ CaCO₃ = 100,09 \frac{g}{mol} $$
$$ Ca = 40,078 \frac{g}{mol} $$
$$ HCO₃ = 61,016 \frac{g}{mol} $$

$$ Ca = \frac{1000 \cdot mg}{g} \cdot \frac{Ca}{CaCO₃} = 1000\frac{mg}{g} \cdot \frac{40,078 \frac{g}{mol}}{100,09 \frac{g}{mol}} = 400,42 \frac{mg}{g} $$
$$ HCO₃ = \frac{1000 \cdot mg}{g} \cdot \frac{2 \cdot HCO₃}{NaHCO₃} = 1000\frac{mg}{g} \cdot \frac{122,032 \frac{g}{mol}}{100,09 \frac{g}{mol}} = 1219,22 \frac{mg}{g} $$

|     Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| ---------: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|         1. | 400     | 0         | 0       | 1220             | 0       | 0      | 0        | 0      |
| Berechnung | 400,42  | 0         | 0       | 1219,22          | 0       | 0      | 0        | 0      |
|       Wert | 400,42  | 0         | 0       | 1219,22          | 0       | 0      | 0        | 0      |

## Berechnung der Dichte

Zur Umrechnung der zugegebenen Menge zwischen Gramm und Milliliter wird bei Säuren deren Dichte benötigt.

### Methode

Anhand von Datenblättern wird die Dichte bei verschiedenen Konzentrationen bestimmt.
Mit diesen Werten wird dann eine Funktion n-ten grades gesucht welche diese Werte möglichst gut approximiert.
Hierfür werden die ermittelten Werte in einer CSV-Datei hinterlegt und mithilfe von Gnuplot eine Funktion ermittelt.

Für eine Konzentration von 0% wird die Dichte von Wasser genommen.

### Milchsäure C₃H₆O₃

Bei der Recherche wurden Sicherheitsdatenblätter größtenteils ignoriert, da diese vor allem allgemeine Informationen zu Milchsäure enthalten aber keine konkreten angaben zum speziellen Produkt.

Aus Datenblätter im Internet konnten folgende Werte ermittelt werden:

|      Gew. % |                 g/ml | Referenz |
| ----------: | -------------------: | :------- |
| 20,5 - 21,5 | 1,045 - 1,055 (20°C) | [1]      |
|          20 |          1,05 (20°C) | [3]      |
| 79,5 - 80,5 |   1,18 - 1,21 ( ?°C) | [2]      |
|          80 |          1,18 (20°C) | [3]      |
|          80 |         1,168 (20°C) | [4]      |
| 79,5 - 80,5 |   1,18 - 1,20 (20°C) | [5]      |
| 79,0 - 81,0 |   1,18 - 1,20 (20°C) | [6]      |
|          90 |          1,22 (20°C) | [3]      |
|        > 88 |   1,20 - 1,21 (20°C) | [7]      |

[1]: https://shop.es-drei.de/documents/spezifikation/SPEC_S100453_L(+)-Milchs%C3%A4ure_21_DE.pdf
[2]: https://www.syskem.de/syskem_pdf_spez/spdb_Milchsaeure_80_.pdf
[3]: https://www.joqora.de/media/sdb/4260452040573.pdf
[4]: https://www.conatex.com/media/manuals/BADE/BADE_9993349.pdf
[5]: https://www.chemieshop24.de/milchsaeure-80-e270-300kg-palette/1000009823000-2
[6]: https://www.hobbybrauerversand.de/mediafiles/Datenblaetter/Brouwland/lactol/Spezifikationen.pdf
[7]: https://assets.thermofisher.com/chem-specs-pdf/retrievePdf?rootSku=18987&sku=189870051

Hieraus werden folgende Werte extrahiert:

| Gew. % | g/ml  |
| -----: | :---- |
|      0 | 0,998 |
|     21 | 1,05  |
|     80 | 1,19  |
|     90 | 1,21  |


Hieraus ergibt sich folgende Formel für eine Lineare Annäherung:

$$ d[g/ml] = 0.00236361 * c[Gew. \%] + 0.999137 $$

Eine Quadratische Annäherung bringt keine nennenswerte Verbesserung.
Die Abweichung von den eingangswerten ist auch minimal.

| Gew. % | g/ml  | Rechnung   | diff        |
| -----: | :---- | :--------- | :---------- |
|      0 | 0,998 | 0,999137   | +0,001137   |
|     21 | 1,05  | 1,04877281 | -0,00122719 |
|     80 | 1,19  | 1,1882258  | -0,0017742  |
|     90 | 1,21  | 1,2118619  | +0,0018619  |

### Salzsäure HCl

Folgende Formel ist aus der [Wikipedia](https://de.wikipedia.org/wiki/Salzs%C3%A4ure#Eigenschaften) entnommen:

$$ c[Gew. \%] = ( d[g/ml] - 1) * 200 $$

Durch umstellen ergibt sich hieraus:

$$ d[g/ml] = \frac{c[Gew. \%]}{200} + 1 = 0.005 * c[Gew. \%] + 1 $$


Daneben ist noch eine Tabelle mit Werten:

| Gew. % | g/ml  |
| -----: | :---- |
|      0 | 0,998 |
|   10,5 | 1,050 |
|   15,5 | 1,075 |
|   20,4 | 1,100 |
|   22,3 | 1,110 |
|   24,3 | 1,120 |
|   26,2 | 1,130 |
|   28,2 | 1,140 |
|   30,1 | 1,150 |
|   32,1 | 1,160 |
|   34,2 | 1,170 |
|   36,2 | 1,180 |
|   38,3 | 1,190 |
|   40,4 | 1,200 |

Hieraus ergibt sich folgende Formel für eine Lineare Annäherung:

$$ d[g/ml] = 0.00503334 * c[Gew. \%] + 0.997681 $$

Eine Quadratische Annäherung bringt keine nennenswerte Verbesserung.
Die Abweichung von den eingangswerten ist auch minimal.

| Gew. % | g/ml  | Rechnung    | diff         |
| -----: | :---- | :---------- | :----------- |
|      0 | 0,998 | 0,997681    | -0,000319    |
|   10,5 | 1,050 | 1,05053107  | +0,00053107  |
|   15,5 | 1,075 | 1,07569777  | +0,00069777  |
|   20,4 | 1,100 | 1,100361136 | +0,000361136 |
|   22,3 | 1,110 | 1,109924482 | -0,000075518 |
|   24,3 | 1,120 | 1,119991162 | -0,000008838 |
|   26,2 | 1,130 | 1,129554508 | -0,000445492 |
|   28,2 | 1,140 | 1,139621188 | -0,000378812 |
|   30,1 | 1,150 | 1,149184534 | -0,000815466 |
|   32,1 | 1,160 | 1,159251214 | -0,000748786 |
|   34,2 | 1,170 | 1,169821228 | -0,000178772 |
|   36,2 | 1,180 | 1,179887908 | -0,000112092 |
|   38,3 | 1,190 | 1,190457922 | +0,000457922 |
|   40,4 | 1,200 | 1,201027936 | +0,001027936 |

### Phosphorsäure H₃PO₄

Folgende Werte wurden ermittelt:

| Gew. % | g/ml  |
| -----: | :---- |
|      0 | 0,998 |
|     25 | 1,150 |
|     40 | 1,255 |
|     50 | 1,336 |
|     75 | 1,58  |
|     85 | 1,689 |

Quelle: https://www.carlroth.com/de/de/von-a-bis-z/ortho-phosphorsaeure/p/4332.1

Hieraus ergibt sich folgende Formel für eine Quadratische Annäherung:

$$ d[g/ml] = 0.999386 + 0.00491011 * c[Gew. \%] + 3.76331 * 10^{-5} * c[Gew. \%]^2 $$

Die Abweichung von den eingangswerten ist minimal.

| Gew. % | g/ml  | Rechnung     | diff          |
| -----: | :---- | :----------- | :------------ |
|      0 | 0,998 | 0,999386     | +0,001386     |
|     25 | 1,150 | 1,1456591875 | -0,0043408125 |
|     40 | 1,255 | 1,25600296   | +0,00100296   |
|     50 | 1,336 | 1,33897375   | +0,00297375   |
|     75 | 1,58  | 1,5793296875 | -0,0006703125 |
|     85 | 1,689 | 1,6886436475 | -0,0003563525 |

### Schwefelsäure H₂SO₄

Folgende Werte wurden ermittelt:

| Gew. % | g/ml  |
| -----: | :---- |
|      0 | 0,998 |
|      2 | 1,013 |
|     10 | 1,068 |
|     20 | 1,139 |
|     30 | 1,219 |
|     40 | 1,303 |
|     50 | 1,395 |
|     75 | 1,669 |
|     96 | 1,835 |

Quelle: https://www.carlroth.com/de/de/von-a-bis-z/schwefelsaeure/p/2800.1

Hieraus ergibt sich folgende Formel für eine Quadratische Annäherung:

$$ d[g/ml] = 0.993422 + 0.00723747 * c[Gew. \%] + 1.77195 * 10^{-5} * c[Gew. \%]^2 $$
sqrt(WSSR/ndf)    : 0.016328

Leider ist die Näherung nicht so gut wie bei den andere Säuren, aber eine höhere Ordnung verbessert die Näherung nur minimal.
Auch sollte die Genauigkeit trotzdem mehr als ausreichen sein.

| Gew. % | g/ml  | Rechnung     | diff          |
| -----: | :---- | :----------- | :------------ |
|      0 | 0,998 | 0,993422     | -0,004578     |
|      2 | 1,013 | 1,007967818  | -0,005032182  |
|     10 | 1,068 | 1,06756865   | -0,00043135   |
|     20 | 1,139 | 1,1452592    | +0,0062592    |
|     30 | 1,219 | 1,22649365   | +0,00749365   |
|     40 | 1,303 | 1,311272     | +0,008272     |
|     50 | 1,395 | 1,39959425   | +0,00459425   |
|     75 | 1,669 | 1,6359044375 | -0,0330955625 |
|     96 | 1,835 | 1,851522032  | +0,016522032  |
