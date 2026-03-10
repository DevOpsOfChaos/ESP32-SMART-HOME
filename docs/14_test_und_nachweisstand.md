# Test- und Nachweisstand

## Rolle dieser Datei
Belegter Ist-Stand des Repos. Hier steht nur, was durch vorhandene Repo-Artefakte nachvollziehbar belegt ist.

Nicht diese Datei:
- `06_testplan.md` = Soll
- `PROTOKOLL/` = historische Einzelstaende

## Bewertungslogik
- `real hardware`: auf echter Hardware gebaut, geflasht oder praktisch getestet
- `real lokal`: mit echten lokalen Containern, MQTT, DB oder Tools ausgefuehrt
- `build`: Kompilier- oder Syntaxnachweis ohne Realbetrieb
- `offen`: vorbereitet oder behauptet, aber nicht sauber als Nachweis gebuendelt

## Belegter Stand per 2026-03-10

| Thema | Status | Einordnung | Beleg |
|---|---|---|---|
| `master <-> net_erl` Bring-up | nachgewiesen | real hardware | `PROTOKOLL/beta06_firmware_bringup_master_net_erl..txt` |
| `master`, `net_erl`, `net_zrl`, `net_sen`, `bat_sen` Buildbasis | nachgewiesen | build | `PROTOKOLL/beta04_hardware_pinstandard_und_pinfixes.txt`, `PROTOKOLL/beta07_basisgeraete_net_zrl_net_sen_bat_sen.txt` |
| `net_zrl`, `net_sen`, `bat_sen` reale Hardwarebasis | offen | offen | kein eigener Hardware-Nachweis im Repo |
| Server-V1 MQTT-Ingest und SQLite-Ablage | nachgewiesen | real lokal | `PROTOKOLL/beta08_server_mqtt_ingest_realpayloads.txt` |
| Influx-Schreibpfad fuer numerische Sensorwerte | nachgewiesen | real lokal | `PROTOKOLL/beta09_influx_schreibpfad_verifiziert.txt` |
| Lokale MQTT-Broker-Auth in der Serverbasis | offen | offen | kein getrackter Nachweis im Repo-Stand |
| Konsolidierter realer Gesamt-Nachweis `Node -> Master -> MQTT/Server -> Master -> Node` | offen | offen | kein einzelner offizieller Gesamtbeleg im Repo |
| Reproduzierbarer Build-/Flash-/Seriell-Runbook | offen | offen | kein belastbares Schritt-fuer-Schritt-Dokument im Repo |

## Wichtige Klarstellungen
- `docs/14_minimalstrecke_net_erl_master.md` beschreibt den technischen Ablauf und den manuellen Pruefpfad der Minimalstrecke. Es ist kein globales Nachweisdokument.
- `PROTOKOLL/beta03_minimalstrecke_net_erl_master.txt` beschreibt die erste vertikale Strecke als Zielbild und Buildstand, belegt aber keinen realen Gesamtlauf auf Hardware.
- `PROTOKOLL/beta06_firmware_bringup_master_net_erl..txt` belegt den realen Bring-up von `master` und `net_erl`, nennt aber einen vollstaendigen End-to-End-Nachweis bis in den Server ausdruecklich noch offen.

## Offene Nachweis- und Doku-Luecken
- exakter realer Ablauf fuer Build, Portwahl, Flash und serielle Kontrolle von `master` und `net_erl`
- ein sauberer Gesamtbeleg fuer die reale Strecke ueber Hardware und Server
- reale Hardware-Nachweise fuer `net_zrl`, `net_sen` und `bat_sen`
- Hardware-README-Dateien enthalten noch keine belastbaren Mindestdaten zu Platine, Pins, Flash und Variationen
