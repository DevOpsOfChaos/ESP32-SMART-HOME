#pragma once
// ============================================================
// BAT-SEN – Pinbelegung
// ============================================================
// Batteriemessung über ADC. Tatsächlichen Pin je Platine prüfen.
// Reed-Kontakt und Taster mit INPUT_PULLUP, LOW-aktiv.
// ============================================================
constexpr int PIN_STATUS_LED    = -1;
constexpr int PIN_RELAY_1       = -1; // nicht bestückt
constexpr int PIN_RELAY_2       = -1; // nicht bestückt
constexpr int PIN_SENSOR_SDA    = -1; // I2C für optionale Sensoren
constexpr int PIN_SENSOR_SCL    = -1;
constexpr int PIN_BUTTON_1      = -1; // Taster/Wandschalter Kanal 1
constexpr int PIN_BUTTON_2      = -1; // Kanal 2 (optional)
constexpr int PIN_BUTTON_3      = -1; // Kanal 3 (optional)
constexpr int PIN_BUTTON_4      = -1; // Kanal 4 (optional)
constexpr int PIN_REED          = -1; // Fensterkontakt (Reed-Schalter)
constexpr int PIN_RAIN_ADC      = -1; // Regensensor ADC-Eingang
constexpr int PIN_BATTERY_ADC   = -1; // Batteriespannungsmessung
