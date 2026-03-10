# Testplan

## Ziel
Die Basis soll schrittweise und nachvollziehbar geprüft werden.

## Phase 1 – Strukturtest
- Repository-Struktur vollständig
- keine privaten Dateien im öffentlichen Bereich
- Beispielkonfigurationen vorhanden
- Build- und Tool-Struktur nachvollziehbar

## Phase 2 – Firmware-Einzelnachweise
- jeder Basisnode startet
- Pins lassen sich initialisieren
- Debug gezielt aktivierbar
- Basis-Konfiguration kompiliert

## Phase 3 – Protokolltest
- `HELLO` / `HELLO_ACK`
- `STATE`
- `EVENT`
- `CMD`
- `CFG`
- `ACK`
- Duplikaterkennung
- Retry-Verhalten

## Phase 4 – End-to-End
- Node -> Master -> MQTT
- MQTT -> Master -> Node
- Availability
- lokale Bedienung bleibt bei Masterverlust erhalten

## Phase 5 – Gerätebezogene Tests
- `NET-ERL` einfache Lampe
- `NET-ZRL` Rolladen-Grundfunktion
- `NET-SEN` Standardsensor
- `BAT-SEN` Fensterkontakt oder Wandschalter

## Phase 6 – Langzeit / Robustheit
- Funkabbrüche
- Neustarts
- Sensorfehler
- ACK-Verluste
- Konfigurationsänderungen
