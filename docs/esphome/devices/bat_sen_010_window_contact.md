# bat_sen_010 Window Contact

## Status

This file is a legacy ESPHome migration reference.

It is still valid, but it does not match the newer Home Assistant-native direction used by the current `net_*` ESPHome configs. It still uses the older MQTT-contract helper packages and deep-sleep reporting model.

## Overview

`bat_sen_010` is a battery-powered window contact with:

- reed contact input
- battery voltage reporting
- deep sleep
- MQTT-based state publishing

## Hardware Mapping

| Component | Pin | Notes |
|---|---|---|
| Reed contact | `GPIO3` | wake source and window state input |
| Setup button | `GPIO2` | manual setup / wake handling |
| Battery ADC | `GPIO4` | voltage measurement |

## Runtime Model

Typical wake cycle:

1. Wake by timer or contact change.
2. Read reed contact and battery voltage.
3. Connect to Wi-Fi and MQTT.
4. Publish availability, meta, state, and any event.
5. Keep a short receive window open for commands.
6. Return to deep sleep.

## Current Behavior

- sleep interval default: `900 s`
- receive window default: `5000 ms`
- contact changes can wake the device
- battery percentage is derived from the measured voltage

## Current ESPHome File

- [bat_sen_window_contact.yaml](D:/LocalRepos/esp32-smart-home/esphome/devices/bat_sen_window_contact.yaml)

## Important Note

This device is kept in the repo as a migration reference. If you want the current simpler public ESPHome direction, start with the `net_*` configs instead.
