#pragma once

// ============================================================
// NET-ZRL – Gerätekonfiguration vor dem Upload
// ============================================================
// Netzbetriebener Zwei-Relais-Node.
// Basis für unterschiedliche Gerätetypen innerhalb derselben
// Basisklasse.
//
// Typische Einsätze:
//   - Rolladensteuerung (Auf/Ab mit Sonderlogik)
//   - Außenlicht mit zwei getrennten Kreisen
//   - Doppelschaltaktor
//
// Wichtige Architekturregel:
//   Die NET-ZRL-Basis darf NICHT pauschal wie ein Rolladen behandelt
//   werden. Rolladen-Schutzlogik ist Sonderlogik und wird nur aktiv,
//   wenn COVER_MODUS_AKTIV = true gesetzt ist.
//
// Private Zugangsdaten stehen in Secrets.h (nicht im Repo).
// ============================================================

constexpr char DEVICE_ID[]   = "NET-ZRL-001";
constexpr char DEVICE_NAME[] = "NET-ZRL Platzhalter";
constexpr char FW_VARIANT[]  = "net_zrl_base";
constexpr bool DEVICE_DEBUG_AKTIV = true;

// Meldeintervall für periodischen STATE (ms).
constexpr unsigned long STATE_INTERVAL_MS = 30000UL;

// Entprellzeit lokaler Taster (ms).
constexpr unsigned long BUTTON_DEBOUNCE_MS = 50UL;

// ACK-Timeout (ms).
constexpr unsigned long ACK_TIMEOUT_MS = 250UL;

// Maximale Retries.
constexpr uint8_t MAX_RETRIES = 3;

// Sperrzeit nach Richtungswechsel (ms).
// Nur im Rolladenmodus relevant.
// Verhindert sofortige Richtungsumkehr zum Motorschutz.
constexpr unsigned long COVER_REVERSE_LOCK_MS = 500UL;

// Rolladen-Sondermodus aktiv?
// Bei true:
//   - Relais dürfen nie gleichzeitig aktiv sein
//   - Reversier-Sperrzeit ist aktiv
//   - Cover-Befehle und Cover-Zustand sind führend
// Bei false:
//   - beide Relais sind unabhängig schaltbar
//   - geeignet für Licht, Doppelschaltaktor, zwei Stromkreise
constexpr bool COVER_MODUS_AKTIV = false;
