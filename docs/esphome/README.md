# ESPHome Devices - Documentation

## Overview

The ESPHome line is an alternative firmware implementation for the smart home devices. Unlike the custom firmware line (ESP-NOW -> Master -> MQTT), ESPHome devices communicate **directly over MQTT** with the server, without an ESP-NOW master as an intermediate layer.

## Architecture

```text
ESPHome device (ESP32-C3) <-> MQTT directly <-> Server (Node-RED)
```

No master. No ESP-NOW. Each device is an independent MQTT client with full topic management (birth / last will, meta, state, events, commands).

## Device overview

| Device | ESPHome ID | Type | Sensors | Highlights |
|---|---|---|---|---|
| [NET-ERL-010](devices/NET-ERL-010_hall_module.md) | `NET-ERL-010` | mains | BME280, VEML7700, PIR | 1 relay, motion event |
| [NET-ERL-020](devices/NET-ERL-020_hall_module_led_ring.md) | `NET-ERL-020` | mains | BME680, VEML7700, ENS160, LD2410 | 1 relay, LED ring, auto-light |
| [NET-SEN-020](devices/NET-SEN-020_weather_station.md) | `NET-SEN-020` | mains | BME280, VEML7700 | rain sensor (GPIO) |
| [NET-ZRL-020](devices/NET-ZRL-020_shutter_module.md) | `NET-ZRL-020` | mains | - | 2 relays, cover calibration |
| [bat_sen_010](devices/bat_sen_010_window_contact.md) | `bat_sen_010` | battery | reed contact | deep sleep (15 min), window event |
| [bat_sen_020](devices/bat_sen_020_rain_sensor.md) | `bat_sen_020` | battery | rain sensor (ADC) | deep sleep (15 min), rain event |

## Shared concepts

### MQTT contract
All ESPHome devices implement the same MQTT contract as the firmware line. See [Firmware MQTT contract](../firmware/03_mqtt_contract_referenz.md) for details.

The ESPHome line uses its own device IDs (3-digit suffix, for example `NET-ERL-020`). MQTT topics are case-sensitive. Battery devices use lowercase IDs with underscores (`bat_sen_010`).

### Events
All devices publish events on `smarthome/device/{id}/event`:

- motion, window, rain, button, relay switching
- 18 event types with trigger source and parameters

### Sensor offsets
Temperature and humidity can be calibrated through YAML substitutions (`temp_offset_01c`, `hum_offset_01pct`). Offsets are stored in tenths of degrees / percent.

### Setup mode
Any device can be put into setup mode by holding the setup button for 5 seconds. Setup mode starts a Wi-Fi access point (SSID = device ID) with a web interface for master MAC and device parameter configuration.

### Code structure
Each ESPHome device consists of:

- **Device YAML** (`esphome/devices/{name}.yaml`): device-specific configuration
- **Shared packages** (`esphome/packages/`): reusable building blocks
  - `smarthome_contract_base.yaml`: MQTT meta, availability, provisioning UI
  - `smarthome_command_ack.yaml`: command ACK system
  - `smarthome_device_event.yaml`: event publishing
  - `smarthome_cover_contract.yaml`: shutter logic (NET-ZRL only)
  - `smarthome_command_dispatch.h`: shared C++ functions (MAC validation)

## Differences vs. the firmware line

| Aspect | ESPHome | Firmware |
|---|---|---|
| Communication | MQTT directly | ESP-NOW -> Master -> MQTT |
| Configuration | YAML substitutions | C++ `DeviceConfig.h` |
| Sensor filtering | 16x oversampling + IIR | EMA filter (alpha = 0.2) |
| Events | all 18 types | 15 types (ESP-NOW only) |
| Deep sleep | ESPHome `deep_sleep` | manual `esp_deep_sleep_start()` |
| Cover | `smarthome_cover_contract.yaml` | inline C++ state machine |
