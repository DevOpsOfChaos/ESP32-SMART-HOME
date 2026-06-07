
# ESP32 Smart Home

An open, English-language release of my ESP32-based smart home project.

This repository packages the hardware, firmware, enclosure work, local server stack, and ESPHome alternatives in a way that can be used piece by piece.

What this is not: a polished consumer product or a cloud platform. It is a local-first, technically honest system that can be reused by builders who only want one part of it.

## What is included

- custom ESP32-C3 hardware
- modular PCB designs and production artifacts
- C++ firmware built with PlatformIO
- an ESPHome line for Home Assistant users
- a local MQTT server and Node-RED dashboard
- 3D-printed enclosure designs
- test scripts and verification notes

## System overview

```text
ESP32 devices
  -> ESP-NOW
  -> Master bridge
  -> MQTT
  -> Local server
       -> Mosquitto
       -> Node-RED dashboard and logic
       -> SQLite snapshots

ESPHome devices
  -> MQTT
  -> Local server
```

## Device families

| Family | Purpose | Typical modules |
|---|---|---|
| `net_erl` | mains-powered relay node | hall light, room light, sensor extensions |
| `net_zrl` | shutter / blind controller | two relays, calibration, local buttons |
| `net_sen` | mains-powered sensor node | environmental sensing, rain input |
| `bat_sen` | battery-powered sensor node | reed contact, rain sensor, deep sleep |
| `master` | bridge node | ESP-NOW to MQTT gateway |

## Use each part independently

- Hardware only: build the PCB and enclosure files without using my firmware.
- Firmware only: reuse the PlatformIO project for your own ESP32-C3 builds.
- ESPHome only: flash the ready-made Home Assistant-friendly YAMLs and ignore the custom firmware.
- Server only: run the local MQTT, Node-RED, and SQLite stack as a standalone dashboard backend.

## Repository map

| Path | Purpose |
|---|---|
| `hardware/` | source boards, shared circuits, enclosures, and manufacturing exports |
| `firmware/` | PlatformIO firmware, device families, shared protocol code |
| `esphome/` | alternative MQTT-first device definitions for Home Assistant |
| `server/` | local Mosquitto, Node-RED, and SQLite stack |
| `docs/` | English overview and legacy reference material |
| `tests/` | smoke tests and contract checks |

Start here if you want the shortest onboarding path:

- [Getting Started](docs/GETTING_STARTED.md)

## Quick start

Run the local dashboard stack:

```powershell
cd server
docker compose up -d
```

Open the dashboard:

```text
http://localhost:1880/ui
```

Build a firmware target:

```powershell
cd firmware
pio run -e <environment-name>
```

Validate an ESPHome device from the repository root:

```powershell
.\scripts\check_esphome.ps1 -Device <device-name>
```

## Design principle

Every major block is intentionally usable on its own. The full system is the reference implementation, not a mandatory bundle.

## Public release note

This repository is a cleaned public release. Secrets, runtime caches, and internal-only working artifacts are intentionally excluded.

## License

All rights reserved for now. The project is public for review and reuse, but no permissive license has been attached yet.

## Author

Manuel Ries
