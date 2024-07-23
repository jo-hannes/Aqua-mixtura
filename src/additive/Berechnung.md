# Berechnung der Zusatzstoffe

## Berechnung des Wasserprofils

In der calculationMatrix wird hinterlegt um wie viel mg sich das Wasserprofil verändert pro Gramm vom Zusatzstoff.

Der Effekt auf das Volumen wird ignoriert, da dieser im normalfall minimal und vernachlässigbar ist.

Die werte wurden größtenteils durch reverse Engineering folgender Tools ermittelt:
1. https://hobbybrauer.de/forum/viewtopic.php?p=399250#p399250
2. https://hobbybrauer.de/forum/viewtopic.php?p=507536#p507536
3. https://www.maischemalzundmehr.de/index.php?inhaltmitte=toolswasserrechner

Alle Werte sind in mg/g. Also mg an zusätzlichen Stoff im Wasser pro g an Zusatzstoff.

### Milchsäure C₃H₆O₃

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 0         | 0       | -680.32          | 0       | 0      | 0        | 999    |
|     2. | 0       | 0         | 0       | -677.18          | 0       | 0      | -        | -      |
|   Wert | 0       | 0         | 0       | -677.18          | 0       | 0      | 0        | 999    |

### Salzsäure HCl

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 0         | 0       | -1685.60         | 1005.55 | 0      | 0        | 0      |
|     2. | 0       | 0         | 0       | -1673.07         | 972.60  | 0      | -        | -      |
|   Wert | 0       | 0         | 0       | -1673.07         | 972.60  | 0      | 0        | 0      |

### Schwefelsäure H₂SO₄

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 0         | 0       | -1246.38         | 0       | 972.32 | 0        | 0      |
|     2. | 0       | 0         | 0       | -1243.88         | 0       | 979.59 | -        | -      |
|   Wert | 0       | 0         | 0       | -1243.88         | 0       | 979.59 | 0        | 0      |

### Phosphorsäure H₃PO₄

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 0         | 0       | -623.22          | 0       | 0      | 972.37   | 0      |
|     2. | 0       | 0         | 0       | -622.45          | 0       | 0      | -        | -      |
|   Wert | 0       | 0         | 0       | -622.45          | 0       | 0      | 972.37   | 0      |

### Calciumsulfat (Braugips) CaSO₄

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 232.78  | 0         | 0       | 0                | 0       | 557.93 | 0        | 0      |
|     2. | 232.78  | 0         | 0       | 0                | 0       | 557.93 | 0        | -      |
|     3. | 232.8   | 0         | 0       | -                | 0       | 558    | -        | -      |
|   Wert | 232.78  | 0         | 0       | 0                | 0       | 557.93 | 0        | 0      |

### Calciumchlorid CaCl₂

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 272.60  | 0         | 0       | 0                | 482.28  | 0      | 0        | 0      |
|     2. | 272.60  | 0         | 0       | 0                | 482.28  | 0      | 0        | -      |
|     3. | 272.7   | 0         | 0       | -                | 482.2   | 0      | -        | -      |
|   Wert | 272.60  | 0         | 0       | 0                | 482.28  | 0      | 0        | 0      |

### Natriumchlorid (Kochsalz) NaCl

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 0         | 393.39  | 0                | 606.66  | 0      | 0        | 0      |
|     2. | 0       | 0         | 393.39  | 0                | 606.66  | 0      | 0        | -      |
|     3. | 0       | 0         | 393.4   | -                | 606.7   | 0      | -        | -      |
|   Wert | 0       | 0         | 393.39  | 0                | 606.66  | 0      | 0        | 0      |

### Natriumhydrogencarbonat (Natron) NaHCO₃

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 0         | 273.66  | 726.34           | 0       | 0      | 0        | 0      |
|     2. | 0       | 0         | 273.66  | 726.34           | 0       | 0      | -        | -      |
|     3. | 0       | 0         | 273.7   | -                | 0       | 0      | -        | -      |
|   Wert | 0       | 0         | 273.66  | 726.34           | 0       | 0      | 0        | 0      |

### Magnesiumchlorid MgCl₂

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 119.53    | 0       | 0                | 174.39  | 0      | 0        | 0      |
|     2. | 0       | 98.59     | 0       | 0                | 174.39  | 0      | -        | -      |
|   Wert | 0       | 59.78     | 0       | 0                | 174.39  | 0      | 0        | 0      |

Ich bin von keinem der Werte für Magnesium überzeugt.
59.78? => Basierend auf dem Verhältnis der Atommassen zum Chlorid

### Magnesiumsulfat MgSO₄

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 0       | 98.59     | 0       | 0                | 0       | 389.73 | 0        | 0      |
|     2. | 0       | 98.59     | 0       | 0                | 0       | 389.73 | 0        | -      |
|     3. | 0       | 98.6      | 0       | -                | 0       | 389.7  | -        | -      |
|   Wert | 0       | 98.59     | 0       | 0                | 0       | 389.73 | 0        | 0      |

### Calciumcarbonat CaCO₃

| Quelle | Calcium | Magnesium | Natrium | Hydrogencarbonat | Chlorid | Sulfat | Phosphat | Lactat |
| -----: | :------ | :-------- | :------ | :--------------- | :------ | :----- | :------- | :----- |
|     1. | 400     | 0         | 0       | 1220             | 0       | 0      | 0        | 0      |
|   Wert | 400     | 0         | 0       | 1220             | 0       | 0      | 0        | 0      |

## Berechnung der Dichte

Zur Berechnung der zugegebenen Menge in Gramm wird bei Säuren deren Dichte benötigt.

TODO
