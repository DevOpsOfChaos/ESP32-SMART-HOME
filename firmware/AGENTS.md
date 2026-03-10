# AGENTS.md

Gilt fuer `firmware/`. Root-`AGENTS.md` bleibt bindend.

## Firmware-Grenzen
- `src/master` ist die einzige ESP-NOW-zu-MQTT-Bridge.
- Nodes unter `src/net_*` und `src/bat_sen` sprechen nur ESP-NOW.
- Kein MQTT, keine Broker-Logik und keine Server-Spezialannahmen in Nodes.
- `net_erl` ist die aktuelle Minimalstrecke; keine Speziallogik anderer Geraete hineinziehen.
- `net_zrl` bleibt allgemeine Zwei-Relais-Basis; Cover-Logik nur ueber klaren Cover-Modus oder benannte Flags.
- Altcode nur fuer Pinning, Sensorverhalten, Sicherheitslogik und bekannte Fehlerquellen referenzieren.

## Aenderungsstil
- Nur das betroffene Geraet oder die betroffene Shared-Library aendern.
- Gemeinsame Libraries nur anfassen, wenn der Nutzen den breiteren Impact rechtfertigt.
- Keine Querschnittsrefactors ueber mehrere Environments ohne klaren Auftrag.
- Kommentare nur dort ergaenzen, wo Hardware-, Timing- oder Sicherheitslogik sonst missverstaendlich waere.

## Checks
- Nach Firmware-Aenderungen mindestens den betroffenen Environment-Build aus `platformio.ini` ausfuehren.
- Bei Aenderungen an Protokoll- oder Shared-Code zusaetzlich naheliegende betroffene Environments bauen, soweit real moeglich.
- Beispiel: `cd .\firmware; pio run -e net_erl`
- Beispiel ohne `pio` im PATH: `cd .\firmware; & "$env:USERPROFILE\.platformio\penv\Scripts\platformio.exe" run -e master`
