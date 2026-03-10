# Server Basis V1

## Rolle dieser Datei
Bereichs-README fuer `server/`. Diese Datei beschreibt nur die Serverbasis und ihren Startpfad.

Nicht diese Datei:
- offizielles Projektgedaechtnis: `../AGENTS.md`, `../docs/PROJECT_CONTEXT.md`, `../docs/CURRENT_SPRINT.md`, `../docs/TASK_QUEUE.md`, `../docs/DECISIONS.md`
- belegter Gesamtstatus: `../docs/14_test_und_nachweisstand.md`

Diese V1 stellt das erste reproduzierbare Serverfundament bereit fuer:

- Mosquitto
- Node-RED mit FlowFuse Dashboard
- InfluxDB
- SQLite

## Scope

- automatische Geraeteanlage aus MQTT
- strikte Trennung von `status` und `state`
- getrennte Behandlung von `event`, `ack` und `cfg/report`
- Sensor-Zeitreihen in InfluxDB
- operative Daten in SQLite
- FlowFuse-Dashboard mit den V1-Seiten
- Wetter nur vorbereitet, nicht fertig

## Bereichsdoku im Server
- `docs/00_server_ueberblick.md`
- `docs/01_server_basisarchitektur.md`
- `docs/02_dashboard_konzept.md`
- `db/README.md`
- `flows/README.md`
- `config/README.md`

## MQTT-Ingest V1

- `smarthome/server/status` landet in `server_settings`
- `smarthome/master/<master_id>/status` und `.../event` legen Master bei Bedarf automatisch an
- `smarthome/node/<node_id>/meta|status|state|event|ack|cfg/report` legen Nodes bei Bedarf automatisch an
- `status_json` und `state_json` bleiben strikt getrennt
- `cfg/report` landet in `device_config`
- Influx bekommt nur numerische Sensorwerte aus `state.sensors`
- retained `event` und retained `ack` werden nur auditiert, aber nicht als aktueller Geraetezustand verarbeitet

## Start

Im Ordner `server/`:

1. optional: `Copy-Item .env.example .env`
2. optional lokale Werte in `.env` setzen
3. `docker compose up --build -d`
4. `docker compose ps`

## Offizielle V1-Dateien

- `docker-compose.yml` ist die einzige versionierte Compose-Wahrheit.
- `config/mosquitto/mosquitto.conf` ist die einzige versionierte Mosquitto-Vorlage.
- `.env.example` ist die einzige versionierte Env-Vorlage.
- `nodered/build-flows.js` erzeugt die produktive `flows.json` beim Image-Build.
- `nodered/entrypoint.sh` initialisiert SQLite und startet danach Node-RED.
- `db/schema.sql` ist das versionierte SQLite-Basisschema.

## Lokal, nicht versioniert

- `.env`
- `config/.env`
- `config/mosquitto/config/` inklusive `passwd`
- lokale Compose-Overrides wie `docker-compose.override.yml` oder `docker-compose.local.yml`
- `nodered/_generated_v1_flows.tmp.json`

## Bewusst offen

- Broker-Authentifizierung / Secret-Haertung
- echte Wetterabfrage
- komplexe Automationen
- historische Diagramm-Abfragen aus Influx
- UI-Politur jenseits der V1-Grundstruktur
