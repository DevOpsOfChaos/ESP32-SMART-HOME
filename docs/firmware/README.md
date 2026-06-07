# Firmware Documentation

This folder contains the technical reference for the ESP32-C3 firmware line, the ESP-NOW binary protocol, the MQTT contract, and the device type model.

## Structure

| File | Description |
|---|---|
| `01_espnow_protokoll_referenz.md` | ESP-NOW binary protocol: header, payloads, CRC, event system |
| `02_devicetypes_capabilities.md` | device classes, power types, control modes, profiles, capability bitmask |
| `03_mqtt_contract_referenz.md` | MQTT topics, JSON schemas, ACK mechanism, cover contract |
| `devices/NET-ERL-001_hall_module.md` | hall module (PIR + BME280 + VEML7700 + relay) |
| `devices/NET-ERL-002_hall_module_led_ring.md` | extended hall module (radar + BME680 + ENS160 + NeoPixel + relay) |
| `devices/NET-ZRL-002_shutter_module.md` | shutter module (2 relays, 3 buttons, calibration, position estimate) |
| `devices/NET-SEN-002_weather_station.md` | weather sensor (BME280 + VEML7700 + digital rain input) |
| `devices/bat_sen_01_window_contact.md` | battery window contact (reed, CR2032, GPIO wake) |
| `devices/bat_sen_02_rain_sensor.md` | battery rain sensor (ADC, hysteresis, timer wake) |

## Device comparison

| Feature | NET-ERL-001 | NET-ERL-002 | NET-ZRL-002 | NET-SEN-002 | bat_sen_01 | bat_sen_02 |
|---|---|---|---|---|---|---|
| Class | NET_ERL (0x01) | NET_ERL (0x01) | NET_ZRL (0x02) | NET_SEN (0x03) | BAT_SEN (0x04) | BAT_SEN (0x04) |
| Power | mains | mains | mains | mains | CR2032 | 2x AA |
| Control | relay_light | relay_light | cover | none | none | none |
| Profile | hall_light | hall_module_led_ring | cover_basic | none | none | none |
| Reporting | hybrid | hybrid | hybrid | hybrid | sleep_event | sleep_event |
| Sensors | BME280, VEML7700, PIR | BME680, VEML7700, ENS160, LD2410 | - | BME280, VEML7700, rain | reed (GPIO) | ADC rain |
| Actuators | 1 relay | 1 relay + NeoPixel | 2 relays (cover) | - | - | - |
| Buttons | - | 1 (GPIO6, active-low) | 3 (GPIO20/4/3) | - | - | - |
| Status LEDs | - | - | 2 (up=GPIO7, down=GPIO6, active-high) | - | - | - |
| ESP-NOW payload | 31B (config) | 45B (gas + config) | 25B (config) | 24-36B (ext) | 24B | 24B |
| Wake | n/a | n/a | n/a | n/a | GPIO + timer | timer (900s) |
| Setup portal | yes | yes | yes | yes | yes | yes |

## Design principles

- base type architecture (`NetErlRuntime`, `NetSenRuntime`, `BatSenRuntime`)
- device-specific hooks (`on_config`, `on_sensor`, `on_control`)
- ESP-NOW retry mechanism with 2 retries at 50 ms
- sensor recovery with damped error logging and automatic re-init
- late-lux principle for auto-light behavior after a PIR trigger
- calibration with rollback on write failure

## Quick links

- ESP-NOW protocol: [01_espnow_protokoll_referenz.md](01_espnow_protokoll_referenz.md)
- device types: [02_devicetypes_capabilities.md](02_devicetypes_capabilities.md)
- MQTT contract: [03_mqtt_contract_referenz.md](03_mqtt_contract_referenz.md)
- all devices: [devices/](devices/)
