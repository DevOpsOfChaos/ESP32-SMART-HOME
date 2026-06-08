# ESPHome Devices

## Overview

The ESPHome area is the separate Home Assistant-oriented device track in this repository.

The supported public direction is simple:

- Home Assistant-native ESPHome devices
- local device logic where needed
- no dependency on the custom ESP-NOW transport
- no dependency on the master bridge
- no requirement to mirror the firmware-side MQTT contract

That is the whole point. If someone wants simple devices that are easy to adopt in Home Assistant, this is the path.

## Supported current configs

| Device | ESPHome ID | Type | Main role | Notes |
|---|---|---|---|---|
| [NET-ERL-010](devices/NET-ERL-010_hall_module.md) | `net-erl-010` | mains | hall light | local motion + lux automation |
| [NET-ERL-020](devices/NET-ERL-020_hall_module_led_ring.md) | `net-erl-020` | mains | hall light + LED ring | configurable motion hold and ring display |
| [NET-SEN-020](devices/NET-SEN-020_weather_station.md) | `net-sen-020` | mains | weather station | BME280 + VEML7700 + rain input |
| [NET-ZRL-020](devices/NET-ZRL-020_shutter_module.md) | `net-zrl-020` | mains | shutter controller | native HA cover entity with local calibration |

These four files are the public direction that matches the current repo messaging.

## Legacy migration references

| Device | Status | Why it is different |
|---|---|---|
| [bat_sen_010](devices/bat_sen_010_window_contact.md) | legacy / transitional | still uses the older MQTT-contract helper packages |
| [bat_sen_020](devices/bat_sen_020_rain_sensor.md) | legacy / transitional | still uses the older MQTT-contract helper packages |

Those files are still valid ESPHome configs, but they belong to the older migration path. They are not the clean "simple Home Assistant device" story anymore.

## Shared usage pattern

1. Copy the target YAML into your ESPHome workspace.
2. Provide the required secrets.
3. Validate the config.
4. Flash the device once by cable.
5. Use OTA and Home Assistant adoption after that.

## Validation

From the repository root:

```powershell
.\scripts\check_esphome.ps1
```

Or validate one target:

```powershell
.\scripts\check_esphome.ps1 -Device net_zrl_shutter_module
```

## Important boundary

Do not promise more uniformity than the repo actually has.

`firmware/` is the full custom architecture.

`esphome/` is the easier Home Assistant track.

Some legacy MQTT-oriented ESPHome files still exist during the transition, and the docs now call that out explicitly instead of pretending everything is one unified stack.
