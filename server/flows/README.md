# Server – Flows

Dieser Ordner enthält Node-RED-Flows für das SmartHome-System.

## Grundsatz

Diese Flows sind **Neuaufbau**, kein Altflow-Import.
Die alte Flow-Struktur war an alte Geräte-IDs und veraltete JSON-Felder gebunden.

## Geplante Flows (Phase 3 und später)

| Datei | Zweck |
|---|---|
| `10_mqtt_empfang.json` | MQTT-Eingangsnormalisierung für alle Geräte |
| `20_state_speichern.json` | STATE-Werte in SQLite schreiben |
| `30_availability.json` | Online/Offline-Status weiterverarbeiten |
| `40_automationen.json` | Regeln: Licht, Rolladen, Meldungen |
| `50_dashboard.json` | Visualisierung (Node-RED Dashboard) |
| `60_api.json` | Externe REST-API für spätere Apps |

## Regeln für neue Flows

- Kein direkter Zugriff auf Nodes (nur über Master/MQTT)
- MQTT-Schema immer mit `docs/04_mqtt_topics.md` abgleichen
- Device-IDs kommen aus den MQTT-Topics, nie hardkodiert im Flow
- Keine privaten Daten im Flow speichern (Passwörter über Umgebungsvariablen)
- Jeder Flow-Tab bekommt einen beschreibenden Namen

## Offene Entscheidungen

- Dashboard-Technologie: Node-RED Dashboard 2.x oder externe App?
- Historisierung: SQLite ausreichend oder InfluxDB für Zeitreihen?
- Notification-Kanal: Pushover, Telegram oder anderes?

Diese Fragen werden vor Phase 3 entschieden und hier eingetragen.
