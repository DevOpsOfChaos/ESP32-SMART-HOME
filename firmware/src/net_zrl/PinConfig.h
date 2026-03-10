#pragma once

#include "../../include/HardwarePinStandard.h"

// ============================================================
// NET-ZRL – Pinbelegung
// ============================================================

constexpr int PIN_STATUS_LED   = -1;
constexpr int PIN_BUTTON_1     = -1; // Taster für Relais 0 / Rolladen Auf
constexpr int PIN_BUTTON_2     = -1; // Taster für Relais 1 / Rolladen Ab
constexpr int PIN_BUTTON_3     = -1; // Stop-Taster (Rolladen)
constexpr int PIN_RELAY_1      = SmartHome::HardwarePinStandard::PIN_RELAY_1; // Relais 1 / Kreis 1
constexpr int PIN_RELAY_2      = SmartHome::HardwarePinStandard::PIN_RELAY_2; // Relais 2 / Kreis 2
constexpr int PIN_SENSOR_SDA   = SmartHome::HardwarePinStandard::PIN_I2C_SDA; // I2C für optionalen Sensor
constexpr int PIN_SENSOR_SCL   = SmartHome::HardwarePinStandard::PIN_I2C_SCL;
constexpr int PIN_PIR          = -1; // optionaler PIR/Radar
constexpr int PIN_INTERNAL_NEOPIXEL = SmartHome::HardwarePinStandard::GPIO_INTERNAL_NEOPIXEL;
