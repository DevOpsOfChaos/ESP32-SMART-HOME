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

constexpr char DEVICE_ID[]   = "net_zrl_01";
constexpr char DEVICE_NAME[] = "NET-ZRL Base";
constexpr char FW_VARIANT[]  = "net_zrl_base";
constexpr bool DEVICE_DEBUG_AKTIV = true;

constexpr unsigned long HELLO_RETRY_INTERVAL_MS = 5000UL;
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 5000UL;
constexpr unsigned long STATE_INTERVAL_MS = 30000UL;
constexpr int WLAN_KANAL = 6;

// 5 s halten oeffnet den lokalen Setup-Modus erneut,
// ohne die persistierten Basiswerte sofort zu loeschen.
constexpr unsigned long SETUP_REENTRY_HOLD_MS = 5000UL;

// 10 s halten fuehrt den gemeinsamen Factory Reset aus
// und startet danach wieder im Setup-Modus.
constexpr unsigned long FACTORY_RESET_HOLD_MS = 10000UL;

constexpr bool RELAY_1_DEFAULT_ON_BOOT = false;
constexpr bool RELAY_2_DEFAULT_ON_BOOT = false;

// Entprellung fuer lokale Taster.
// 35 ms reicht fuer mechanische Kontakte,
// ohne den lokalen Bedienpfad weich oder unpraezise wirken zu lassen.
constexpr unsigned long INPUT_EVENT_DEBOUNCE_MS = 35UL;

// Long-Press-Schwelle fuer lokale Button-Events.
// Wird aktuell nur fuer Eventmeldungen genutzt, nicht fuer Sonderaktionen.
constexpr unsigned long LOCAL_BUTTON_LONGPRESS_MS = 800UL;

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
constexpr unsigned long LOOP_INTERVAL_MS = 10UL;
