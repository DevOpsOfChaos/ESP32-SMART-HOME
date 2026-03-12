# SERVER_SIMULATION_PLAN

Stand: `2026-03-12`

## Zweck

Diese Planung beschreibt eine serverseitige Validierungsstrategie mit einem Fake-Master an der MQTT-Grenze.
Sie soll den aktuellen Server gegen vertragstreue, aber simulierte MQTT-Nachrichten pruefen, ohne die reale Architektur zu verbiegen.

Diese Planung ist ausdruecklich:

- kein Firmware-Aenderungsauftrag
- kein ESP-NOW-Protokolltest
- kein Fake-Hardware-Nachweis
- kein direkter Server-zu-Node-Test

## Harte Grenzen

- Nodes bleiben auch in der Simulation nur interne Modelle hinter dem Fake-Master.
- MQTT bleibt ausschliesslich die Grenze zwischen Master und Server.
- Der Server spricht nie direkt mit einem Node-Modell.
- Simulierte MQTT-Nachrichten muessen denselben V1-Topic-Baum benutzen wie der reale Vertrag, weil der aktuelle Server nur dort lauscht.
- `HELLO` ist fuer den Server nicht direkt sichtbar. Simulierbar ist nur die serverseitige Folge eines erfolgreichen `HELLO`/`HELLO_ACK`, also der danach vom Master publizierte MQTT-Burst.

## Aktueller Repo-Faktensatz

- Der Server verarbeitet aktuell `server/status`, `master/<master_id>/{status,event}` und `node/<node_id>/{meta,status,state,event,ack,cfg/report}`.
- Unbekannte Master und Nodes werden serverseitig automatisch in SQLite angelegt.
- `status`, `state`, `event` und `ack` bleiben in `device_last_state` getrennt.
- `cfg/report` wird in `device_config` persistiert.
- Influx bekommt nur numerische Werte aus `node/.../state`.
- Retained `event` und retained `ack` werden auditiert, aber nicht als aktueller Zustand uebernommen.
- Es gibt im aktuellen Flow-Generator keine explizite serverseitige Duplikatunterdrueckung fuer ingestete MQTT-Nachrichten. `audit_log` schreibt jede Nachricht, `device_last_state` arbeitet effektiv nach dem Muster "last write wins", und das reale Influx-Verhalten bei Duplikaten muss gemessen statt geraten werden.
- Der versionierte `cmd/set`-Egress ist aktuell hart auf `net_erl_01` und `net_zrl_01` verdrahtet.

Die letzte Zeile ist der entscheidende Haken:
Eine saubere, nicht verwechselbare Simulation mit `sim_*`-IDs kann alle Ingest-Pfade sofort pruefen.
Den serverseitigen `cmd/set`-Egress kann sie mit den aktuellen versionierten Flows fuer `sim_*`-IDs aber nicht vollstaendig pruefen, solange kein kleiner testbarer Simulationspfad im Server existiert.

## Simulationsstrategie

### 1. Isolierte Laufumgebung

Die Simulation muss auf einem isolierten lokalen Server-Stack laufen:

- eigener Compose-Projektname, zum Beispiel `smarthome_sim`
- Bind nur auf `127.0.0.1`
- abweichende Host-Ports, zum Beispiel `18883`, `18880`, `18086`
- frische oder bewusst zurueckgesetzte Volumes fuer Nachweislaeufe

Ohne diese Isolation ist die ganze Uebung wertlos.
Dann vermischst du Simulationsdaten mit echten Geraeten und produzierst spaeter Beweis-Muell.

### 2. Einziger MQTT-Akteur ausser dem Server ist der Fake-Master

Der Fake-Master ist ein einzelner Testakteur mit zwei Rollen:

- MQTT-Publisher fuer `smarthome/master/<sim_master_id>/...` und `smarthome/node/<sim_node_id>/...`
- MQTT-Subscriber fuer `smarthome/node/<sim_node_id>/cmd/set` und spaeter optional `cfg/set`

Die simulierten Nodes existieren nur innerhalb dieses Fake-Masters als interne Modelle.
Sie verbinden sich nicht separat mit MQTT.

### 3. Keine neue Topic-Welt erfinden

Nicht benutzen:

- `smarthome/sim/...`
- `simulation/...`
- irgendein zweiter Topic-Baum

Grund:
Der aktuelle Server abonniert den V1-Vertrag unter `smarthome/master/...` und `smarthome/node/...`.
Wenn die Simulation einen Sonder-Baum erfindet, testet sie nicht den echten Serververtrag.

Die Trennung zwischen real und simuliert muss deshalb ueber drei Dinge kommen:

- isolierte Laufumgebung
- reservierte `sim_*`-IDs
- explizite Simulation-Kennzeichnung in Protokoll und Doku

### 4. `HELLO` ehrlich einordnen

Simulation kann fuer den Server nur das pruefen:

- Master kommt online
- Node wird durch den Master bekannt gemacht
- Node publiziert erste `meta`, `status`, `state`

Simulation kann fuer den Server nicht das pruefen:

- echten ESP-NOW-`HELLO`-Transport
- `HELLO_ACK` auf Funkebene
- Timing- und Retry-Details zwischen Master und Node auf der Luftschnittstelle

Wenn spaeter jemand behauptet, der Server-Simulationslauf habe `HELLO` bewiesen, ist das schlicht falsch.

## Namensregeln fuer Simulationsgeraete und Topics

| Element | Regel | Beispiel |
|---|---|---|
| Compose-Projekt | Prefix `smarthome_sim` | `smarthome_sim` |
| Master-ID | immer `sim_master_<nn>` | `sim_master_01` |
| Node-ID | immer `sim_<basisklasse>_<nn>` | `sim_net_erl_01`, `sim_net_sen_01`, `sim_bat_sen_01` |
| Display-Name | immer mit `[SIM]` beginnen | `[SIM] sim_net_sen_01` |
| Room / Label | wenn gesetzt, mit `sim_` beginnen | `sim_lab` |
| Request-ID | Prefix `simreq_` plus Case-ID | `simreq_ack_ok_0001` |
| Testfall-ID | Prefix `simcase_` | `simcase_offline_recovery` |
| Topic-Baum | unveraendert nach Vertrag | `smarthome/node/sim_net_sen_01/state` |
| Protokolldatei | Prefix `server_sim_` im Namen | `betaXX_server_sim_ingest_ack_duplicate.txt` |

Zusaetzliche Regeln:

- keine realen Device-IDs wie `net_erl_01`, `net_zrl_01`, `net_sen_01`, `bat_sen_01` fuer Simulationslaeufe verwenden
- keine realen MAC-Adressen, COM-Ports oder Hardwarebezeichnungen in Simulationspayloads verwenden
- keine Screenshots oder SQL-Ausgaben aus Simulation ohne `[SIM]`-Kontext weiterverwenden

## Server-Flows, die per Simulation sinnvoll validierbar sind

| Flow | Simulierbarer Stimulus am Fake-Master | Erwartete Serverwirkung | Grenze |
|---|---|---|---|
| `HELLO` / Registrierung, serverseitige Folge | `master/status`, dann `node/meta`, `node/status online:true`, optional erstes `node/state` | `devices`-Anlage fuer Master und Node, `meta_json`, `last_seen`, `device_capabilities`, `device_last_state` | prueft nicht den echten ESP-NOW-`HELLO` |
| `status` / `state` / `meta` Ingest | vertragstreue Payloads fuer Aktor-, Sensor- und Batterieprofile | getrennte Ablage von `status_json` und `state_json`, Capability-Inferenz, Metadaten-Update, Auto-Registry | prueft nur MQTT-Vertrag oberhalb des Masters |
| Offline / Recovery | `node/status online:false`, spaeter wieder `online:true`, optional neues `meta` und `state` | Status kippt auf offline und zurueck, letzter `state` bleibt bis zur naechsten Zustandsmeldung erhalten, Audit-Zeilen entstehen | prueft nicht die echte Timeout-Logik im Master |
| `ack`-Handling | `node/ack` mit realitaetsnahen Feldern wie `request_id`, `ack_seq`, `retry_count` | `audit_log` ingest, `device_last_state.ack_json`, keine Vermischung mit `state_json` | prueft nicht die Funk-ACK-Mechanik |
| Duplikat-Handling | identische `meta`, `status`, `state`, `ack`, `cfg/report` mehrfach senden | `audit_log` schreibt jede Nachricht, Upserts bleiben idempotent, `device_last_state` wird ueberschrieben, reales Influx-Verhalten wird beobachtet | es gibt aktuell keine explizite serverseitige Ingest-Dedup |
| SQLite-Schreibpfade | Mischlast aus `meta`, `status`, `state`, `ack`, `cfg/report` | `devices`, `device_capabilities`, `device_config`, `device_last_state`, `audit_log` werden korrekt bedient | nur Serversicht |
| Influx-Schreibpfad | `node/state` mit numerischen und nicht numerischen Feldern | nur numerische Sensorwerte landen in Influx, Aktor-/Bool-/Textfelder nicht | Influx-Duplikatsemantik muss gemessen werden |

## `cmd/set`-Egress-Audit: ehrliche Einordnung

Hier liegt die wichtigste Grenze des aktuellen Repo-Stands.

Was der Server heute kann:

- versionierte `cmd/set`-Publishes fuer `net_erl_01`
- versionierte `cmd/set`-Publishes fuer `net_zrl_01`
- parallele `audit_log`-Egress-Zeile fuer genau diese Pfade

Was fuer eine saubere Simulation fehlt:

- ein serverseitiger Testpfad, der dieselbe Egress-Semantik fuer `sim_*`-IDs erzeugt

Deshalb gibt es genau zwei ehrliche Optionen:

### Option A: Sofort nutzbare Simulation ohne Serveraenderung

Validieren:

- alle Ingest-Pfade
- Offline / Recovery
- ACK-Ingest
- Duplikate
- SQLite
- Influx

Nicht vollstaendig validieren:

- sauber benannten `cmd/set`-Egress fuer `sim_*`-IDs

### Option B: Kleinster Zusatz fuer vollstaendige Simulationsabdeckung

Ergaenze spaeter einen kleinen, klar getrennten serverseitigen Simulationspfad:

- eigener test-only Trigger im Server
- aktiviert nur in Simulationslaeufen
- publiziert auf `smarthome/node/sim_.../cmd/set`
- schreibt denselben `audit_log`-Egress wie der produktive Pfad
- kein neuer Topic-Baum
- keine Firmware-Aenderung

Das ist der kleinste ehrliche Weg, um auch `cmd/set`-Egress mit nicht verwechselbaren Simulations-IDs zu pruefen.

Der bequeme Shortcut, einfach `net_erl_01` oder `net_zrl_01` als "Simulation" wiederzuverwenden, ist genau die Sorte unsaubere Abkuerzung, die spaeter Nachweisdokumente unbrauchbar macht.

## Kleinster nuetzlicher Implementierungsumfang

### Stufe 1: sofort sinnvoll, ohne Serveraenderung

Ein einziges Testwerkzeug reicht:

- `server/tools/sim_master_contract_runner.py`

Aufgaben dieses Werkzeugs:

- isolierten Server-Stack starten oder gegen ihn laufen
- als Fake-Master mit dem lokalen Broker verbinden
- definierte Testfaelle sequenziell publizieren
- `cmd/set`-Topics abonnieren
- bei Bedarf passende `ack`- und `state`-Folgen zuruecksenden
- SQLite- und Influx-Erwartungen pruefen
- einen knappen Run-Report in Markdown oder JSON schreiben

Empfohlene Testfallgruppen fuer Stufe 1:

1. Registrierung: `sim_master_01` plus `sim_net_sen_01`
2. `meta`/`status`/`state`-Ingest mit realitaetsnahen Payloads
3. `cfg/report`-Ingest
4. Offline / Recovery
5. `ack`-Ingest
6. Duplikatlaeufe fuer `state`, `ack` und `cfg/report`
7. Influx-Validierung fuer numerische Sensorwerte

Das ist klein, nuetzlich und sofort belastbar.
Alles Groessere waere zuerst Tooling-Show, nicht Testwert.

### Stufe 2: kleinster Zusatz fuer den kompletten Zielscope

Wenn auch `cmd/set`-Egress mit sauberen `sim_*`-IDs abgedeckt werden soll:

- kleiner test-only Egress-Trigger im Server
- derselbe Fake-Master-Runner hoert auf `smarthome/node/sim_+/cmd/set`
- der Runner sendet passende `ack`- und `state`-Antworten

Mehr braucht es fuer den ersten ehrlichen Gesamtumfang nicht.

Nicht noetig im ersten Schritt:

- kein Fake-ESP-NOW
- kein Dashboard-UI-Test
- kein Playwright
- kein grosses Testframework
- keine zweite Compose-Datei
- keine neue DB

## Erwartete Assertions pro Kernfall

### Registrierung

- Zeile in `devices` fuer `sim_master_01`
- Zeile in `devices` fuer `sim_<geraet>_01`
- `display_name` beginnt mit `[SIM]`
- `device_role`, `device_class`, `power_source`, `origin_master_id` sind plausibel
- `device_capabilities` wurde aus `meta` aufgebaut

### Ingest und Trennung

- `status_json` und `state_json` bleiben getrennt
- `ack_json` landet nicht in `state_json`
- `cfg/report` landet in `device_config`
- `audit_log` enthaelt jede ingestete Nachricht mit `direction = ingest`

### Offline / Recovery

- `status_json.online` kippt erst auf `false`, spaeter wieder auf `true`
- letzter `state_json` bleibt bis zur neuen Zustandsmeldung erhalten
- Recovery erzeugt neue Audit-Zeilen und aktualisiert `last_seen`

### Duplikate

- `audit_log` zaehlt beide Eingaenge
- `devices` und `device_config` wachsen nicht unkontrolliert
- `device_last_state` bleibt logisch konsistent
- Influx-Ergebnis wird konkret gemessen und dokumentiert, nicht angenommen

### Influx

- numerische Metriken erzeugen `smarthome_sensor`-Punkte
- Bool- und Textfelder werden verworfen
- Tags `node_id`, `device_type`, `room`, `metric`, `unit` sind plausibel

## Trennung zwischen Simulationsevidenz und Hardwareevidenz

Diese Trennung ist nicht optional.

### Formulierungsregeln

Erlaubt:

- "Fake-Master publizierte ..."
- "simulierter Node `sim_net_sen_01` ..."
- "simulation-only Servervalidierung"

Nicht erlaubt:

- "Node sendete ..." wenn es nur der Fake-Master war
- "realer Hardwarepfad" fuer einen Simulationslauf
- "HELLO nachgewiesen" durch reine MQTT-Simulation

### Dokumentationsregeln

- Plan und Regeln leben in `docs/SERVER_SIMULATION_PLAN.md`
- einzelne Simulationslaeufe kommen als eigene historische Dateien nach `PROTOKOLL/`
- rohe Laufartefakte koennen unter `local/` oder einem anderen nicht offiziellen Arbeitsbereich liegen
- `docs/14_test_und_nachweisstand.md` darf Simulationsergebnisse nur dann aufnehmen, wenn sie dort explizit als `simulation` oder gleichwertig getrennt markiert werden
- Simulation darf niemals still in bestehende `real hardware`- oder `real lokal + real hardware`-Zeilen einsickern

Wenn diese Trennung spaeter nicht hart verteidigt wird, ist der Schaden vorhersagbar:
Du bekommst schnell eine Doku, die alles "irgendwie getestet" nennt und nichts mehr sauber beweist.

## Risiken und Grenzen

- Serversimulation beweist keinen Funkpfad.
- Serversimulation beweist keine physische Schaltwirkung.
- Serversimulation beweist keine Sensorplausibilitaet auf echter Hardware.
- Serversimulation beweist keine Master-Timeout-Implementierung an der Funkkante, sondern nur deren serverseitig sichtbare MQTT-Folge.
- Die aktuelle Serverbasis hat fuer `cmd/set` noch keinen sauberen Simulationspfad mit `sim_*`-IDs.
- Influx-Duplikatverhalten darf nicht aus Bauchgefuehl dokumentiert werden; es muss pro Testfall beobachtet werden.

## Empfohlene Ablage im Repo

Der Plan gehoert als kanonische Querreferenz nach:

- `docs/SERVER_SIMULATION_PLAN.md`

Grund:

- die Regeln betreffen nicht nur `server/`, sondern auch Nachweislogik, Beweisgrenzen und Doku-Wahrheit im Gesamtprojekt
- `server/docs/` waere fuer reine Bereichsarchitektur passend, aber zu eng fuer die notwendige Trennung zwischen Simulation und realem Projektnachweis

Begleitend sinnvoll:

- spaeter Link aus `docs/README.md`
- einzelne ausgefuehrte Simulationslaeufe in `PROTOKOLL/`
- nur klar getrennte, ehrlich klassifizierte Zusammenfassungen in `docs/14_test_und_nachweisstand.md`

## Klare Empfehlung

1. Zuerst Stufe 1 bauen: isolierter Fake-Master fuer Ingest, ACK, Offline, Duplikate, SQLite und Influx.
2. `cmd/set`-Egress nicht mit realen Pilot-IDs als angebliche Simulation weichzeichnen.
3. Wenn `cmd/set` fuer `sim_*`-IDs gebraucht wird, einen kleinen test-only Serverpfad ergaenzen und sonst nichts.
4. Simulationsevidenz immer getrennt von Hardwareevidenz halten.

Das ist die kleinste saubere Linie.
Alles andere ist entweder Selbsttaeuschung oder spaetere Nachweisverschmutzung.
