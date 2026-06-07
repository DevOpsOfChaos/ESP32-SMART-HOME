# Modular Control Board (`mod_Steuerkreis_ESP32`)

## KiCad sources

The full KiCad project files are stored under:

[`shared/modularer_steuerkreis/`](modularer_steuerkreis/)

## Overview

The modular control board is the shared controller board for all relay-based devices in this project. It hosts the ESP32-C3 module and the reusable support circuitry that is common to `net_erl` and `net_zrl`.

## Bill of materials

| Component | Value / type | Quantity | Notes |
|---|---|---:|---|
| `IC1` | ESP32-C3 SuperMini Plus V2.0 | 1 | MCU module |
| `AD1` | ESP32-C3_SUPERMINI_TH | 1 | Through-hole adapter |
| `R1`, `R2` | 4.7 kOhm | 2 | I2C pull-ups, switchable by solder bridge |
| `R3` | 100 Ohm | 1 | LED / signal resistor |
| `R4` | 10 kOhm | 1 | Reset / pull-up support |
| `C1` | 100 nF | 1 | Decoupling |
| `C2` | 10 uF | 1 | Local supply buffer |
| `J1` | JST-PH 5-pin | 1 | Cable to the power board |
| `J2` | Header | 1 | Sensor breakout |
| `TP1-TP5` | Test points | 5 | Debug access |

## Pin mapping

### Control board to power board

| Signal | ESP32-C3 pin | Power board use |
|---|---|---|
| `5V` | VIN | Board supply |
| `GND` | GND | Common ground |
| `GPIO10` | GPIO10 | Relay control / trigger |
| `GPIO5` | GPIO5 | Second relay control / trigger |
| `3V3` | 3.3 V rail | Logic reference / sensors |

### GPIO breakouts

| GPIO | Breakout usage |
|---|---|
| GPIO0 | I2C SDA |
| GPIO1 | I2C SCL |
| GPIO2 | Sensor input / ADC |
| GPIO3 | Free |
| GPIO4 | LED ring data on `NET-ERL-002` |
| GPIO5 | Relay 2 / trigger |
| GPIO6 | Free |
| GPIO7 | Free |
| GPIO8 | Onboard LED |
| GPIO9 | Boot strap, not a normal I/O |
| GPIO10 | Relay 1 / trigger |
| GPIO20 | UART RX |
| GPIO21 | UART TX |

## SVG schematic

The board schematic is exported as SVG for quick inspection and documentation.

## Compatibility

| Item | Status |
|---|---|
| KiCad sources | [`shared/modularer_steuerkreis/`](modularer_steuerkreis/) |
| Relay families | `net_erl`, `net_zrl` |
| Sensor breakouts | Shared GPIO headers |
| Power board link | 5-pin JST-PH cable |

## Design features

- Shared control board for both relay families
- Switchable I2C pull-ups
- Sensor and debug breakouts
- Simple board-to-board cabling
- Designed for replacement without redesigning the whole device
