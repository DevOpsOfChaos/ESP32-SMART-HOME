# Hardware-Pinstandard

## Zweck des Dokuments
Dieses Dokument legt die feste Pinbelegung der aktuellen Platinen als verbindlichen Hardwarestandard fest. Es ist die fachliche Referenz fuer neue Firmware, neue Geraetevarianten und Reviews bestehender PinConfigs.

## Verbindliche globale Pinstandards der Platinen
- I2C liegt immer auf `GPIO0` = SDA und `GPIO1` = SCL.
- Der interne NeoPixel liegt immer auf `GPIO8`.
- Der Firmware-Referenzpunkt im Repo ist `firmware/include/HardwarePinStandard.h`.

## Regeln fuer alle Boards
- Die I2C-Konvention `GPIO0/GPIO1` gilt boarduebergreifend, auch wenn ein konkretes Geraet den Bus in der aktuellen Firmware noch nicht nutzt.
- Der interne NeoPixel auf `GPIO8` ist als feste Board-Vorgabe zu betrachten. Firmware darf diesen Pin nicht fuer andere Funktionen umwidmen.
- Board-spezifische `PinConfig.h`-Dateien muessen diese Standards uebernehmen statt freie Platzhalter oder abweichende Magic Numbers zu fuehren.

## Regeln nur fuer Relais-Boards
- Relais-Boards nutzen `Relais 1 = GPIO10`.
- Falls ein zweites Relais vorhanden ist, gilt `Relais 2 = GPIO5`.
- `net_erl` nutzt damit verbindlich `GPIO10` fuer `Relais 1`.
- `net_zrl` nutzt verbindlich `GPIO10` fuer `Relais 1` und `GPIO5` fuer `Relais 2`.

## Regeln nur fuer Batterie-Geraete
- Batterie-Geraete messen die Batteriespannung ueber einen Spannungsteiler auf `GPIO4`.
- Die konkrete ADC-Kalibrierung bleibt Firmware-Aufgabe, der Anschluss-Pin selbst ist nicht mehr offen.

## Verbindlichkeit fuer neue Firmware und neue Geraete
- Neue Firmware fuer bestehende Boards muss diese Pinbelegung einhalten.
- Neue Geraete auf derselben Platinenbasis muessen diese Pinregeln ebenfalls uebernehmen, solange keine ausdruecklich dokumentierte Hardwareabweichung vorliegt.

## Abweichungen
- Abweichungen sind nicht stillschweigend erlaubt.
- Jede Abweichung muss vor Umsetzung ausdruecklich dokumentiert werden:
  - in der betroffenen Hardware-Doku
  - in der zugehoerigen `PinConfig.h`
  - mit kurzer Begruendung, warum der Standard verlassen wird
