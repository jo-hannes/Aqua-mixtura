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
