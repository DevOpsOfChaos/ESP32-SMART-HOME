# Server-Ueberblick

Der Server ist in dieser Architektur nicht die Intelligenz der Feldgeraete.
Er arbeitet oberhalb des Masters und uebernimmt:
- MQTT im Heimnetz
- Node-RED als serverseitige Automations- und Integrationsschicht
- FlowFuse Dashboard als UI
- InfluxDB fuer Sensor-Zeitreihen
- SQLite fuer Geraete-, Config-, Automations- und Auditdaten

Direkte Kommunikation mit Nodes findet nicht statt.
Alle Wege laufen ueber den Master.

## Verbindliche Server-Dokumente

- `01_server_basisarchitektur.md`
  Zielbild, Module, Verantwortlichkeiten, Compose-Zielplattform, Wetterintegration
- `../../docs/04_mqtt_topics.md`
  MQTT-Vertrag mit Topic-Baum, Payload-Regeln und Publikationsrechten
- `../db/README.md`
  Datenmodell fuer SQLite und InfluxDB
- `02_dashboard_konzept.md`
  Seiten, Kernfunktionen und UI-Regeln fuer FlowFuse Dashboard
- `../flows/README.md`
  Zielstruktur der spaeteren Node-RED-Flow-Module

## Harte Grenzen

- Nur Heimnetz, keine Cloud-Pflicht und keine Home-Assistant-Abhaengigkeit.
- Docker Compose ist die Zielplattform fuer PC und spaeter Raspberry Pi.
- `smarthome/` bleibt das MQTT-Praefix.
- `status` und `state` bleiben getrennt.
- Physische Geraete werden technisch nur durch den Master bekannt gemacht.
