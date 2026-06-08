# NET-SEN-020 Weather Station

## Overview

`NET-SEN-020` is a Home Assistant-oriented ESPHome weather station built around:

- BME280 for temperature, humidity, and pressure
- VEML7700 for ambient light
- one digital rain input

This config is a direct ESPHome/Home Assistant device. It is not part of the custom firmware architecture.

## Exposed Entities

Typical Home Assistant entities from the current config:

- `sensor.net_sen_weather_station_temperature`
- `sensor.net_sen_weather_station_humidity`
- `sensor.net_sen_weather_station_pressure`
- `sensor.net_sen_weather_station_illuminance`
- `binary_sensor.net_sen_weather_station_rain`
- `number.net_sen_weather_station_sensor_interval`
- `button.net_sen_weather_station_restart`
- `button.net_sen_weather_station_restart_in_safe_mode`
- `binary_sensor.net_sen_weather_station_status`
- `text_sensor.net_sen_weather_station_ip_address`
- `text_sensor.net_sen_weather_station_connected_ssid`
- `text_sensor.net_sen_weather_station_connected_bssid`

Exact entity IDs can vary after adoption.

## Hardware Mapping

| Component | Pin / Bus | Notes |
|---|---|---|
| BME280 | `I2C`, `SDA=GPIO0`, `SCL=GPIO1`, `0x76` | temperature, humidity, pressure |
| VEML7700 | `I2C`, `SDA=GPIO0`, `SCL=GPIO1`, `0x10` | ambient illuminance |
| Rain input | `GPIO3` | inverted, pull-up |
| Setup button | `GPIO2` | diagnostic entity, also a strapping pin |

## Sampling Model

The sensor polling interval is configurable from Home Assistant through `Sensor Interval`.

Current defaults:

- default: `10 s`
- range: `5 s` to `600 s`

That interval is applied to both the BME280 component and the VEML7700 component.

## Calibration Inputs

The YAML currently supports fixed substitution-based offsets for:

- temperature: `temp_offset_c`
- humidity: `hum_offset_pct`

Those are compile-time values in the current config, not runtime entities.

## Connectivity and Recovery

This device uses:

- ESPHome native API
- OTA password
- fallback AP with captive portal
- local ESPHome web server on port `81`

Fallback AP:

- SSID: `net-sen-020-setup`
- password: `net-sen-setup`

## Current ESPHome File

- [net_sen_weather_station.yaml](D:/LocalRepos/esp32-smart-home/esphome/devices/net_sen_weather_station.yaml)

## Validation Note

`GPIO2` is used for the setup button. ESPHome validates the config, but it also warns that `GPIO2` is a strapping pin. That is acceptable only if the hardware behavior is already understood and verified on the real board.

## Verification Checklist

1. Confirm temperature, humidity, pressure, and illuminance values appear in Home Assistant.
2. Confirm the rain entity changes when the rain input is toggled.
3. Change `Sensor Interval` and confirm the update cadence actually changes.
4. Confirm the fallback AP and captive portal work when Wi-Fi credentials are wrong.
5. Confirm the web server is reachable on port `81` on the local network.
