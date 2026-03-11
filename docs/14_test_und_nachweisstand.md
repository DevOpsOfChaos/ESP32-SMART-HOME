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

## Belegter Stand per 2026-03-11

| Thema | Status | Einordnung | Beleg |
|---|---|---|---|
| `master <-> net_erl` Bring-up | nachgewiesen | real hardware | `PROTOKOLL/beta06_firmware_bringup_master_net_erl..txt`, `PROTOKOLL/beta11_realer_retest_master_net_erl_mqtt_minimalpfad.txt` |
| `master <-> net_erl` MQTT-Minimalpfad inkl. Relais-Schalten | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta11_realer_retest_master_net_erl_mqtt_minimalpfad.txt` |
| `master <-> net_zrl` Bring-up inkl. HEARTBEAT und `STATE_REPORT` | nachgewiesen | real hardware | `PROTOKOLL/beta15_net_zrl_realer_bringup_und_blockade_offizieller_serverpfad.txt` |
| `master`, `net_erl`, `net_zrl`, `net_sen`, `bat_sen` Buildbasis | nachgewiesen | build | `PROTOKOLL/beta04_hardware_pinstandard_und_pinfixes.txt`, `PROTOKOLL/beta07_basisgeraete_net_zrl_net_sen_bat_sen.txt` |
| Kombinierter Live-Nachweis im aktuellen Repo-Stand fuer `net_zrl_01` (`Node-RED -> cmd/set -> audit_log egress -> Master -> net_zrl -> MQTT state -> device_last_state`) | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta16_net_zrl_kombinierter_live_nachweis_offizieller_serverpfad_aktueller_repo_stand.txt` |
| `net_sen`, `bat_sen` reale Hardwarebasis | offen | offen | kein eigener Hardware-Nachweis im Repo |
| Server-V1 MQTT-Ingest und SQLite-Ablage | nachgewiesen | real lokal | `PROTOKOLL/beta08_server_mqtt_ingest_realpayloads.txt` |
| Influx-Schreibpfad fuer numerische Sensorwerte | nachgewiesen | real lokal | `PROTOKOLL/beta09_influx_schreibpfad_verifiziert.txt` |
| Lokale MQTT-Broker-Auth in der Serverbasis | offen | offen | kein getrackter Nachweis im Repo-Stand |
| Offizieller serverseitiger Rueckweg fuer `net_erl_01` ueber Node-RED | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta12_offizieller_serverseitiger_rueckweg_nachweis_net_erl_01.txt` |
| SQLite-Audit fuer ausgehende serverseitige `cmd/set`-Publishes | nachgewiesen | real lokal | `PROTOKOLL/beta13_cmd_set_audit_sqlite_egress.txt` |
| Kombinierter Live-Nachweis im aktuellen Repo-Stand fuer den Pilotpfad `net_erl_01` (`Node-RED -> cmd/set -> audit_log egress -> Master -> net_erl -> MQTT state -> device_last_state`) | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta14_kombinierter_live_nachweis_offizieller_rueckweg_net_erl_01_aktueller_repo_stand.txt` |
| ACK-/Retry-Nachweis fuer den offiziellen `net_erl_01`-`cmd/set`-Pfad (`Node-RED -> cmd/set -> Master -> net_erl -> MQTT ack`, plus kontrollierter Retry nach `net_erl`-Reset) | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta17_net_erl_01_offizieller_ack_retry_nachweis.txt` |
| Offline-Timeout fuer `master <-> net_erl_01` inkl. vertraglichem MQTT-Statuspfad, SQLite-`audit_log`, `device_last_state.status_json` und real beobachtetem Wieder-online-Kommen | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta18_offizieller_offline_timeout_nachweis_master_net_erl_01.txt` |
| Langzeitstabilitaet fuer `master <-> net_erl_01` im aktuellen Repo-Stand ueber 45 Minuten mit 6 offiziellen serverseitigen `cmd/set`-Schaltvorgaengen, ruhigen Laufphasen und paralleler Beobachtung von MQTT, Serial und SQLite | nachgewiesen | real hardware, real lokal | `PROTOKOLL/beta19_langzeitstabilitaetsnachweis_master_net_erl_01.txt` |
| Reproduzierbarer Build-/Flash-/Seriell-Runbook | nachgewiesen | real hardware | `docs/16_build_flash_bringup_master_net_erl.md`, `PROTOKOLL/beta11_realer_retest_master_net_erl_mqtt_minimalpfad.txt` |

## Wichtige Klarstellungen
- `docs/14_minimalstrecke_net_erl_master.md` beschreibt den technischen Ablauf und den manuellen Pruefpfad der Minimalstrecke. Es ist kein globales Nachweisdokument.
- `PROTOKOLL/beta03_minimalstrecke_net_erl_master.txt` beschreibt die erste vertikale Strecke als Zielbild und Buildstand, belegt aber keinen realen Gesamtlauf auf Hardware.
- `PROTOKOLL/beta06_firmware_bringup_master_net_erl..txt` belegt den realen Bring-up von `master` und `net_erl`, nennt aber einen vollstaendigen End-to-End-Nachweis bis in den Server ausdruecklich noch offen.
- `PROTOKOLL/beta11_realer_retest_master_net_erl_mqtt_minimalpfad.txt` belegt den erneuten realen Lauf mit aktuellem `master`-/`net_erl`-Stand, sichtbaren MQTT-V1-Topics und realem Relais-Schaltpfad. Der offizielle serverseitige Bedienpfad war dort noch nicht enthalten.
- `PROTOKOLL/beta12_offizieller_serverseitiger_rueckweg_nachweis_net_erl_01.txt` belegt den offiziellen serverseitigen Rueckweg fuer `net_erl_01` ueber Node-RED im damaligen Stand. Der ausgehende `cmd/set`-Publish selbst war dort noch kein SQLite-Audit-Eintrag.
- `PROTOKOLL/beta13_cmd_set_audit_sqlite_egress.txt` belegt den aktuellen Repo-Stand fuer genau diese Luecke: ausgehende serverseitige `cmd/set`-Publishes landen jetzt zusaetzlich in `audit_log`, ohne `device_last_state` oder eingehende `state`-/`event`-/`ack`-Semantik zu vermischen.
- `PROTOKOLL/beta14_kombinierter_live_nachweis_offizieller_rueckweg_net_erl_01_aktueller_repo_stand.txt` belegt den kombinierten offiziellen Live-Lauf im aktuellen Repo-Stand. Der Lauf zeigte zusaetzlich eine reale Betriebsgrenze: Ein blosses `docker compose up --build -d` kann wegen persistiertem `server_nodered_data` ein altes `/data/flows.json` weiterfahren; fuer den echten Test neuer Flow-Generator-Staende muss die persistierte Node-RED-Datenbasis sauber erneuert oder die persistierte Flow-Datei bewusst ersetzt werden.
- `PROTOKOLL/beta15_net_zrl_realer_bringup_und_blockade_offizieller_serverpfad.txt` belegt den realen `master <-> net_zrl`-Basispfad nach einer minimalen Firmware-Korrektur im HELLO-Broadcast. Dieser Stand ist bewusst kein offizieller Servernachweis fuer `net_zrl`: In derselben Session war der lokale Docker-Desktop-Serverpfad wegen `hasNoVirtualization=true` nicht startbar, daher wurden keine `cmd/set`-, MQTT- oder SQLite-Aussagen fuer `net_zrl` nachgezogen.
- `PROTOKOLL/beta16_net_zrl_kombinierter_live_nachweis_offizieller_serverpfad_aktueller_repo_stand.txt` belegt den offiziellen kombinierten Live-Lauf fuer `net_zrl_01` im aktuellen Repo-Stand. Dafuer wurde der versionierte Node-RED-Generator minimal um den fehlenden `net_zrl_01`-`cmd/set`-Pfad erweitert, `server_nodered_data` bewusst erneuert und danach der reale Rueckweg fuer `relay_1` und `relay_2` jeweils EIN/AUS ueber Broker, SQLite und serielle Logs belegt.
- `PROTOKOLL/beta17_net_erl_01_offizieller_ack_retry_nachweis.txt` belegt den bisher fehlenden ACK-/Retry-Nachweis fuer genau den offiziellen `net_erl_01`-`cmd/set`-Pfad. Dabei wurde ACK im Normalfall ueber `smarthome/node/net_erl_01/ack` mit `request_id`, `ack_seq` und `retry_count` real beobachtet und ein kontrollierter Retry durch unmittelbaren `net_erl`-Reset vor dem offiziellen `inject_net_erl_on`-Trigger real erzwungen.
- `PROTOKOLL/beta18_offizieller_offline_timeout_nachweis_master_net_erl_01.txt` belegt den bisher offenen Offline-Timeout fuer `master <-> net_erl_01` im aktuellen Repo-Stand. Der Ausfall wurde dabei kontrolliert und real ueber einen ESP32-C3-Bootloader-Hold von `net_erl_01` erzeugt; mitten im Offline-Fenster waren `status_json = online:false` und das unveraenderte letzte `state_json = relay_1:true` gleichzeitig in SQLite sichtbar. Nach der realen Wiederkehr wurden `online:true`, HELLO/STATE und der neue fachliche Zustand wieder korrekt eingespielt.
- `PROTOKOLL/beta19_langzeitstabilitaetsnachweis_master_net_erl_01.txt` belegt den bisher offenen Langzeitnachweis fuer den Pilotpfad `master <-> net_erl_01` im aktuellen Repo-Stand. Der Lauf lief real ueber 45 Minuten mit 6 offiziellen serverseitigen `cmd/set`-Schaltvorgaengen, `543` beobachteten HEARTBEATs am Master bei `5.003 s` Durchschnittsabstand, genau `18` neuen `audit_log`-Zeilen (`6 x cmd/set:egress`, `6 x ack:ingest`, `6 x state:ingest`) und ohne unerwartete `online:false`-Umschlaege, Retries, MQTT-Reconnects oder beobachtete Neustarts.
- Die aktuell belegten kombinierten Live-Nachweise gelten weiter nur fuer die engen Pilotpfade `net_erl_01` und `net_zrl_01`. Sie sind kein Gesamtprojektnachweis; offen bleiben weiterhin weitere Basisgeraete, ACK-/Retry-Nachweise jenseits des jetzt belegten `net_erl_01`-Command-Pfads und Langzeitstabilitaet jenseits des jetzt belegten `master <-> net_erl_01`-Pilotpfads.

## Offene Nachweis- und Doku-Luecken
- ACK-/Retry-Nachweis fuer weitere Basisgeraete oder weitere Command-Pfade neben dem jetzt belegten `net_erl_01`-Pilotpfad
- Langzeitstabilitaet fuer weitere offizielle Pfade oder Mehrgeraetebetrieb jenseits des jetzt belegten `master <-> net_erl_01`-Pilotpfads
- weitere serverseitige Rueckweg-Nachweise fuer andere Basisgeraete als `net_erl_01` und `net_zrl_01`
- reale Hardware-Nachweise fuer `net_sen` und `bat_sen`
- vollstaendiger Gesamtprojektnachweis ueber die belegten `net_erl_01`- und `net_zrl_01`-Pilotpfade hinaus
- Hardware-README-Dateien enthalten noch keine belastbaren Mindestdaten zu Platine, Pins, Flash und Variationen
