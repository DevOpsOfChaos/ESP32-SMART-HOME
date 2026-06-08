---
layout: default
title: ESP32 Smart Home
---

# ESP32 Smart Home

This is the public landing page for the repository.

The project has two separate technical tracks:

| Track | Purpose | Core path |
|---|---|---|
| Custom firmware | Full project architecture | ESP-NOW -> Master -> MQTT -> local server |
| ESPHome | Simple Home Assistant adoption | ESPHome API -> Home Assistant |

That separation matters. ESPHome is not the same runtime stack rewritten in YAML. It is the easier path for people who want simple devices that fit naturally into Home Assistant.

## What the repository contains

- custom ESP32-C3 hardware
- a custom firmware stack with ESP-NOW and a master bridge
- a local server stack with Mosquitto, Node-RED, and SQLite
- a separate ESPHome track for Home Assistant users
- enclosure and fabrication assets
- validation scripts and notes

## Current ESPHome direction

The supported public ESPHome direction is Home Assistant-native and intentionally independent from the custom firmware architecture.

Supported current configs:

- `NET-ERL-010` hall module
- `NET-ERL-020` hall module with LED ring
- `NET-SEN-020` weather station
- `NET-ZRL-020` shutter module

Some older ESPHome MQTT-contract files still remain in the repo as migration references. They are valid transitional material, not the main public story.

## Current custom firmware direction

The custom firmware remains the core architecture for the full project:

- ESP-NOW between devices and the master
- MQTT between the master and the server
- Node-RED as the current dashboard and automation layer
- hardware-specific device firmware built on shared base types

## Read the right thing

- [Documentation](../docs/README.md)
- [Firmware](../firmware/README.md)
- [ESPHome](../esphome/README.md)
- [Server](../server/README.md)
- [Hardware](../hardware/README.md)
- [Tests](../tests/README.md)
- [Archive](../docs/archive/README.md)

## Reality check

This repository is a documented project state, not a polished product platform.

If you want the original full architecture, use `firmware/`.

If you want the easier Home Assistant path, use `esphome/`.
