#pragma once

// ============================================================
// NET-ERL – Gerätekonfiguration vor dem Upload
// ============================================================
// Netzbetriebener Node mit einem Relais.
//
// Typische Einsätze:
//   - einfache Lampensteuerung
//   - Schaltaktor mit lokalem Taster
//   - Flurlicht (mit PIR-Erweiterung)
//   - Küchenlampe (mit Sensor-Erweiterung und WS2812-Ring)
//
// Sondergeräte dieser Klasse erhalten eigene fw_variant,
// bleiben aber als NET-ERL-xxx Device-IDs im System.
//
// Private Zugangsdaten stehen in Secrets.h (nicht im Repo).
// Vorlage: firmware/include/Secrets.example.h
// ============================================================

// Geräte-ID.
// Format: NET-ERL-001
// Nur Laufnummer anpassen, keine Sensorbestückung reinschreiben.
constexpr char DEVICE_ID[]   = "NET-ERL-001";

// Anzeigename für MQTT-meta und Node-RED.
// Kann nach Inbetriebnahme über CFG geändert werden.
constexpr char DEVICE_NAME[] = "Lampe Platzhalter";

// Firmware-Variante.
// Mögliche Werte: "net_erl_base", "net_erl_flur", "net_erl_kueche"
constexpr char FW_VARIANT[]  = "net_erl_base";

// Debug aktiv lassen in Testständen.
constexpr bool DEVICE_DEBUG_AKTIV = true;

// Meldeintervall für periodischen STATE (Millisekunden).
// Begründung: 30 s sind ein guter Kompromiss zwischen
// Aktualität und Funklast. Änderungen werden immer sofort
// als EVENT gemeldet.
constexpr unsigned long STATE_INTERVAL_MS = 30000UL;

// Entprellzeit für den lokalen Taster (Millisekunden).
// Begründung: 50 ms verhindern Prelleffekte bei üblichen
// Mikrotastern, ohne echte Doppelklicks zu verschlucken.
constexpr unsigned long BUTTON_DEBOUNCE_MS = 50UL;

// Relais-Standardzustand beim Boot (0 = aus, 1 = an).
// Begründung: Im Normalbetrieb ist "aus" der sichere Startzustand.
constexpr bool RELAY_DEFAULT_ON_BOOT = false;

// ACK-Timeout für gesendete Nachrichten (Millisekunden).
// Begründung: 250 ms ist kurz genug für schnelle Wiederholung,
// aber lang genug für Empfänger-Verarbeitung + ACK-Rücksendung.
constexpr unsigned long ACK_TIMEOUT_MS = 250UL;

// Maximale Anzahl ACK-Retries bevor Fehler gemeldet wird.
// Begründung: 3 Versuche decken kurze Funkunterbrechungen ab,
// ohne ewig zu blockieren.
constexpr uint8_t MAX_RETRIES = 3;
