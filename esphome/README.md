# ESPHome

This folder is the separate ESPHome track for people who want simple Home Assistant devices.

The supported public direction is not "the same firmware architecture in YAML." It is the opposite: independent ESPHome devices that do not depend on the custom ESP-NOW transport, the master bridge, or the firmware-side MQTT contract.

## What this folder is for

- straightforward Home Assistant adoption
- device-local logic where it makes sense
- easier reuse of the hardware without adopting the custom firmware stack
- clearer separation from the custom `firmware/` architecture

## What is currently in here

Supported public direction:

- `net_erl_hall_module.yaml`
- `net_erl_hall_module_led_ring.yaml`
- `net_sen_weather_station.yaml`
- `net_zrl_shutter_module.yaml`

These configs are Home Assistant-oriented ESPHome devices using the native ESPHome API.

Legacy migration references still kept in the repo:

- `bat_sen_window_contact.yaml`
- `bat_sen_rain_sensor.yaml`
- `packages/`

Those files still use the older MQTT-contract helper packages. They remain valid, but they are not the simplified public direction anymore.

## Use

Typical workflow:

1. copy the device YAML you actually want into your ESPHome workspace
2. provide your local secrets
3. validate the config
4. flash once by cable, then use OTA
5. adopt the device in Home Assistant

## Validation

Use the Windows helper scripts to validate configuration or compile a device target.

```powershell
.\scripts\check_esphome.ps1
```

Or only validate a specific device:

```powershell
.\scripts\check_esphome.ps1 -Device net_erl_hall_module
```
