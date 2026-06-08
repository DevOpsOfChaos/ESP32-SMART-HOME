# NET-ERL-010 Hall Module

## Overview

`NET-ERL-010` is a plain ESPHome hall-light controller for Home Assistant.

The device keeps the actual motion-light logic locally on the ESP, so it still works even if Home Assistant is temporarily unavailable. Home Assistant is used to view the entities and change the configuration values.

## Current Features

- One relay exposed as a light
- One PIR motion sensor
- One VEML7700 illuminance sensor
- Local automatic switch-on when motion is detected and the lux value is below the configured threshold
- Local automatic switch-off after the configured delay once motion ends
- Home Assistant entities for changing the automation settings
- Lux polling only while the light is off, to avoid pointless self-illumination readings

Temperature and humidity are intentionally not part of this device anymore.

## Exposed Entities

After flashing and adopting the device, Home Assistant should show entities similar to these:

- `light.net_erl_hall_module_light`
- `binary_sensor.net_erl_hall_module_motion`
- `sensor.net_erl_hall_module_illuminance`
- `switch.net_erl_hall_module_auto_light`
- `number.net_erl_hall_module_lux_threshold`
- `number.net_erl_hall_module_auto_off_delay`

Entity IDs can differ slightly depending on your naming conventions in Home Assistant.

## Hardware Mapping

| Component | Pin / Bus | Notes |
| --- | --- | --- |
| Relay | `GPIO10` | Exposed as a light |
| PIR motion sensor | `GPIO7` | Exposed as a motion binary sensor |
| VEML7700 | `I2C`, `SDA=GPIO0`, `SCL=GPIO1`, `0x10` | Exposed as illuminance, polled only while light is off |

## Automation Behavior

The local device logic works like this:

1. Motion goes `on`
2. The device checks whether auto-light is enabled
3. The device checks whether the VEML7700 has a valid lux reading
4. The device compares the current lux value against the configured threshold
5. If the measured lux is at or below the threshold and the light is currently off, the relay turns on
6. When motion goes `off`, the device starts an internal off-timer
7. If motion is detected again before the timer expires, the timer is cancelled
8. If the timer expires and motion is still off, the light turns off

The VEML7700 is not continuously polled. It is updated once per minute and only while the light is off. That avoids wasting reads and avoids using obviously contaminated lux values caused by the lamp itself.

This means the basic automation does not depend on a live Home Assistant connection.

## Configurable Values

These settings are exposed to Home Assistant as normal entities:

### Auto Light

- Entity: `switch.net_erl_hall_module_auto_light`
- Type: on/off
- Purpose: enables or disables the local automation

If disabled, the device stops doing automatic on/off control. The light can still be switched manually from Home Assistant.

### Lux Threshold

- Entity: `number.net_erl_hall_module_lux_threshold`
- Unit: `lx`
- Default: `80`
- Range: `0` to `1000`

The light only turns on automatically if the measured illuminance is less than or equal to this value.

### Auto Off Delay

- Entity: `number.net_erl_hall_module_auto_off_delay`
- Unit: `s`
- Default: `120`
- Range: `10` to `1800`

This is the run-on time after motion ends.

## Important Behavior Details

### Manual Light Control

If the light is turned on manually while auto-light is enabled, the device does not automatically assume that this was an automation-triggered light cycle. Auto-off is only applied to light cycles that were started automatically by the device.

That is intentional. Otherwise the automation would interfere with manual control too aggressively.

### No Fallback Access Point

The current configuration does not use an ESPHome fallback AP or `captive_portal`.

That keeps the device simpler, but it also means:

- if Wi-Fi credentials are wrong
- or the WLAN environment changes badly

you do not have a built-in wireless recovery path. In that case you need to reflash or recover the device another way.

## Current ESPHome File

The live device configuration is:

- [net_erl_hall_module.yaml](D:/LocalRepos/esp32-smart-home/esphome/devices/net_erl_hall_module.yaml)

## Verification Checklist

Do not trust assumptions. Verify the real behavior:

1. Confirm the device appears online in Home Assistant.
2. Confirm the motion entity switches between `off` and `on`.
3. Confirm the illuminance sensor reports plausible values.
4. Set the lux threshold high enough to force an automatic switch-on test.
5. Trigger motion and confirm the light turns on locally.
6. Stop moving and confirm the light turns off after the configured delay.
7. Trigger motion again during the delay and confirm the timer is cancelled.
8. Turn the light on manually and confirm that manual control is not unexpectedly forced back off by an unrelated old timer.
