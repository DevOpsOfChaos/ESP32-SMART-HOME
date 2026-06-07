# NET-SEN - Mains-Powered Sensor Base Type

> `device_class: net_sen` | `SH_CLASS_NET_SEN (0x03)`

## Overview

NET-SEN is the base type for **always-on mains-powered sensor nodes**. No sleep, always online.

## KiCad sources

👉 **[kicad/](kicad/)** - full KiCad project

| File | Description |
|---|---|
| `net_sen.kicad_pro` | project file |
| `net_sen.kicad_sch` | hierarchical root schematic |
| `Leistungskreis.kicad_sch` | sub-sheet: 230V power supply |
| `Steuerstromkreis.kicad_sch` | sub-sheet: ESP32 + I2C |
| `net_sen.kicad_pcb` | 2-layer PCB layout |
| `Bilder/` | layout views (PNG exports) |
| `production/` | BOM, placement, netlist |

### SVG schematics

- [schematics/net_sen.svg](schematics/net_sen.svg) - root
- [schematics/net_sen-Leistungskreis.svg](schematics/net_sen-Leistungskreis.svg) - power supply
- [schematics/net_sen-Steuerstromkreis.svg](schematics/net_sen-Steuerstromkreis.svg) - ESP32

## Board design

Single-board design, not modular:

- **HLK-PM03:** 230V AC -> 3.3V DC
- **Fuse:** 5x20mm
- **I2C:** GPIO0/1 with 4k7 pullups via solder bridges
- **230V input:** Phoenix MKDS 1.5/3

## Pin map

| GPIO | Function |
|---|---|
| GPIO0 | I2C SDA |
| GPIO1 | I2C SCL |
| GPIO2-10 | free (header) |
| GPIO20/21 | UART |

## Versions

1. **Standard version** (active) - in [kicad/](kicad/)
2. **TCA9548APWR version** - with I2C multiplexer (separate KiCad folder)

## Devices

| Device | ID | Sensors |
|---|---|---|
| 👉 [NET-SEN-002](devices/NET-SEN-002_weather_station/) | weather station | BME280, VEML7700, rain sensor |
