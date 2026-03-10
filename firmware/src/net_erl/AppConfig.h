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

// Feste Pilot-ID für die erste lauffähige Vertikalstrecke.
constexpr char DEVICE_ID[]   = "net_erl_01";

// Lesbarer Name für Logs und HELLO.
constexpr char DEVICE_NAME[] = "NET-ERL Pilot";

// Bewusst keine Gerätefamilie aufblasen: genau ein Pilot-Stand.
constexpr char FW_VARIANT[]  = "net_erl_pilot";

// Debug aktiv lassen in Testständen.
constexpr bool DEVICE_DEBUG_AKTIV = true;

// HELLO wird bis zum ersten HELLO_ACK erneut versucht.
constexpr unsigned long HELLO_RETRY_INTERVAL_MS = 5000UL;

// HEARTBEAT hält die Online-Erkennung des Masters am Leben.
constexpr unsigned long HEARTBEAT_INTERVAL_MS = 5000UL;

// Muss mit dem Master-/AP-Kanal übereinstimmen.
constexpr int WLAN_KANAL = 6;

// Relais-Standardzustand beim Boot (0 = aus, 1 = an).
// Begründung: Im Normalbetrieb ist "aus" der sichere Startzustand.
constexpr bool RELAY_DEFAULT_ON_BOOT = false;

// Verzögerung der Hauptschleife.
constexpr unsigned long LOOP_INTERVAL_MS = 10UL;
