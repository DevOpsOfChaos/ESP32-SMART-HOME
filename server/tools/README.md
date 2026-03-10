# Server – Tools

Hilfsskripte für Betrieb, Test und Wartung des SmartHome-Servers.

## Verfügbare Skripte

| Skript | Zweck |
|---|---|
| `mqtt_test_pub.sh` | Testnachrichten auf MQTT-Topics veröffentlichen |
| `mqtt_console.sh` | Live-Mitschnitt aller MQTT-Topics |
| `smoke_test.sh` | Grundfunktionstest: MQTT erreichbar, Node-RED antwortet |
| `git_push.ps1` | Git-Push aus dem Arbeitsverzeichnis (Windows) |
| `git_pull.ps1` | Git-Pull mit Statusausgabe (Windows) |

## Noch zu erstellen (Phase 3)

- `validate_topics.py` – Prüft, ob alle Devices dem MQTT-Schema entsprechen
- `check_availability.sh` – Zeigt alle offline-gemeldeten Geräte

## Regeln

- Keine Echtzugangsdaten in Skripten
- Zugangsdaten aus .env oder Parameterübergabe
- Skripte sollen auch für Debug-Sitzungen im Alltagsbetrieb nützlich sein
