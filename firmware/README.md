# Firmware

This folder contains the PlatformIO firmware for the custom ESP32-C3 line.

It is the main code path for the project when you want the full custom stack:

- ESP-NOW device communication
- a master bridge to MQTT
- shared protocol and provisioning helpers
- concrete device targets for each hardware family

## Layout

- `src/basetypes/` contains reusable runtime bases for each device family
- `src/devices/` contains concrete device builds
- `include/` contains shared headers and local secret templates
- `lib/` contains protocol, provisioning, and storage helpers
- `scripts/` contains build helpers

## Use this track when

Use `firmware/` if you want full control over timing, provisioning, transport, and the device model.

If you mainly want simpler Home Assistant devices, use the separate `esphome/` folder instead. That is not another variant of this firmware stack. It is its own track.

## Build

Build a target with PlatformIO:

```powershell
cd firmware
pio run -e <environment-name>
```

Upload follows the same pattern once the target is selected.

## Secrets

Do not commit `include/Secrets.h`. Keep the example file as the template and copy it locally when needed.

## What matters here

This is not a generic firmware dump. The useful parts are the reusable protocol layer, the base types, and the concrete device implementations that prove the system works end to end.
