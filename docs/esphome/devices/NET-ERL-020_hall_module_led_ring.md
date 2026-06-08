# NET-ERL-020 Hall Module LED Ring

## Overview

`NET-ERL-020` is a Home Assistant-oriented ESPHome hall-light device with a local relay, a PIR motion input, a VEML7700 lux sensor, and a configurable 17-pixel LED ring.

This is no longer an ESPHome version of the old firmware feature stack. The current config is simpler and more honest:

- one local hall light
- one local motion state with hold time
- one local lux-based auto-light rule
- one configurable LED ring display

The old BME680, ENS160, and firmware-contract story does not describe the current YAML anymore.

## Exposed Entities

Typical Home Assistant entities from the current config:

- `light.hall_light`
- `binary_sensor.motion_detected`
- `sensor.ambient_illuminance`
- `switch.automatic_light_control`
- `switch.led_ring_motion_display`
- `number.motion_hold_time`
- `number.auto_light_lux_threshold`
- `number.led_ring_brightness_percent`
- `number.led_ring_color_red`
- `number.led_ring_color_green`
- `number.led_ring_color_blue`
- `number.led_ring_source_value`
- `number.led_ring_scale_minimum`
- `number.led_ring_scale_maximum`
- `number.led_ring_animation_speed`
- `select.led_ring_animation_mode`

Exact entity IDs can vary after adoption.

## Hardware Mapping

| Component | Pin / Bus | Notes |
|---|---|---|
| Relay | `GPIO10` | exposed as `Hall Light` |
| PIR | `GPIO7` | raw motion input |
| Local button | `GPIO6` | toggles the relay |
| VEML7700 | `I2C`, `SDA=GPIO0`, `SCL=GPIO1`, `0x10` | ambient lux input |
| LED ring | `GPIO4` | 17x WS2812B, `GRB` order |

## Local Motion Logic

The PIR does not map directly to the public motion entity.

Instead, the config keeps a held motion state:

1. Raw PIR goes high.
2. Motion becomes active immediately.
3. `Motion Hold Time` starts extending the active state.
4. After the raw PIR goes low, the held motion state remains on until the hold timer expires.

That makes the visible motion state less twitchy and gives the light logic a usable persistence window.

## Auto-Light Logic

The light logic is local to the ESP:

1. `Automatic Light Control` must be enabled.
2. Held motion must be active.
3. The lux value must be valid.
4. Lux must be at or below `Auto Light Lux Threshold`.
5. The relay turns on and the device marks the light as automation-owned.
6. When motion ends, the automation-owned light turns back off.

If the light was turned on manually, the config does not blindly treat it as automation-owned.

## LED Ring Logic

The LED ring is a local display, not a server contract feature.

It only lights while both of these are true:

- `LED Ring Motion Display` is enabled
- the held motion state is active

The visible bar is driven by `LED Ring Source Value`, mapped between the configured minimum and maximum scale values.

Available animation modes:

- `Static Bar`
- `Pulse Bar`
- `Comet Bar`
- `Scanner Bar`

Color and brightness are fully user-configurable from Home Assistant.

## Connectivity and Recovery

This device uses:

- ESPHome native API
- OTA password
- fallback AP with captive portal

Fallback AP:

- SSID: `net-erl-020-setup`
- password: `net-erl-setup`

## Current ESPHome File

- [net_erl_hall_module_led_ring.yaml](D:/LocalRepos/esp32-smart-home/esphome/devices/net_erl_hall_module_led_ring.yaml)

## Verification Checklist

1. Confirm the relay can be toggled from Home Assistant.
2. Confirm the local button toggles the relay.
3. Confirm `Motion Detected` stays on for the configured hold time.
4. Confirm the light only auto-turns on when lux is below the configured threshold.
5. Confirm the light turns back off when motion has ended.
6. Confirm the LED ring only lights while motion display is enabled and motion is active.
7. Confirm animation, color, brightness, and scale changes are reflected on the ring.
