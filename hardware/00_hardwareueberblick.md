# Hardware Overview

> Status: May 2026 | Platform: ESP32-C3 (RISC-V) | Design tool: KiCad 9.0

## Architecture

All devices in this project are based on the **TENSTAR ESP32-C3 SuperMini Plus V2.0** (ESP32-C3, RISC-V 32-bit, 160 MHz, 4 MB flash, 400 KB SRAM, Wi-Fi b/g/n, BLE 5.0). The microcontroller is mounted on the carrier boards through a through-hole adapter board (`ESP32-C3_SUPERMINI_TH`).

Communication uses **ESP-NOW** for node-to-master traffic and **MQTT** for master-to-server traffic.

## Device families

| Family | Type | PCB design | Power | Quantity |
|---|---|---|---|---|
| `net_erl` | 1-relay node | Modular relay board | 230 V AC via HLK-5M05 | 2 devices |
| `net_zrl` | 2-relay blind controller | Modular relay board | 230 V AC via HLK-5M05 | 1 device |
| `net_sen` | Mains-powered sensor node | Mains sensor board | 230 V AC via HLK-PM03 | 1 device |
| `bat_sen` | Battery-powered sensor node | Battery sensor board | Battery / accumulator | 2 devices |
| `master` | Bridge node | ESP-NOW to MQTT bridge | 230 V AC | 1 device |

## Modular relay board design

All relay nodes (`net_erl`, `net_zrl`) use a **two-part modular design**:

1. **Control board** (`mod_Steuerkreis_ESP32`) - identical for all relay variants
   - ESP32-C3 SuperMini on a through-hole adapter
   - I2C pull-ups (4.7 kOhm, switchable by solder bridge)
   - GPIO pin headers for sensors
   - 5 test points for debugging
   - 2x M3 mounting holes

2. **Power board** - varies by relay type
   - `net_erl`: 1x relay (HF46F), 1x P-channel MOSFET (FQP27P06), 1x optocoupler (PC817)
   - `net_zrl`: 2x relays (HF46F), 2x N-channel MOSFETs (IRLZ34N), 2x optocouplers (PC817)
   - Both: Hi-Link AC/DC converter (230 V -> 5 V), resettable fuse (5x20 mm), 230 V terminals

The two boards are connected through a 5-pin JST-PH cable (5 V, GND, GPIO10, GPIO5, 3V3).

## Pin map matrix

| GPIO | BAT-SEN | NET-SEN | NET-ERL | NET-ZRL | Function |
|---|---|---|---|---|---|
| 0 | I2C SDA | I2C SDA | I2C SDA | I2C SDA | I2C data |
| 1 | I2C SCL | I2C SCL | I2C SCL | I2C SCL | I2C clock |
| 2 | Battery voltage sense | free | NC | NC | Battery ADC |
| 3 | free | free | NC | NC | - |
| 4 | free | free | LED ring (NET-ERL-002) | NC | WS2812 data pin for LED-ring module |
| 5 | free | free | Trigger_PIN2 | Trigger_PIN2 (down) | Relay 2 |
| 6 | free | free | free | free | - |
| 7 | free | free | free | free | - |
| 8 | free | free | free | free | Onboard LED |
| 9 | free | free | NC | NC | - |
| 10 | free | free | Trigger_PIN1 | Trigger_PIN1 (up) | Relay 1 |
| 20 | RX | RX | RX | RX | UART RX |
| 21 | TX | TX | TX | TX | UART TX |

## Power designs

| Type | Input | Converter | Output | Special note |
|---|---|---|---|---|
| BAT-SEN | Battery (3.0-4.2 V) | None (P-MOSFET gate, Zener protection) | 3.3 V (LDO on ESP board) | Deep-sleep optimized |
| NET-SEN | 230 V AC | HLK-PM03 | 3.3 V DC | Resettable fuse |
| NET-ERL power board | 230 V AC | HLK-5M05 | 5 V DC | Powers relay coil and ESP |
| NET-ZRL power board | 230 V AC | HLK-5M05 | 5 V DC | Powers 2 relay coils and ESP |

## Core design patterns

1. **I2C solder bridges (JP1/JP2):** the 4.7 kOhm I2C pull-ups can be enabled or disabled on every board by solder bridge
2. **Optocoupler isolation (PC817):** galvanic separation between the ESP GPIOs and the relay MOSFET drivers
3. **Flyback diodes (1N4007):** protection against inductive spikes from relay coils
4. **Test points (TP1-TP5):** debugging access on the modular control board
5. **Sensor pin headers:** all boards break out GPIOs, GND, and VCC on headers for external sensors

## Directory structure

```text
hardware/
├── 00_hardwareueberblick.md              ← this file
│
├── shared/                               ← shared design blocks
│   ├── esp32-c3-supermini.md             ← ESP32-C3 platform reference
│   ├── modularer_steuerkreis.md          ← control-board documentation (BOM, pins)
│   ├── komponenten.md                    ← component catalog
│   ├── esp32-c3-supermini/               ← KiCad: through-hole adapter
│   ├── modularer_steuerkreis/            ← KiCad: control board (shared by all relay nodes)
│   └── schematics/                       ← SVG schematics
│
├── master/                               ← master bridge
│   └── README.md
│
├── net_erl/                              ← base type: 1 relay
│   ├── README.md                         ← overview + pin map
│   ├── leistungskreis_1relais.md         ← power-board documentation
│   ├── leistungskreis/                   ← KiCad: power board
│   ├── simulation/                       ← LTSpice
│   ├── schematics/                       ← SVG schematics
│   └── devices/                          ← concrete device variants
│       ├── NET-ERL-001_hall_module/
│       └── NET-ERL-002_hall_module_led_ring/
│
├── net_zrl/                              ← base type: 2 relay / blind controller
│   ├── README.md
│   ├── leistungskreis_2relais.md
│   ├── leistungskreis/                   ← KiCad: power board (2 relays)
│   ├── schematics/
│   └── devices/
│       └── NET-ZRL-002_shutter_module/
│
├── net_sen/                              ← base type: mains sensor
│   ├── README.md
│   ├── kicad/                            ← KiCad: mains-sensor board
│   ├── schematics/
│   └── devices/
│       └── NET-SEN-002_weather_station/
│
└── bat_sen/                              ← base type: battery sensor
    ├── README.md
    ├── kicad/                            ← KiCad: battery-sensor board
    ├── schematics/
    └── devices/
        ├── bat_sen_01_window_contact/
        └── bat_sen_02_rain_sensor/
```

> **Legend:** `← KiCad:` = original KiCad project files (`.kicad_pro`, `.kicad_sch`, `.kicad_pcb`, BOM, STEP)

## KiCad sources

| Base type | Control board | Power board / board |
|---|---|---|
| **net_erl** | [shared/modularer_steuerkreis/](shared/modularer_steuerkreis/) | [net_erl/leistungskreis/](net_erl/leistungskreis/) |
| **net_zrl** | [shared/modularer_steuerkreis/](shared/modularer_steuerkreis/) | [net_zrl/leistungskreis/](net_zrl/leistungskreis/) |
| **net_sen** | - (single-board) | [net_sen/kicad/](net_sen/kicad/) |
| **bat_sen** | - (single-board) | [bat_sen/kicad/](bat_sen/kicad/) |
| **master** | - (no dedicated PCB) | - |

## Main components

| Component | Type | Used in |
|---|---|---|
| ESP32-C3 SuperMini Plus V2.0 | Microcontroller | All devices |
| ESP32-C3_SUPERMINI_TH | Through-hole adapter | All devices |
| HLK-5M05 | AC/DC converter 230 V -> 5 V | NET-ERL, NET-ZRL |
| HLK-PM03 | AC/DC converter 230 V -> 3.3 V | NET-SEN |
| Hongfa HF46F/5-HS1 | 5 V / 10 A relay | NET-ERL (1x), NET-ZRL (2x) |
| PC817 | Optocoupler | NET-ERL (1x), NET-ZRL (2x) |
| FQP27P06 | P-channel MOSFET, TO-220 | NET-ERL |
| IRLZ34N | N-channel MOSFET, TO-220 | NET-ZRL |
| 1N4007 | Flyback diode | All relay boards |
| BME280 | Temp / humidity / pressure (I2C 0x76) | NET-ERL-001, NET-SEN-002 |
| BME680 | Temp / humidity / pressure / gas (I2C 0x76) | NET-ERL-002 |
| VEML7700 | Lux sensor (I2C 0x10) | NET-ERL-001/002, NET-SEN-002 |
| ENS160 | MOX gas sensor (I2C 0x52) | NET-ERL-002 |
| LD2410 | Radar presence sensor (UART) | NET-ERL-002 |
| NeoPixel ring (12 LEDs) | WS2812 | NET-ERL-002 |
| BZX55C3V3 | 3.3 V Zener diode | BAT-SEN |
| IRF9530 | P-channel MOSFET, SOT-23 | BAT-SEN |
