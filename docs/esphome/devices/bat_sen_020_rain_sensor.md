# bat_sen_020 Rain Sensor

## Status

This file is a legacy ESPHome migration reference.

It is still valid, but it belongs to the older MQTT-contract ESPHome path rather than the newer Home Assistant-native direction used by the current `net_*` configs.

## Overview

`bat_sen_020` is a battery-powered rain sensor with:

- analog rain input
- battery voltage reporting
- hysteresis-based wet/dry detection
- deep sleep
- MQTT-based reporting

## Hardware Mapping

| Component | Pin | Notes |
|---|---|---|
| Rain ADC | `GPIO3` | analog rain measurement |
| Setup button | `GPIO2` | manual setup / wake handling |
| Battery ADC | `GPIO4` | voltage measurement |

## Runtime Model

Typical wake cycle:

1. Wake by timer.
2. Read rain ADC and battery voltage.
3. Apply wet/dry hysteresis.
4. Connect to Wi-Fi and MQTT.
5. Publish availability, meta, state, and any event.
6. Keep a short receive window open for commands.
7. Return to deep sleep.

## Current Behavior

- sleep interval default: `900 s`
- receive window default: `5000 ms`
- wet threshold: `2200`
- clear threshold: `2050`

## Current ESPHome File

- [bat_sen_rain_sensor.yaml](D:/LocalRepos/esp32-smart-home/esphome/devices/bat_sen_rain_sensor.yaml)

## Important Note

This device remains in the repository because it still compiles and documents useful hardware behavior, but it is not the lead public ESPHome story anymore.
