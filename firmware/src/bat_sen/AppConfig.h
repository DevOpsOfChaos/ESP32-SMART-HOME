#pragma once

// ============================================================
// BAT-SEN – Gerätekonfiguration vor dem Upload
// ============================================================
// Batteriebetriebener Sensor- und Event-Node.
// Das Gerät schläft die meiste Zeit und wacht nur auf
// bei Ereignissen oder nach dem Schlafintervall.
//
// Typische Einsätze:
//   - Fensterkontakt (Reed-Schalter)
//   - Wandschalter (1..4 Tasten)
//   - Regensensor
//   - Batteriebetriebener Außensensor
//
// Wichtig:
//   BAT-SEN sendet KEIN MQTT. Nur ESP-NOW an Master.
//   Der Master entscheidet, ob das Gerät als "offline" gilt.
//   Aggressives Offline-Marking vermeiden (OFFLINE_TIMEOUT_BAT_MS).
//
// Private Zugangsdaten stehen in Secrets.h (nicht im Repo).
// ============================================================

constexpr char DEVICE_ID[]   = "BAT-SEN-001";
constexpr char DEVICE_NAME[] = "Kontakt Platzhalter";
constexpr char FW_VARIANT[]  = "bat_sen_base";
constexpr bool DEVICE_DEBUG_AKTIV = true;

// Schlafintervall (Sekunden).
// Begründung: Im normalen Betrieb reicht ein 5-Minuten-Herzschlag.
// Bei Ereignissen wacht das Gerät sofort auf.
constexpr unsigned long SLEEP_INTERVAL_S = 300UL;

// Empfangsfenster nach TX (Millisekunden).
// Begründung: Master braucht max. 150 ms für HELLO_ACK oder CFG.
// 300 ms gibt genug Puffer ohne Akku zu belasten.
constexpr unsigned long RX_WINDOW_MS = 300UL;

// Wachhaltezeit nach Ereignis vor Schlaf (Millisekunden).
// Begründung: 1 s reicht für ACK-Empfang und mögliche CFG-Antwort.
constexpr unsigned long WACH_NACH_EVENT_MS = 1000UL;

// Batterie: Warnung unter diesem Prozentsatz.
constexpr uint8_t LOW_BATTERY_PCT = 20;

constexpr unsigned long ACK_TIMEOUT_MS = 250UL;
constexpr uint8_t MAX_RETRIES = 3;
