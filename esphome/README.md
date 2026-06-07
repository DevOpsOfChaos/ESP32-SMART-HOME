# ESPHome

This folder is the alternative device line for Home Assistant users.

It bypasses the custom ESP-NOW master bridge and talks MQTT directly to the local server contract.

## Why it exists

- easier adoption for Home Assistant users
- no dependency on the custom master bridge
- each device can be flashed and used on its own

## Important rule

The MQTT topic layout and payload fields must stay compatible with the server side. The dashboard should not care whether a device runs custom firmware or ESPHome.

## Use

Typical workflow:

1. copy the folder into your Home Assistant ESPHome workspace
2. provide your local secrets
3. validate or compile one device at a time
4. flash once by cable, then use OTA for updates

## Validation

Use the Windows helper scripts to validate configuration or compile a device target.

## Device set

The repository includes ESPHome definitions for the finished device families so Home Assistant users can reuse the hardware without adopting the custom firmware stack.
