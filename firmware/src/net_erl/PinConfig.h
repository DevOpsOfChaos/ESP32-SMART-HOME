#pragma once

// ============================================================
// NET-ERL – Pinbelegung
// ============================================================
// Finale Pinzuordnung nach Hardwareentscheidung eintragen.
// -1 = nicht belegt oder für diese Variante irrelevant.
//
// Hinweis: Pinbelegungen aus dem Altprojekt dürfen als
// Referenz genutzt werden, müssen aber pro Hardwarestand
// geprüft und explizit eingetragen werden.
// ============================================================

// Status-LED (optional, LOW-aktiv oder HIGH-aktiv je Hardware).
constexpr int PIN_STATUS_LED   = -1;

// Lokaler Taster (INPUT_PULLUP, LOW bei Betätigung).
constexpr int PIN_BUTTON_1     = -1;

// Relais 0 (OUTPUT, HIGH = Relais an).
// Hinweis: Sicherheitsrelevante Aktoren mit invertierter Logik
// müssen im Applikationscode explizit berücksichtigt werden.
constexpr int PIN_RELAY_1      = -1;

// Zweites Relais – bei NET-ERL nicht bestückt.
constexpr int PIN_RELAY_2      = -1;

// I2C Bus für optionale Sensoren (z. B. AHT/ENS/VEML).
constexpr int PIN_SENSOR_SDA   = -1;
constexpr int PIN_SENSOR_SCL   = -1;

// PIR-Sensor Eingang (optional, HIGH bei Bewegung).
constexpr int PIN_PIR          = -1;

// WS2812-Ring Datenleitung (optional, für Küchenlampe).
constexpr int PIN_LED_RING     = -1;

// Anzahl WS2812-Pixel im Ring (0 = kein Ring bestückt).
constexpr int LED_RING_COUNT   = 0;
