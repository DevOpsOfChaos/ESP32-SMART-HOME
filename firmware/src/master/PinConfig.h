#pragma once

#include "../../include/HardwarePinStandard.h"

// ============================================================
// Master – Pinbelegung
// ============================================================
// Der Master nutzt keine Relais und aktuell keine Sensorik.
// Feste Board-Standards bleiben trotzdem zentral hinterlegt.
// ============================================================

// Status-LED (optional).
// Zeigt Verbindungszustand an: AUS = Trennung, AN = verbunden.
// -1 setzen, wenn keine LED verbaut ist.
constexpr int PIN_STATUS_LED = -1;

// Optionaler Reset-Taster (LOW = Werksreset-Auslösung nach Haltezeit).
constexpr int PIN_BUTTON_1   = -1;

// Nicht genutzt auf dem Master:
constexpr int PIN_RELAY_1    = -1;
constexpr int PIN_RELAY_2    = -1;
constexpr int PIN_SENSOR_SDA = SmartHome::HardwarePinStandard::PIN_I2C_SDA;
constexpr int PIN_SENSOR_SCL = SmartHome::HardwarePinStandard::PIN_I2C_SCL;
constexpr int PIN_INTERNAL_NEOPIXEL = SmartHome::HardwarePinStandard::GPIO_INTERNAL_NEOPIXEL;
