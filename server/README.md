# Server

This folder contains the local backend stack for the project.

It is intentionally narrow: MQTT broker, Node-RED logic, and SQLite snapshots. No cloud layer, no remote account system, no unnecessary platform dependencies.

## Role

- Mosquitto as the local MQTT broker
- Node-RED for data ingestion, dashboard logic, and automation flows
- SQLite for lightweight persistence and snapshots

## What the server does

- accepts MQTT traffic from devices and ESPHome nodes
- routes device and master topics
- stores the latest device state
- provides a dashboard for overview and device detail views
- supports limited automations and actuator commands
- keeps the runtime local and inspectable

## What it deliberately does not try to be

- a full home automation platform
- a time-series database
- a generic message archive
- a cloud-controlled remote service
- a giant configuration product

## Start

From the repository root:

```powershell
cd server
Copy-Item .env.example .env
docker compose up -d
```

Open the dashboard:

```text
http://localhost:1880/ui
```

## Local broker warning

If you already use a separate MQTT broker, make sure `server/.env` and the ESPHome secrets point to the same broker. A broker split is the most common reason for a setup that looks correct but appears dead in the dashboard.

## Contract test

Run the smoke test from the repository root:

```powershell
.\tests\server\server_contract_smoke.ps1
```

If the stack is already running:

```powershell
.\tests\server\server_contract_smoke.ps1 -SkipStart
```

The test checks the current server contract, including the latest snapshot fields, ACK-related data, and startup/migration errors.

## Main files

- `docker-compose.yml`
- `.env.example`
- `sqlite/00_schema_phase1.sql`
- `nodered/settings.js`
- `nodered/flows/*.json`
- `nodered/lib/*.js`

## Local-only files

- `.env`
- runtime JSON state under `nodered/`
- local broker authentication data under `config/mosquitto/`
- temporary databases and caches
