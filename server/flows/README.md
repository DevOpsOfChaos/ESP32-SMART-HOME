# Server – Flows

Dieser Ordner enthält Node-RED-Flows für das SmartHome-System.

## Grundsatz

Diese Flows sind **Neuaufbau**, kein Altflow-Import.
Die alte Flow-Struktur war an alte Geräte-IDs und veraltete JSON-Felder gebunden.

## Zielstruktur

Die spaetere Flow-Struktur folgt funktionalen Modulen statt gewachsener Sammel-Tabs:

| Bereich | Zweck |
|---|---|
| `10_ingest` | MQTT-Eingang validieren und normalisieren |
| `20_registry` | `meta` und `status` in Registry uebernehmen |
| `30_state_store` | aktuellen `status` und `state` in SQLite halten |
| `40_time_series` | relevante Messwerte nach InfluxDB schreiben |
| `50_automation` | serverseitige Regeln ausfuehren |
| `60_dashboard` | FlowFuse-Dashboard-Daten und UI-Aktionen |
| `70_weather` | Wetterdaten holen, normalisieren und publizieren |
| `80_logging` | Audit, Fehlerbilder und Ereignisprotokolle |

## Regeln für neue Flows

- Kein direkter Zugriff auf Nodes (nur über Master/MQTT)
- MQTT-Schema immer mit `docs/04_mqtt_topics.md` abgleichen
- Device-IDs kommen aus den MQTT-Topics, nie hardkodiert im Flow
- Keine privaten Daten im Flow speichern (Passwörter über Umgebungsvariablen)
- Jeder Flow-Tab bekommt einen beschreibenden Namen
- FlowFuse Dashboard ist gesetzt; deprecated `node-red-dashboard` ist keine Option.
- SQLite ist operative Datenhaltung, InfluxDB ist Zeitreihenspeicher.
