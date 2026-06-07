# ESP32-C3 SuperMini Plus V2.0 - Platform Reference

> All devices in this project use the TENSTAR ESP32-C3 SuperMini Plus V2.0.

## Technical data

| Property | Value |
|---|---|
| Microcontroller | Espressif ESP32-C3 (RISC-V, 32-bit) |
| CPU clock | up to 160 MHz |
| Flash | 4 MB |
| SRAM | 400 KB |
| Wi-Fi | 802.11 b/g/n (2.4 GHz) |
| Bluetooth | BLE 5.0 |
| USB | Native USB-CDC + JTAG |
| ADC | 12-bit, up to 6 usable channels |
| PWM | Available on almost all GPIOs |
| Supply voltage | 5 V (USB / VIN) |
| Logic voltage | 3.3 V |
| Onboard LDO | ME6211 / XC6206 / AMS1117 clone |
| Max. output current (3.3 V) | approx. 500 mA |

## Current draw

| Mode | Current |
|---|---|
| Wi-Fi TX peak | approx. 300 mA |
| Idle (Wi-Fi connected) | 70-80 mA |
| Light sleep | approx. 5 mA |
| Deep sleep (chip only) | approx. 40 uA |
| Deep sleep (whole board) | approx. 0.5-0.8 mA |

## Onboard peripherals

| Component | Pin / detail |
|---|---|
| Power LED | Red (hard-wired) |
| User LED | Blue (GPIO8) |
| RGB LED | WS2812 (GPIO8) |
| Reset button | EN |
| Boot button | GPIO9 |
| Antenna | PCB antenna + U.FL connector |

> **Note:** The blue LED and the RGB LED share GPIO8. They cannot be used at the same time.

## Through-hole adapter (`ESP32-C3_SUPERMINI_TH`)

For use on the project boards, the SuperMini board is mounted on a **through-hole adapter board**:

- KiCad symbol: `ESP32-C3_SUPERMINI_TH.kicad_sym`
- Footprint: `MODULE_ESP32-C3_SUPERMINI_TH.kicad_mod`
- 3D model: `ESP32-C3_SUPERMINI_TH.step`
- Makes the module easy to insert and replace on 2x10 pin headers

## Available GPIOs on the adapter

| Pin | Function | Analog | Special note |
|---|---|---|---|
| GPIO0 | I2C SDA / free | yes | ADC1_CH0 |
| GPIO1 | I2C SCL / free | yes | ADC1_CH1 |
| GPIO2 | free | yes | ADC1_CH2 |
| GPIO3 | free | yes | ADC1_CH3 |
| GPIO4 | free | yes | ADC1_CH4 |
| GPIO5 | free | yes | ADC2_CH0 |
| GPIO6 | free | no | - |
| GPIO7 | free | no | - |
| GPIO8 | free (LED) | no | Onboard WS2812 |
| GPIO9 | Boot button | no | Not usable as regular I/O |
| GPIO10 | free | no | - |
| GPIO20 | RX / free | no | UART |
| GPIO21 | TX / free | no | UART |

> **Strapping pins:** GPIO2, GPIO8, and GPIO9 affect boot behavior. GPIO9 is not usable during normal operation.

## Interfaces

- **UART:** 2x (RX/TX on GPIO20/21)
- **I2C:** 1x (SDA/SCL on GPIO0/1)
- **SPI:** Available on GPIO4-7
- **ADC:** 12-bit, up to 6 channels (GPIO0-5)
- **PWM:** On almost all GPIOs
- **USB-CDC:** Native USB serial for programming and debug
- **USB-JTAG:** On-chip debugger
