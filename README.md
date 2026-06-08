
# ESP32 Smart Home

An open, English-language release of my ESP32-based smart home project.

This repository packages the hardware, firmware, enclosure work, local server stack, and a separate ESPHome track in a way that can be used piece by piece.

What this is not: a polished consumer product or a cloud platform. It is a local-first, technically honest system that can be reused by builders who only want one part of it.

## What is included

- custom ESP32-C3 hardware
- modular PCB designs and production artifacts
- C++ firmware built with PlatformIO
- separate ESPHome device configs for Home Assistant users
- a local MQTT server and Node-RED dashboard
- 3D-printed enclosure designs
- test scripts and verification notes

## System overview

```text
Custom firmware path
  ESP32 devices
    -> ESP-NOW
    -> Master bridge
    -> MQTT
    -> Local server
         -> Mosquitto
         -> Node-RED dashboard and logic
         -> SQLite snapshots

Separate ESPHome path
  ESPHome devices
    -> Home Assistant
    -> Optional local integrations, depending on the device config
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
- ESPHome only: flash the Home Assistant-friendly YAMLs and ignore the custom firmware completely.
- Server only: run the local MQTT, Node-RED, and SQLite stack as a standalone dashboard backend.

## Repository map

| Path | Purpose |
|---|---|
| `hardware/` | source boards, shared circuits, enclosures, and manufacturing exports |
| `firmware/` | PlatformIO firmware, device families, shared protocol code |
| `esphome/` | separate Home Assistant-oriented ESPHome configs and migration references |
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

Validate the ESPHome configs from the repository root:

```powershell
.\scripts\check_esphome.ps1 -Device <device-name>
```

## ESPHome boundary

The `esphome/` area is not a firmware port of the custom stack. The supported public direction is simpler than that: easy-to-adopt ESPHome devices for people who want straightforward Home Assistant integration without the custom ESP-NOW or master architecture.

Some legacy MQTT-contract ESPHome files still remain in the repository as migration references. They are not the recommended public story.

## Design principle

Every major block is intentionally usable on its own. The full system is the reference implementation, not a mandatory bundle.

## Public release note

This repository is a cleaned public release. Secrets, runtime caches, and internal-only working artifacts are intentionally excluded.

## License

All rights reserved for now. The project is public for review and reuse, but no permissive license has been attached yet.

## Author

Manuel Ries
