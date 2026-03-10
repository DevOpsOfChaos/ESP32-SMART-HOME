# Server-Basis

Diese Serverstruktur ist die **neue** öffentliche Basis für den Raspberry-Pi-Teil des Projekts.

## Ziel
Der Server übernimmt später:
- MQTT-Broker
- Node-RED
- Visualisierung
- Automationen
- Logging / Auswertung

## Wichtige Abgrenzung
Diese Struktur ist **nicht** aus alten Flows zusammenkopiert.
Sie ist absichtlich schlank, damit die Serverseite neu und sauber aufgebaut werden kann.

## Geplante Unterordner
- `config/` öffentliche Beispielkonfigurationen
- `flows/` neue Node-RED-Flows
- `db/` Datenmodelle und Schemen
- `tools/` Hilfsskripte
- `docs/` Server-spezifische Dokumentation

## Regeln
- nur `.example`-Dateien mit Beispieldaten committen
- keine realen Passwörter oder Tokens eintragen
- keine aus altem Flow-Chaos übernommene Spezialstruktur einführen
- MQTT-Schema immer mit `docs/04_mqtt_topics.md` abgleichen
