# Testplan

## Rolle dieser Datei
Soll-Pruefplan. Diese Datei beschreibt, was geprueft werden soll, nicht was bereits belegt ist.

Aktueller belegter Ist-Stand:
- `docs/14_test_und_nachweisstand.md`

## Ziel
Die bestehende Basis soll schrittweise, reproduzierbar und ohne Schein-Nachweise geprueft werden.

## Phase 1 – Repo- und Strukturpruefung
- Repository-Struktur vollständig
- keine privaten Dateien im öffentlichen Bereich
- Beispielkonfigurationen vorhanden
- Rollen von Fachdoku, Testplan, Nachweis und PROTOKOLL klar

## Phase 2 – Firmware-Build pro Basisgeraet
- `master`, `net_erl`, `net_zrl`, `net_sen`, `bat_sen` bauen
- Basis-Konfiguration kompiliert
- Buildartefakte werden nicht versioniert

## Phase 3 – Build-, Flash- und Serientest
- reproduzierbarer Ablauf fuer Build, Portwahl, Flash und seriellen Monitor dokumentiert
- reale Startmeldungen und Basisverhalten pro Testgeraet nachvollziehbar

## Phase 4 – Protokoll- und Transporttest
- `HELLO` / `HELLO_ACK`
- `STATE`
- `EVENT`
- `CMD`
- `CFG`
- `ACK`
- Duplikaterkennung
- Retry-Verhalten

## Phase 5 – Server-V1
- MQTT-Ingest gemaess `docs/04_mqtt_topics.md`
- SQLite-Ablage gemaess `server/db/README.md`
- Influx-Schreibpfad fuer numerische Sensorwerte
- lokaler Broker-Start mit und ohne optionale Auth

## Phase 6 – Reale End-to-End-Strecke
- Node -> Master -> MQTT
- MQTT -> Master -> Node
- Server-Ingest
- Availability
- lokale Bedienung bleibt bei Masterverlust erhalten

## Phase 7 – Geraetebezogene Tests
- `NET-ERL` einfache Lampe
- `NET-ZRL` Rolladen-Grundfunktion
- `NET-SEN` Standardsensor
- `BAT-SEN` Fensterkontakt oder Wandschalter

## Phase 8 – Langzeit / Robustheit
- Funkabbrüche
- Neustarts
- Sensorfehler
- ACK-Verluste
- Konfigurationsänderungen
