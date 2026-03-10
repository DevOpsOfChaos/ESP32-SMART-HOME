#pragma once

// ============================================================
// NET-SEN – Gerätekonfiguration vor dem Upload
// ============================================================
// Netzbetriebener Sensor-Node ohne Relais.
// Typische Einsätze: Raumklima, Luftqualität, Präsenz.
// ============================================================

constexpr char DEVICE_ID[]   = "NET-SEN-001";
constexpr char DEVICE_NAME[] = "Sensor Platzhalter";
constexpr char FW_VARIANT[]  = "net_sen_base";
constexpr bool DEVICE_DEBUG_AKTIV = true;

// Meldeintervall für periodischen STATE (ms).
// Begründung: 60 s reichen für langsam ändernde Klimawerte.
constexpr unsigned long STATE_INTERVAL_MS = 60000UL;

// Sofort-Meldeschwellen (Differenz zum letzten gemeldeten Wert):
// Begründung: Deutliche Änderungen (1 °C, 5 % rF) sofort senden,
// damit der Server nicht auf das nächste Intervall warten muss.
constexpr int16_t  TEMP_DELTA_01C  = 10;  // 1.0 °C in 0.1-°C-Schritten
constexpr uint16_t HUM_DELTA_01PCT = 50;  // 5.0 % in 0.1-%-Schritten
constexpr uint16_t LUX_DELTA       = 100; // 100 Lux

constexpr unsigned long ACK_TIMEOUT_MS = 250UL;
constexpr uint8_t MAX_RETRIES = 3;
