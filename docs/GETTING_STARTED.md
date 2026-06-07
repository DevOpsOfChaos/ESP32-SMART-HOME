# Getting Started

This is the shortest useful path for a first-time user.

## 1. Pick your entry point

- `firmware/` if you want the custom ESP-NOW + MQTT line
- `esphome/` if you want the Home Assistant-friendly MQTT line
- `server/` if you want the local MQTT / Node-RED / SQLite stack
- `hardware/` if you want the PCB and enclosure work

## 2. Start the local server

```powershell
cd server
Copy-Item .env.example .env
docker compose up -d
```

Open the dashboard:

```text
http://localhost:1880/ui
```

## 3. Choose a device family

- `net_erl` for relay and hall-light style devices
- `net_zrl` for shutter / blind control
- `net_sen` for mains-powered sensors
- `bat_sen` for battery-powered sensors

## 4. Decide whether you want custom firmware or ESPHome

Use the custom firmware if you want the original project architecture. Use ESPHome if you want a simpler Home Assistant workflow and direct MQTT.

## 5. Read only what you need

- [Root README](../README.md)
- [Hardware overview](../hardware/README.md)
- [Firmware overview](../firmware/README.md)
- [Server overview](../server/README.md)
- [ESPHome overview](../esphome/README.md)

