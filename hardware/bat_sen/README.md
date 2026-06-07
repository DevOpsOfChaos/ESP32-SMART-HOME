# BAT-SEN Battery Sensor Base Type

> `device_class: bat_sen` | `SH_CLASS_BAT_SEN (0x04)`

## Overview

BAT-SEN is the base type for battery-powered sensor nodes with deep sleep. It is optimized for low standby current.

## KiCad sources

👉 **[kicad/](kicad/)** - full KiCad project

| File | Description |
|---|---|
| `bat_sen.kicad_pro` | project file |
| `bat_sen.kicad_sch` | hierarchical root schematic |
| `Leistungskreis.kicad_sch` | sub-sheet: power gating, battery |
| `Steuerstromkreis.kicad_sch` | sub-sheet: ESP32 + I2C |
| `bat_sen.kicad_pcb` | 2-layer PCB layout |
| `Pinbelegung/Pinbelegung.txt` | GPIO map |
| `Code/SamplecodeBasis.txt` | ESP-NOW deep-sleep example |
| `production/` | BOM, placement, netlist |

### SVG schematics

- [schematics/bat_sen.svg](schematics/bat_sen.svg) - root
- [schematics/bat_sen-Leistungskreis.svg](schematics/bat_sen-Leistungskreis.svg) - power gating
- [schematics/bat_sen-Steuerstromkreis.svg](schematics/bat_sen-Steuerstromkreis.svg) - ESP32

## Power-saving design

- **Power gating:** P-MOSFET (IRF9530, SOT-23) disconnects sensors during deep sleep
- **Zener diode:** BZX55C3V3 (DO-35) - overvoltage protection
- **Voltage divider:** R2+R5 (100k+100k) on GPIO4 - battery monitoring
- **Deep sleep:** about 0.5-0.8 mA board current
- **Wake-up:** timer (RTC) or external interrupt

## Pin map

| GPIO | Function | Note |
|---|---|---|
| GPIO0 | I2C SDA | Optional |
| GPIO1 | I2C SCL | Optional |
| GPIO2 | Setup button | Local bring-up button |
| GPIO4 | Voltage measurement (ADC) | Battery voltage via divider |
| GPIO3-10 | free | header pins |
| GPIO20/21 | UART | debug |

## Battery profiles

| Profile | Type | Voltage | Capacity |
|---|---|---|---|
| `BAT_PROFILE_CR2032` | CR2032 | 3.0V | ~225 mAh |
| `BAT_PROFILE_2X_AA` | 2x AA | 3.0V | ~2500 mAh |
| `BAT_PROFILE_2X_AAA` | 2x AAA | 3.0V | ~1000 mAh |

## Devices

| Device | ID | Sensors | Battery |
|---|---|---|---|
| 👉 [bat_sen_01](devices/bat_sen_01_window_contact/) | window contact | reed switch | 2x AAA in series |
| 👉 [bat_sen_02](devices/bat_sen_02_rain_sensor/) | rain sensor | ADC rain sensor | 2x AA |
