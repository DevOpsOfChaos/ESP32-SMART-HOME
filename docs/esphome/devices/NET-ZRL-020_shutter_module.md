# NET-ZRL-020 Shutter Module

## Overview

`NET-ZRL-020` is a Home Assistant-oriented ESPHome shutter controller with:

- two relay outputs
- three local buttons
- two local status LEDs
- native Home Assistant cover control
- local travel-time calibration and position estimation

This is not the old MQTT contract device anymore. The current config is a native ESPHome cover implementation.

## Exposed Entities

Typical Home Assistant entities from the current config:

- `cover.net_zrl_shutter`
- `number.net_zrl_shutter_default_travel_time`
- `number.net_zrl_shutter_travel_up_time`
- `number.net_zrl_shutter_travel_down_time`
- `select.net_zrl_shutter_relay_up_mapping`
- `button.net_zrl_shutter_start_calibration`
- `button.net_zrl_shutter_clear_calibration`
- `button.net_zrl_shutter_restart`
- `text_sensor.net_zrl_shutter_status`
- `text_sensor.net_zrl_shutter_ip_address`

Exact entity IDs can vary after adoption.

## Hardware Mapping

| Component | Pin | Notes |
|---|---|---|
| Relay up | `GPIO10` | interlocked with relay down |
| Relay down | `GPIO5` | interlocked with relay up |
| Button up | `GPIO20` | local open control |
| Button down | `GPIO4` | local close control, long hold restarts the device |
| Button stop | `GPIO3` | stop action, long hold starts calibration |
| LED up | `GPIO7` | movement / calibration indicator |
| LED down | `GPIO6` | movement / calibration indicator |

## Cover Behavior

The device exposes a template cover and manages relay movement locally.

Supported local behaviors:

- open
- close
- stop
- target position after calibration

Intermediate positions are only meaningful once travel times are known. Before calibration, full open and full close are still allowed, but partial positioning is intentionally blocked.

## Calibration Model

The calibration flow is fully local:

1. Start calibration.
2. Move to the top end.
3. Wait for the down phase to begin.
4. Measure the full down run.
5. Wait for the up phase to begin.
6. Measure the full up run.
7. Store both travel times.

Current persistence:

- travel up time is stored
- travel down time is stored
- calibration state is restored across reboot

If calibration is cleared, the device forgets the travel times and partial position accuracy is gone.

## Local Button Logic

The button behavior is more opinionated than a generic cover module:

- `UP`: opens the cover, or advances the calibration flow when waiting for the up phase
- `DOWN`: closes the cover on short press, restarts the device on a 5-second hold, or advances calibration when waiting for the down phase
- `STOP`: stops motion immediately; holding for 5 seconds while idle starts calibration

That behavior is efficient once learned, but it is not self-explanatory. Anyone using this hardware needs to know it up front.

## Relay Mapping

`Relay Up Mapping` lets you swap which relay direction counts as "up."

Options:

- `relay_a`
- `relay_b`

That exists because real shutter wiring is messy and pretending otherwise is naive.

## Status Text

The current status text can report:

- `open`
- `closed`
- `opening`
- `closing`
- `stopped`
- one of several calibration states

## Connectivity and Recovery

This device uses:

- ESPHome native API
- OTA password
- fallback AP with captive portal
- local web server

Fallback AP:

- SSID: `net-zrl-020-setup`
- password: `net-zrl-setup`

## Current ESPHome File

- [net_zrl_shutter_module.yaml](D:/LocalRepos/esp32-smart-home/esphome/devices/net_zrl_shutter_module.yaml)

## Verification Checklist

1. Confirm open, close, and stop work from Home Assistant.
2. Confirm the relays never energize both directions at once.
3. Confirm long-hold and short-press button actions match the documented behavior.
4. Run a full calibration and confirm travel times are stored.
5. Confirm intermediate positioning works only after calibration.
6. Power-cycle the device and confirm the stored travel times persist.
