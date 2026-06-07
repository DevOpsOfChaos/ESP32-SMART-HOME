# Shared Component Reference

This page lists the component families that appear across the hardware designs in this repository.

## Related KiCad sources

| Component | KiCad files |
|---|---|
| ESP32-C3 SuperMini Plus V2.0 | `esp32-c3-supermini/` |
| ESP32-C3_SUPERMINI_TH | `esp32-c3-supermini/` |
| Modular control board | `modularer_steuerkreis/` |
| Relay boards | `net_erl/leistungskreis/`, `net_zrl/leistungskreis/` |

## Microcontrollers and adapters

### ESP32-C3 SuperMini Plus V2.0

The main MCU module used throughout the project. It is documented separately in [`esp32-c3-supermini.md`](esp32-c3-supermini.md).

### ESP32-C3_SUPERMINI_TH

Through-hole adapter board for mounting the SuperMini module on the project PCBs. It is used wherever the module needs to be socketed or replaced easily.

## Power supplies

### HLK-5M05

Hi-Link AC/DC converter used for 230 V AC to 5 V DC conversion on relay boards.

### HLK-PM03

Hi-Link AC/DC converter used for 230 V AC to 3.3 V DC conversion on the mains-powered sensor board.

## Relays

### Hongfa HF46F/5-HS1

5 V relay used on the relay boards. The design supports one relay on `net_erl` and two relays on `net_zrl`.

## Optocouplers

### PC817

Optocoupler used for galvanic isolation between the control side and the relay driver side.

## MOSFETs

### FQP27P06

P-channel MOSFET in TO-220 package, used on the single-relay board.

### IRLZ34N

N-channel MOSFET in TO-220 package, used on the dual-relay board.

### IRF9530

P-channel MOSFET in SOT-23 package, used on the battery-powered sensor designs.

## Diodes

### 1N4007

Flyback diode used on relay boards to protect against inductive spikes.

### BZX55C3V3

3.3 V Zener diode used in the battery-powered supply path.

## Sensors

### BME280

Temperature, humidity, and pressure sensor used on selected devices.

### BME680

Temperature, humidity, pressure, and gas sensor used on the LED-ring relay module.

### VEML7700

Lux sensor used on the relay and mains-sensor devices.

### ENS160

MOX gas sensor used on the LED-ring relay module.

### AHT21

Temperature and humidity sensor used on some legacy or alternate sensor layouts.

### LD2410

Radar presence sensor used on the LED-ring relay module.

### HC-SR501

PIR motion sensor used in some experimental or supporting setups.

## Passive parts

Common passive parts are used project-wide where necessary:

- resistors for pull-ups, dividers, and LEDs
- capacitors for decoupling and smoothing
- connectors and headers for board interconnects
- fuses and protection parts where mains voltage is involved
