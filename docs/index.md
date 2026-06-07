---
layout: default
title: ESP32 Smart Home
---

# ESP32 Smart Home

This is the public project page for the cleaned English release of the repository.

The project shows a local, modular ESP32-based smart home system. It is meant for readers who want to understand how sensors, actuators, wireless transport, server logic, dashboarding, and verification can be combined into one controlled project.

## What it is

The system is built from:

- ESP32 devices for sensing and actuation
- a local server stack with MQTT ingest, a Node-RED dashboard, and SQLite snapshots
- clear device contracts for `meta`, `availability`, `state`, `event`, `ack`, and `command`
- traceable tests and notes instead of architecture claims without proof

There are two usable firmware lines:

| Line | Summary | Best for |
|---|---|---|
| Custom firmware (main line) | ESP-NOW between devices and master, MQTT only between master and server | full control, custom transport, core project line |
| ESPHome alternative | devices speak directly to the same MQTT contract | Home Assistant users and YAML-first workflows |

The shared point is the server contract. Node-RED should see the same logical device state regardless of whether a device comes from the custom firmware line or from ESPHome.

## Current technical line

The official main line stays intentionally narrow:

- ESP-NOW between distributed devices and master
- MQTT between master and server
- the master as the bridge for the custom firmware line
- Node-RED as the current server and visualization core
- base types instead of uncontrolled one-off behavior

The ESPHome line is a deliberately separate alternative. It does not use an ESP-NOW master. It uses direct MQTT, but stays compatible with the topic and payload contract.

## Public documentation

The English public docs now live in the main `docs/` entry points. Legacy German reference material has been moved to `docs/archive/german-reference/`.

## Confirmed public state

The current public state already shows a stable line of the server core and concrete real device paths.

Visible and verified:

- MQTT ingest for devices and master
- shared device state model
- minimal SQLite foundation
- dashboard V1 baseline
- `net_zrl_shutter_module` as the shutter path
- working `net_erl_hall_module`
- prepared `net_erl_hall_module_led_ring`
- confirmed setup path for the hall module
- working `net_sen_weather_station`
- confirmed setup path for `net_sen`
- prepared ESPHome YAMLs for `net_erl`, `net_sen`, `net_zrl`, and `bat_sen`
- real verification notes in the archive

Important:
This is **not** a finished product platform. It is a controlled, documented project state. The custom firmware line is the architectural main line; ESPHome is the practical alternative for users who want the same server contract but prefer to build devices through Home Assistant / ESPHome.

## Repo areas

- [Documentation](../docs/README.md)
- [Firmware](../firmware/README.md)
- [ESPHome alternative](../esphome/README.md)
- [Server](../server/README.md)
- [Hardware](../hardware/README.md)
- [Tests](../tests/README.md)
- [Archive](../docs/archive/README.md)

## Notes

- The public repo stays technically direct and system-neutral.
- Internal or private working states do not belong here.
- Small, traceable steps matter more than broad uncontrolled refactors.
