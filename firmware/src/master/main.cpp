/*
====================================================================
 Projekt   : SmartHome ESP32
 Gerät     : Master (ESP32-C3)
 Datei     : main.cpp
 Version   : 0.2.0
 Stand     : 2026-03-10

 Funktion:
 Der Master ist die ESP-NOW <-> MQTT Bridge des Systems.
 Er empfängt Pakete aller Nodes über ESP-NOW, übersetzt sie
 in MQTT-Nachrichten und leitet MQTT-Befehle/Konfigurationen
 als ESP-NOW-Pakete an die zugehörigen Nodes weiter.

 Der Master ist kein zweites Node-RED und keine
 Automationsplattform. Alle Regeln und Automationen
 gehören auf den Server.

 Hardware:
 - ESP32-C3 SuperMini oder DevKit
 - WLAN auf festem Kanal (lokal konfiguriert)

 Architekturebene: B – Bridge
 - spricht ESP-NOW mit Nodes
 - spricht MQTT mit dem Server (Raspberry Pi)

 Master-Aufgaben:
 1. ESP-NOW-Empfang aller Nodes
 2. Geräte-Registry im RAM (max. SH_MAX_DEVICES Einträge)
 3. ACK/Retry für relevante Ausgangsnachrichten
 4. Duplikaterkennung eingehender Pakete (Sequenznummer)
 5. Availability-Erkennung (Online/Offline je Gerät)
 6. Zeitverteilung (TIME-Pakete an alle Nodes)
 7. Konfigurations-Routing (CFG-Pakete: MQTT -> Node)
 8. Befehlsrouting (CMD-Pakete: MQTT -> Node)
 9. MQTT-Veröffentlichung von STATE und EVENT

 Hinweise:
 - Secrets.h ist nicht im Repository.
 - Vorlage: firmware/include/Secrets.example.h
 - Private Zugangsdaten niemals in diese Datei.
 - Debug nur in Testständen aktiv (DEVICE_DEBUG_AKTIV in AppConfig.h).
====================================================================
*/

// ============================================================
// 1. Bibliotheken
// ============================================================

#include <Arduino.h>
#include "AppConfig.h"
#include "PinConfig.h"
#include "../../include/ProjectVersion.h"
#include "../../include/BuildConfig.h"
#include "../../include/DebugConfig.h"
#include "../../lib/ShProtocol/src/Protocol.h"
#include "../../lib/ShProtocol/src/DeviceTypes.h"

// Secrets.h ist lokal und liegt nicht im Repository.
// Vorlage: firmware/include/Secrets.example.h
#if __has_include("../../include/Secrets.h")
  #include "../../include/Secrets.h"
#else
  #warning "Keine Secrets.h gefunden. Bitte aus Secrets.example.h erstellen."
  #define WIFI_SSID         "KEIN_SSID"
  #define WIFI_PASSWORD     "KEIN_PASSWORT"
  #define MQTT_HOST         "127.0.0.1"
  #define MQTT_PORT         1883
  #define MQTT_USER         "mqtt_user"
  #define MQTT_PASSWORD     "KEIN_MQTT_PASSWORT"
#endif

// ============================================================
// 2. Versions- und Geräteinformationen
// ============================================================

constexpr char DATEI_GERAET[]  = "Master";
constexpr char DATEI_VERSION[] = "0.2.0";

// ============================================================
// 3. Einstellungen vor dem Upload
// ============================================================
// Gerätespezifische Einstellungen stehen in AppConfig.h.
// Vor dem Flashen prüfen:
//   - DEVICE_ID  (AppConfig.h)
//   - DEVICE_NAME (AppConfig.h)
//   - WLAN_KANAL (AppConfig.h)
//   - Secrets.h vorhanden und befüllt?

// ============================================================
// 4. Debug-Konfiguration
// ============================================================

// Debug aktiv, wenn sowohl globale als auch gerätespezifische
// Freigabe gesetzt ist.
constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;

// ============================================================
// 5. Konstanten, Zeitwerte und Grenzwerte
// ============================================================

// Hauptloop-Takt in Millisekunden.
// Begründung: 10 ms erlaubt flüssige ACK-Timeout-Prüfung
// ohne spürbare CPU-Last.
constexpr unsigned long LOOP_INTERVAL_MS = 10UL;

// Zeitabstand zwischen TIME-Verteilungen an alle Nodes (ms).
// Begründung: Minütliche Synchronisation reicht für lokale
// Node-Zeitstempel. Zu häufig belastet die Funkstrecke.
constexpr unsigned long TIME_SYNC_INTERVAL_MS = 60000UL;

// Takt für den Availability-Check (ms).
// Begründung: Bei 30-s-STATE-Intervall der Nodes reagiert
// ein 10-s-Check spätestens nach 3 ausgebliebenen Meldungen.
constexpr unsigned long AVAILABILITY_CHECK_MS = 10000UL;

// Offline-Timeout für Netzgeräte (ms).
// Begründung: 3 × 30-s-Meldeintervall = 90 s.
constexpr unsigned long OFFLINE_TIMEOUT_NET_MS = 90000UL;

// Offline-Timeout für Batteriegeräte (ms).
// Begründung: Batteriegeräte melden seltener. 10 Minuten
// verhindern unnötige Offline-Flicker.
constexpr unsigned long OFFLINE_TIMEOUT_BAT_MS = 600000UL;

// ============================================================
// 6. Pinbelegung
// ============================================================
// Alle Pins in PinConfig.h.

// ============================================================
// 7. Datenstrukturen und Statusvariablen
// ============================================================

// Eintrag in der RAM-Geräte-Registry des Masters.
struct RegistryEntry {
    char     device_id[SH_DEVICE_ID_LEN];
    char     device_name[SH_DEVICE_NAME_LEN];
    uint8_t  device_class;
    uint16_t caps;
    uint8_t  power_type;
    uint8_t  peer_mac[6];
    bool     ist_online;
    unsigned long letzter_kontakt_ms;
    uint8_t  letzter_seq;   // für Duplikaterkennung
    uint16_t fw_version;
    uint32_t boot_counter;
};

struct MasterState {
    bool     wlan_verbunden;
    bool     mqtt_verbunden;
    bool     espnow_bereit;
    unsigned long letzter_time_sync_ms;
    unsigned long letzter_availability_check_ms;
    unsigned long letzteSchleifeMs;
    uint8_t  seq_zaehler;
};

RegistryEntry registry[SH_MAX_DEVICES];
uint8_t       registry_anzahl = 0;
MasterState   masterStatus = {};

// ============================================================
// 8. Funktionsprototypen
// ============================================================

void initialisiereHardware();
void initialisiereWlan();
void initialisiereMqtt();
void initialisiereEspNow();
void aktualisiereKommunikation();
void aktualisiereAvailability();
void aktualisiereZeitverteilung();
void verarbeiteEingehendesEspNowPaket(const uint8_t* mac, const uint8_t* daten, int laenge);
void verarbeiteHello(const uint8_t* mac, const SmartHome::HelloPayload& payload);
void sendeHelloAck(const uint8_t* peer_mac, uint8_t seq);
void veroeffentlicheState(const char* device_id, const uint8_t* payload, uint16_t laenge);
void veroeffentlicheEvent(const char* device_id, const uint8_t* payload, uint16_t laenge);
void veroeffentlicheAvailability(const char* device_id, bool online);
void veroeffentlicheMeta(const RegistryEntry& entry);
int  findeRegistryEintrag(const uint8_t* mac);
int  erstelleRegistryEintrag(const uint8_t* mac, const SmartHome::HelloPayload& payload);
void gibStartmeldungAus();
void debugInfo(const char* text);
void debugWarn(const char* text);
void debugError(const char* text);

// ============================================================
// 9. Hilfsfunktionen
// ============================================================

void debugInfo(const char* text) {
    if (DEBUG_LOKAL_AKTIV) { Serial.print("[INFO] "); Serial.println(text); }
}
void debugWarn(const char* text) {
    if (DEBUG_LOKAL_AKTIV) { Serial.print("[WARN] "); Serial.println(text); }
}
void debugError(const char* text) {
    if (DEBUG_LOKAL_AKTIV) { Serial.print("[ERR ] "); Serial.println(text); }
}

int findeRegistryEintrag(const uint8_t* mac) {
    for (uint8_t i = 0; i < registry_anzahl; i++) {
        if (memcmp(registry[i].peer_mac, mac, 6) == 0) return (int)i;
    }
    return -1;
}

// ============================================================
// 10. Hardware- und Initialisierungsfunktionen
// ============================================================

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) {
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, LOW);
    }
}

/**
 * WLAN-Verbindung aufbauen auf festem Kanal.
 * Platzhalter: WiFi.begin() mit festem Kanal folgt in Phase 2.
 */
void initialisiereWlan() {
    debugInfo("WLAN: Initialisierung (Platzhalter)");
    masterStatus.wlan_verbunden = false;
}

/**
 * MQTT-Verbindung aufbauen.
 * Abonnierte Topics: smarthome/+/cmd  und  smarthome/+/cfg
 * Platzhalter: folgt in Phase 3.
 */
void initialisiereMqtt() {
    debugInfo("MQTT: Initialisierung (Platzhalter)");
    masterStatus.mqtt_verbunden = false;
}

/**
 * ESP-NOW initialisieren und RX-Callback registrieren.
 * Platzhalter: esp_now_init() folgt in Phase 2.
 */
void initialisiereEspNow() {
    debugInfo("ESP-NOW: Initialisierung (Platzhalter)");
    masterStatus.espnow_bereit = false;
}

// ============================================================
// 11. Kommunikationsfunktionen
// ============================================================

/**
 * ESP-NOW-Empfangs-Callback.
 * Prüft Header, erkennt Duplikate, dispatcht nach msg_type.
 *
 * Parameter:
 *   mac    - Sender-MAC (6 Bytes)
 *   daten  - Rohdaten
 *   laenge - Paketlänge
 */
void verarbeiteEingehendesEspNowPaket(
    const uint8_t* mac,
    const uint8_t* daten,
    int laenge)
{
    if (laenge < (int)SH_HEADER_SIZE) {
        debugWarn("ESP-NOW: Paket zu kurz");
        return;
    }

    const SmartHome::MsgHeader* header =
        reinterpret_cast<const SmartHome::MsgHeader*>(daten);

    if (!SmartHome::isValidHeader(*header)) {
        debugWarn("ESP-NOW: Ungueltiger Header");
        return;
    }

    const uint8_t* payload = daten + SH_HEADER_SIZE;

    switch (header->msg_type) {
        case SH_MSG_HELLO: {
            if (header->payload_len == sizeof(SmartHome::HelloPayload)) {
                verarbeiteHello(mac,
                    *reinterpret_cast<const SmartHome::HelloPayload*>(payload));
            }
            break;
        }
        case SH_MSG_STATE: {
            int idx = findeRegistryEintrag(mac);
            if (idx >= 0) {
                registry[idx].letzter_kontakt_ms = millis();
                if (!registry[idx].ist_online) {
                    registry[idx].ist_online = true;
                    veroeffentlicheAvailability(registry[idx].device_id, true);
                }
                veroeffentlicheState(
                    registry[idx].device_id, payload, header->payload_len);
            }
            break;
        }
        case SH_MSG_EVENT: {
            int idx = findeRegistryEintrag(mac);
            if (idx >= 0) {
                registry[idx].letzter_kontakt_ms = millis();
                veroeffentlicheEvent(
                    registry[idx].device_id, payload, header->payload_len);
            }
            break;
        }
        case SH_MSG_ACK:
            debugInfo("ACK empfangen");
            break;
        default:
            debugWarn("ESP-NOW: Unbekannter msg_type");
            break;
    }
}

void verarbeiteHello(const uint8_t* mac, const SmartHome::HelloPayload& payload) {
    if (!SmartHome::isValidDeviceId(payload.device_id)) {
        debugWarn("HELLO: Ungueltiger device_id");
        return;
    }
    int idx = findeRegistryEintrag(mac);
    if (idx < 0) {
        idx = erstelleRegistryEintrag(mac, payload);
        if (idx < 0) { debugError("HELLO: Registry voll"); return; }
        debugInfo("HELLO: Neues Geraet registriert");
    } else {
        memcpy(registry[idx].device_name, payload.device_name, SH_DEVICE_NAME_LEN);
        registry[idx].fw_version   = payload.fw_version;
        registry[idx].boot_counter = payload.boot_counter;
    }
    registry[idx].letzter_kontakt_ms = millis();
    registry[idx].ist_online = true;
    veroeffentlicheMeta(registry[idx]);
    veroeffentlicheAvailability(registry[idx].device_id, true);
    sendeHelloAck(mac, 0);
}

int erstelleRegistryEintrag(
    const uint8_t* mac,
    const SmartHome::HelloPayload& payload)
{
    if (registry_anzahl >= SH_MAX_DEVICES) return -1;
    int idx = (int)registry_anzahl++;
    memset(&registry[idx], 0, sizeof(RegistryEntry));
    memcpy(registry[idx].peer_mac, mac, 6);
    SmartHome::safeCopyDeviceId(payload.device_id, registry[idx].device_id);
    memcpy(registry[idx].device_name, payload.device_name, SH_DEVICE_NAME_LEN);
    registry[idx].device_name[SH_DEVICE_NAME_LEN - 1] = '\0';
    registry[idx].device_class = payload.device_class;
    registry[idx].caps = ((uint16_t)payload.caps_hi << 8) | payload.caps_lo;
    registry[idx].power_type   = payload.power_type;
    registry[idx].fw_version   = payload.fw_version;
    registry[idx].boot_counter = payload.boot_counter;
    registry[idx].letzter_seq  = 0xFF;
    return idx;
}

void sendeHelloAck(const uint8_t* peer_mac, uint8_t seq) {
    (void)peer_mac; (void)seq;
    debugInfo("HELLO_ACK: gesendet (Platzhalter)");
}

void veroeffentlicheState(const char* d, const uint8_t* p, uint16_t l) {
    (void)d; (void)p; (void)l;
    debugInfo("MQTT STATE: (Platzhalter)");
}
void veroeffentlicheEvent(const char* d, const uint8_t* p, uint16_t l) {
    (void)d; (void)p; (void)l;
    debugInfo("MQTT EVENT: (Platzhalter)");
}
void veroeffentlicheAvailability(const char* d, bool online) {
    (void)d;
    debugInfo(online ? "MQTT: online" : "MQTT: offline");
}
void veroeffentlicheMeta(const RegistryEntry& e) {
    (void)e;
    debugInfo("MQTT META: (Platzhalter)");
}

// ============================================================
// 12. Logikfunktionen
// ============================================================

/**
 * Prüft Registry auf Timeout und markiert inaktive Geräte offline.
 */
void aktualisiereAvailability() {
    unsigned long jetzt = millis();
    for (uint8_t i = 0; i < registry_anzahl; i++) {
        if (!registry[i].ist_online) continue;
        unsigned long timeout_ms = (registry[i].power_type == SH_POWER_BATTERY)
            ? OFFLINE_TIMEOUT_BAT_MS : OFFLINE_TIMEOUT_NET_MS;
        if ((jetzt - registry[i].letzter_kontakt_ms) > timeout_ms) {
            registry[i].ist_online = false;
            veroeffentlicheAvailability(registry[i].device_id, false);
        }
    }
}

/**
 * Sendet TIME-Pakete an alle Nodes bei Ablauf des Intervalls.
 * Platzhalter: NTP-Zeitquelle folgt in Phase 3.
 */
void aktualisiereZeitverteilung() {
    unsigned long jetzt = millis();
    if ((jetzt - masterStatus.letzter_time_sync_ms) < TIME_SYNC_INTERVAL_MS) return;
    masterStatus.letzter_time_sync_ms = jetzt;
    debugInfo("TIME: Zeitverteilung (Platzhalter)");
}

void aktualisiereKommunikation() {
    // Platzhalter: MQTT-Loop, ESP-NOW-Statusprüfung
}

// ============================================================
// 13. Debugfunktionen
// ============================================================

void gibStartmeldungAus() {
    if (!DEBUG_LOKAL_AKTIV) return;
    Serial.println("================================");
    Serial.println(PROJECT_NAME);
    Serial.print(DATEI_GERAET); Serial.print(" v"); Serial.println(DATEI_VERSION);
    Serial.print("Registry-Slots: "); Serial.println(SH_MAX_DEVICES);
    Serial.println("Neubasis-Geruest v0.2.0");
    Serial.println("================================");
}

// ============================================================
// 14. setup()
// ============================================================

void setup() {
    if (DEBUG_LOKAL_AKTIV) { Serial.begin(115200); delay(150); }
    gibStartmeldungAus();
    memset(registry, 0, sizeof(registry));
    registry_anzahl = 0;
    masterStatus = {};
    initialisiereHardware();
    initialisiereWlan();
    initialisiereEspNow();
    initialisiereMqtt();
}

// ============================================================
// 15. loop()
// ============================================================

void loop() {
    unsigned long jetzt = millis();
    aktualisiereKommunikation();
    if ((jetzt - masterStatus.letzter_availability_check_ms) >= AVAILABILITY_CHECK_MS) {
        masterStatus.letzter_availability_check_ms = jetzt;
        aktualisiereAvailability();
    }
    aktualisiereZeitverteilung();
    masterStatus.letzteSchleifeMs = jetzt;
    delay(LOOP_INTERVAL_MS);
}
