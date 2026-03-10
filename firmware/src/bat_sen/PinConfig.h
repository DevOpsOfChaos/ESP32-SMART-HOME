#pragma once

#include "../../include/HardwarePinStandard.h"
// ============================================================
// BAT-SEN – Pinbelegung
// ============================================================
// Batteriemessung ueber festen Spannungsteiler-Eingang.
// Reed-Kontakt und Taster mit INPUT_PULLUP, LOW-aktiv.
// ============================================================
constexpr int PIN_STATUS_LED    = -1;
constexpr int PIN_RELAY_1       = -1; // nicht bestückt
constexpr int PIN_RELAY_2       = -1; // nicht bestückt
constexpr int PIN_SENSOR_SDA    = SmartHome::HardwarePinStandard::PIN_I2C_SDA; // I2C für optionale Sensoren
constexpr int PIN_SENSOR_SCL    = SmartHome::HardwarePinStandard::PIN_I2C_SCL;
constexpr int PIN_BUTTON_1      = -1; // Taster/Wandschalter Kanal 1
constexpr int PIN_BUTTON_2      = -1; // Kanal 2 (optional)
constexpr int PIN_BUTTON_3      = -1; // Kanal 3 (optional)
constexpr int PIN_BUTTON_4      = -1; // Kanal 4 (optional)
constexpr int PIN_REED          = -1; // Fensterkontakt (Reed-Schalter)
constexpr int PIN_RAIN_ADC      = -1; // Regensensor ADC-Eingang
constexpr int PIN_BATTERY_ADC   = SmartHome::HardwarePinStandard::PIN_BATTERY_ADC; // Batteriespannungsmessung
constexpr int PIN_INTERNAL_NEOPIXEL = SmartHome::HardwarePinStandard::GPIO_INTERNAL_NEOPIXEL;
