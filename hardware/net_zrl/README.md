# NET-ZRL - 2-Relay Shutter Base Type

> `device_class: net_zrl` | `SH_CLASS_NET_ZRL (0x02)`

## Overview

NET-ZRL is the base type for **mains-powered two-relay nodes** with shutter / cover control. Safety comes from the serial relay wiring.

## KiCad sources

| Board | Path | Type |
|---|---|---|
| 🔑 Modular control board | [../shared/modularer_steuerkreis/](../shared/modularer_steuerkreis/) | **identical to net_erl** |
| ⚡ Power board (230V) | [leistungskreis/](leistungskreis/) | NET-ZRL specific (2 relays) |

### 2-relay power board

👉 **[leistungskreis_2relais.md](leistungskreis_2relais.md)** - full documentation with BOM

-> KiCad: `Leistungskreis_korrektur.{kicad_pro,sch,pcb}` in [leistungskreis/](leistungskreis/)

-> SVG schematic: [schematics/leistungskreis.svg](schematics/leistungskreis.svg)

-> **Important:** N-channel MOSFETs (IRLZ34N), 2x optocouplers, serial safety circuit

### Control board

👉 **[../shared/modularer_steuerkreis.md](../shared/modularer_steuerkreis.md)** - the same board as net_erl

-> KiCad: [../shared/modularer_steuerkreis/](../shared/modularer_steuerkreis/)

-> SVG: [../shared/schematics/steuerkreis.svg](../shared/schematics/steuerkreis.svg)

## Pin map

| GPIO | Function | Note |
|---|---|---|
| GPIO0 | I2C SDA | Optional |
| GPIO1 | I2C SCL | Optional |
| GPIO5 | Trigger_PIN2 | Relay 2 / Down (via PC817) |
| GPIO10 | Trigger_PIN1 | Relay 1 / Up (via PC817) |
| GPIO20 | RX | UART debug |
| GPIO21 | TX | UART debug |

## Devices

| Device | ID | Special behavior |
|---|---|---|
| 👉 [NET-ZRL-002](devices/NET-ZRL-002_shutter_module/) | shutter control | 2 relays, 3 buttons, calibration |
