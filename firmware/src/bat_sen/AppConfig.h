#pragma once

#include <stdint.h>

#include "../../lib/ShSensors/src/ProviderIds.h"

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

constexpr char DEVICE_ID[]   = "bat_sen_01";
constexpr char DEVICE_NAME[] = "BAT-SEN Base";
constexpr char FW_VARIANT[]  = "bat_sen_base";
constexpr bool DEVICE_DEBUG_AKTIV = true;

constexpr unsigned long HELLO_RETRY_INTERVAL_MS = 5000UL;
constexpr int WLAN_KANAL = 6;

// 5 s halten oeffnet den lokalen Setup-Modus erneut,
// ohne die persistierten Basiswerte sofort zu loeschen.
constexpr unsigned long SETUP_REENTRY_HOLD_MS = 5000UL;

// 10 s halten fuehrt den gemeinsamen Factory Reset aus
// und startet danach wieder im Setup-Modus.
constexpr unsigned long FACTORY_RESET_HOLD_MS = 10000UL;

// Schlafintervall (Sekunden).
// Begründung: Im normalen Betrieb reicht ein 5-Minuten-Herzschlag.
// Bei Ereignissen wacht das Gerät sofort auf.
constexpr unsigned long SLEEP_INTERVAL_S = 300UL;
constexpr unsigned long HEARTBEAT_INTERVAL_MS = SLEEP_INTERVAL_S * 1000UL;
constexpr unsigned long STATE_INTERVAL_MS = SLEEP_INTERVAL_S * 1000UL;

// Empfangsfenster nach TX (Millisekunden).
// Begründung: Master braucht max. 150 ms für HELLO_ACK oder CFG.
// 300 ms gibt genug Puffer ohne Akku zu belasten.
constexpr unsigned long RX_WINDOW_MS = 300UL;

// Wachhaltezeit nach Ereignis vor Schlaf (Millisekunden).
// Begründung: 1 s reicht für ACK-Empfang und mögliche CFG-Antwort.
constexpr unsigned long WACH_NACH_EVENT_MS = 1000UL;

// Batterie: Warnung unter diesem Prozentsatz.
constexpr uint8_t LOW_BATTERY_PCT = 20;

enum BatteryProfileV1 : uint8_t {
    BAT_PROFILE_COIN_3V_PRIMARY = 1U,
    BAT_PROFILE_ALKALINE_2X = 2U,
    BAT_PROFILE_LIION_1S = 3U,
};

struct BatteryProfileVoltageRange {
    uint16_t leer_mv;
    uint16_t voll_mv;
};

// V1 supports exactly these three profiles. NiMH, LiFePO4 and other mixes
// stay out of scope until there is a real hardware need for them.
// The base build defaults to the 3 V coin profile until the concrete board
// profile is pinned down in repo config. Change this constant deliberately
// for alkaline_2x or liion_1s hardware instead of trusting the percentage.
constexpr BatteryProfileV1 BATTERY_PROFILE = BAT_PROFILE_COIN_3V_PRIMARY;

constexpr uint32_t BATTERY_DIVIDER_TOP_OHM = 100000UL;
constexpr uint32_t BATTERY_DIVIDER_BOTTOM_OHM = 100000UL;
constexpr uint8_t BATTERY_ADC_SAMPLE_COUNT = 4U;
constexpr uint16_t RAIN_DELTA_RAW = 32U;
constexpr unsigned long INPUT_EVENT_DEBOUNCE_MS = 35UL;
constexpr unsigned long WINDOW_DEBOUNCE_MS = 40UL;
constexpr unsigned long MULTI_BUTTON_LONGPRESS_MS = 800UL;

constexpr BatteryProfileVoltageRange BATTERY_PROFILE_COIN_3V_PRIMARY_RANGE = {2000U, 3000U};
constexpr BatteryProfileVoltageRange BATTERY_PROFILE_ALKALINE_2X_RANGE = {2000U, 3200U};
constexpr BatteryProfileVoltageRange BATTERY_PROFILE_LIION_1S_RANGE = {3200U, 4200U};

static_assert(
    BATTERY_PROFILE == BAT_PROFILE_COIN_3V_PRIMARY ||
    BATTERY_PROFILE == BAT_PROFILE_ALKALINE_2X ||
    BATTERY_PROFILE == BAT_PROFILE_LIION_1S,
    "BAT-SEN V1 supports only the explicit coin/alcaline/liion battery profiles.");

#ifndef BAT_SEN_REED_PROVIDER
#define BAT_SEN_REED_PROVIDER SH_BAT_SEN_REED_PROVIDER_NONE
#endif

#ifndef BAT_SEN_REED_ACTIVE_LOW
#define BAT_SEN_REED_ACTIVE_LOW 1
#endif

#ifndef BAT_SEN_REED_USE_PULLUP
#define BAT_SEN_REED_USE_PULLUP 1
#endif

#ifndef BAT_SEN_BUTTON_PROVIDER
#define BAT_SEN_BUTTON_PROVIDER SH_BAT_SEN_BUTTON_PROVIDER_NONE
#endif

#ifndef BAT_SEN_BUTTON_ACTIVE_LOW
#define BAT_SEN_BUTTON_ACTIVE_LOW 1
#endif

#ifndef BAT_SEN_BUTTON_USE_PULLUP
#define BAT_SEN_BUTTON_USE_PULLUP 1
#endif

#ifndef BAT_SEN_RAIN_PROVIDER
#define BAT_SEN_RAIN_PROVIDER SH_BAT_SEN_RAIN_PROVIDER_NONE
#endif

#ifndef BAT_SEN_BUTTON_CHANNELS
#define BAT_SEN_BUTTON_CHANNELS 1U
#endif

constexpr uint8_t BAT_SEN_REED_PROVIDER_KIND = BAT_SEN_REED_PROVIDER;
constexpr bool BAT_SEN_REED_SIGNAL_ACTIVE_LOW = BAT_SEN_REED_ACTIVE_LOW != 0;
constexpr bool BAT_SEN_REED_SIGNAL_USE_PULLUP = BAT_SEN_REED_USE_PULLUP != 0;
constexpr uint8_t BAT_SEN_BUTTON_PROVIDER_KIND = BAT_SEN_BUTTON_PROVIDER;
constexpr bool BAT_SEN_BUTTON_SIGNAL_ACTIVE_LOW = BAT_SEN_BUTTON_ACTIVE_LOW != 0;
constexpr bool BAT_SEN_BUTTON_SIGNAL_USE_PULLUP = BAT_SEN_BUTTON_USE_PULLUP != 0;
constexpr uint8_t BAT_SEN_RAIN_PROVIDER_KIND = BAT_SEN_RAIN_PROVIDER;
constexpr uint8_t BAT_SEN_BUTTON_CHANNEL_COUNT = BAT_SEN_BUTTON_CHANNELS;

constexpr unsigned long LOOP_INTERVAL_MS = 25UL;
