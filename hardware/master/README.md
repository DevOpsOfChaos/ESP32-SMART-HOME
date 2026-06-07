# Master Bridge Base Type

> ESP-NOW ↔ MQTT bridge | `device_class: master` | `SH_CLASS_MASTER (0xFE)`

## Overview

The master is the central bridge between the ESP-NOW mesh network and the MQTT server (Node-RED on a Raspberry Pi).

> **No dedicated PCB design.** The master uses the ESP32-C3 SuperMini Plus V2.0 directly, with no carrier board.

## Hardware

| Property | Value |
|---|---|
| Board | TENSTAR ESP32-C3 SuperMini Plus V2.0 |
| MCU | Espressif ESP32-C3 (RISC-V, 160 MHz) |
| Flash / SRAM | 4 MB / 400 KB |
| Power | 5V DC via USB-C |
| Mode | always on (no sleep) |
| Wi-Fi | 2.4 GHz, connected to the local network |
| MQTT broker | mosquitto on Raspberry Pi |

## Layout

```text
ESP32-C3 SuperMini Plus V2.0
  USB-C -> 5V power
  ESP32-C3 RISC-V MCU
  PCB antenna + U.FL
  WS2812 status LED on GPIO8
  no external circuitry
  no carrier board
  no sensors
```

## Pin usage

| GPIO | Function | Note |
|---|---|---|
| GPIO8 | WS2812 RGB LED | onboard status indicator |
| GPIO20 | UART RX | internal debug |
| GPIO21 | UART TX | internal debug |

All other GPIOs are unused. The master has no sensors, no relays, and no buttons.

## ESP-NOW ↔ MQTT flow

```text
BAT-SEN -> 
NET-SEN -> ESP-NOW -> Master -> MQTT -> Node-RED
NET-ERL ->                 ->           -> SQLite
NET-ZRL ->                             -> Dashboard
```

## Firmware

- **Path:** `firmware/src/basetypes/master_firmware/`
- **main.cpp:** about 2700 lines
- **Platform:** PlatformIO, `espressif32`, Arduino framework

### Core functions

- ESP-NOW peer management and dynamic registry
- MQTT ingest pipeline (`smarthome/device/+/state`)
- JSON serialization of device states
- hello / heartbeat tracking
- diagnostics endpoints

## Device IDs

- **Primary:** `MASTER-001`
- **Fallback:** `MASTER-002`

## Differences vs. other base types

| Property | Master | net_erl / net_zrl | net_sen / bat_sen |
|---|---|---|---|
| Own PCB | no | yes (modular) | yes (single-board) |
| KiCad project | no | yes | yes |
| Sensors | no | optional (I2C) | yes |
| Actuators | no | yes (relays) | no |
| Sleep | no | no | bat_sen: yes |
| Wi-Fi | yes (always on) | no (ESP-NOW only) | no (ESP-NOW only) |
| MQTT | yes | no | no |

## Board reference

👉 [../shared/esp32-c3-supermini.md](../shared/esp32-c3-supermini.md) - technical data for the ESP32-C3 SuperMini Plus
