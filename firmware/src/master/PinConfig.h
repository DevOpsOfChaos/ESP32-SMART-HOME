#pragma once

// ============================================================
// Master – Pinbelegung
// ============================================================
// Finale Pinbelegung nach Hardwareentscheidung.
// -1 bedeutet: dieser Pin ist für diesen Hardwarestand nicht
// belegt oder noch nicht festgelegt.
//
// Hinweis: Der Master hat keine Relais und keine Sensoren.
// Er benötigt nur Status-LED und Reset-Taster.
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
constexpr int PIN_SENSOR_SDA = -1;
constexpr int PIN_SENSOR_SCL = -1;
