# Docs-Index

## Rolle dieser Datei
Zentraler Index fuer die oeffentliche Projektdoku.

## Zuerst lesen
1. `../README.md`
2. `00_projektueberblick.md`
3. `01_architektur.md`
4. `DECISIONS.md`
5. `14_test_und_nachweisstand.md`, wenn Status oder Testaussagen relevant sind

## Kerndoku
- `DECISIONS.md`: dauerhafte Entscheidungen mit Grund
- `00_projektueberblick.md`: kurzer Projektabriss
- `01_architektur.md`: Zielarchitektur und harte Grenzen
- `14_test_und_nachweisstand.md`: belegter Ist-Stand; `simulation` bleibt dort explizit von `real hardware` getrennt

## Fachdoku
- `03_protokoll.md`: ESP-NOW-Protokoll und Paketregeln
- `04_mqtt_topics.md`: MQTT-Vertrag
- `05_hardware_geraete.md`: Basisgeraete und Verweis auf die aktuelle Sondermodul-Doku
- `BAT_SEN_ADC_FACT_GAP.md`: aktueller `bat_sen`-ADC-Stand, erledigte Realnachweise und ehrliche Restpunkte
- `15_hardware_pinstandard.md`: verbindlicher Pinstandard
- `17_firmware_featurematrix.md`: Basistyp-Zustaendigkeit, Feature-Suitability und ehrliche Reifestufen
- `18_setup_provisioning_reset.md`: verbindlicher Setup-, Provisionierungs- und Resetvertrag fuer Nodes
- `19_sondermodule_roadmap.md`: aktuelle Sondermodule, ihre Basistyp-Ableitung und offene Reifepunkte

## Test und Nachweis
- `06_testplan.md`: Soll-Pruefplan, keine Ist-Aussagen
- `14_minimalstrecke_net_erl_master.md`: technischer Ablauf und Detailblatt der Minimalstrecke, kein globales Nachweisdokument
- `16_build_flash_bringup_master_net_erl.md`: kontrolliertes Runbook fuer Build, Portwahl, Flash und seriellen Bring-up von `master` und `net_erl`

## Historie
- `../PROTOKOLL/`: historische Einzelstaende, inklusive damaliger Testaussagen
- `../CHANGELOG.md`: verdichtete Versionshistorie des Repos

## Bereichsdoku
- `../server/README.md` und `../server/docs/`: Server-Bereichsdoku
- `../server/docs/03_server_runbook_lokalbetrieb.md`: lokales Betriebs-Runbook fuer den `server/`-Stack
- `../server/docs/04_server_simulation_runbook.md`: isolierter Fake-Master-Simulationslauf; ausdruecklich kein Hardware-Nachweis
- `../server/db/README.md`: Datenbasis des Servers
- `../server/flows/README.md`: Flow-Rolle und Ingest-Semantik
- `../hardware/*/README.md`: geraetespezifische Hardwareunterlagen
- `../tools/*/README.md`: Werkzeugrollen
- `../firmware/test/*/README.md`: Testordner-Rollen

## Offene Doku-Luecken
- bewusste Board-Profilwahl fuer `bat_sen_01`-`battery_pct` und spaetere Deep-Sleep-Rueckkehrvalidierung erst bei implementiertem Sleep-Pfad; der `battery_mv`-Messpfad ist bereits real belegt
- weitere offizielle Nachweise jenseits der belegten Einzelpfade `net_erl_01`, `net_zrl_01`, `net_sen_01`, `bat_sen_01` und der getrennten `sim_*`-Servervalidierung
- vollstaendiger Gesamtprojektnachweis ueber die bisherigen realen Pilotpfade und die getrennte Simulation hinaus
- Fotos, Schaltplaene und variantenspezifische Verdrahtungsdetails sind weiterhin nicht als eigene Hardware-Doku im Repo nachgezogen
