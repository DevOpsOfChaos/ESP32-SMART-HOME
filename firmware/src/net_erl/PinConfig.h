#pragma once

#include "../../include/HardwarePinStandard.h"

// ============================================================
// NET-ERL – Pinbelegung
// ============================================================
// NET-ERL folgt dem verbindlichen Hardwarestandard der Platine.
// Nur optional nicht bestueckte Funktionen bleiben auf -1.
// ============================================================

// Separate Status-LED aktuell nicht bestueckt.
constexpr int PIN_STATUS_LED   = -1;

// Lokaler Taster (INPUT_PULLUP, LOW bei Betätigung).
constexpr int PIN_BUTTON_1     = -1;

// Relais 1 der Relais-Platine.
constexpr int PIN_RELAY_1      = SmartHome::HardwarePinStandard::PIN_RELAY_1;

// Zweites Relais – bei NET-ERL nicht bestückt.
constexpr int PIN_RELAY_2      = -1;

// I2C Bus für optionale Sensoren (z. B. AHT/ENS/VEML).
constexpr int PIN_SENSOR_SDA   = SmartHome::HardwarePinStandard::PIN_I2C_SDA;
constexpr int PIN_SENSOR_SCL   = SmartHome::HardwarePinStandard::PIN_I2C_SCL;

// PIR-Sensor Eingang (optional, HIGH bei Bewegung).
constexpr int PIN_PIR          = -1;

// Interner Board-NeoPixel.
constexpr int PIN_INTERNAL_NEOPIXEL = SmartHome::HardwarePinStandard::GPIO_INTERNAL_NEOPIXEL;

// WS2812-Ring Datenleitung (optional, für Küchenlampe).
constexpr int PIN_LED_RING     = -1;

// Anzahl WS2812-Pixel im Ring (0 = kein Ring bestückt).
constexpr int LED_RING_COUNT   = 0;
