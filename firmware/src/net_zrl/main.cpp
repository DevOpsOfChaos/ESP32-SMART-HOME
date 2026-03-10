/*
====================================================================
 Projekt   : SmartHome ESP32
 Gerät     : NET-ZRL (Netzbetrieben, 2 Relais)
 Datei     : main.cpp
 Version   : 0.2.1
 Stand     : 2026-03-10

 Funktion:
 Netzbetriebener Node mit zwei Relais.
 Basis für Rolladensteuerung und Doppelschaltaktoren.

 Hardware (Basisvariante):
 - ESP32-C3 SuperMini
 - 2 Relais
 - optionale Taster (Auf / Ab / Stop)
 - optionale Sensoren

 Wichtige Architekturregel:
 Rolladen-Schutzlogik gilt nur im COVER_MODUS_AKTIV.
 Im allgemeinen NET-ZRL-Betrieb müssen beide Relais unabhängig
 nutzbar bleiben, z. B. für zwei Lichtkreise.

 Architekturebene: A – Node
 - spricht ausschließlich ESP-NOW mit dem Master
 - kein MQTT, kein Node-RED, kein Dashboard-Wissen

 Sicherheitsverhalten bei Masterverlust:
 - Laufender lokaler Betrieb bleibt erhalten
 - Lokale Taster bleiben funktionsfähig
 - Keine Panikabschaltung

 Hinweise:
 - Secrets.h nicht im Repository.
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

// ============================================================
// 2. Versions- und Geräteinformationen
// ============================================================

constexpr char DATEI_GERAET[]  = "NET-ZRL";
constexpr char DATEI_VERSION[] = "0.2.1";

// ============================================================
// 4. Debug-Konfiguration
// ============================================================

constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;

// ============================================================
// 5. Konstanten, Zeitwerte und Grenzwerte
// ============================================================

constexpr unsigned long LOOP_INTERVAL_MS  = 10UL;
constexpr unsigned long HELLO_INTERVAL_MS = 300000UL;

// ============================================================
// 7. Datenstrukturen und Statusvariablen
// ============================================================

// Rolladen-Fahrzustände
enum CoverState : uint8_t {
    COVER_GESTOPPT   = SH_COVER_STATE_STOPPED,
    COVER_FAEHRT_AUF = SH_COVER_STATE_MOVING_UP,
    COVER_FAEHRT_AB  = SH_COVER_STATE_MOVING_DOWN
};

struct NodeState {
    bool     masterBekannt;
    bool     relais0An;         // Auf / Kreis 1
    bool     relais1An;         // Ab / Kreis 2
    CoverState cover_zustand;
    uint8_t  cover_position;    // 0..100, 255 = unbekannt
    bool     relais_verriegelt; // nur im Rolladenmodus relevant
    unsigned long verriegelungBisMs;
    unsigned long letzteStateMeldungMs;
    unsigned long letztesHelloMs;
    unsigned long letzteSchleifeMs;
    uint8_t  seq_zaehler;
};

NodeState nodeStatus = {};

// ============================================================
// 8. Funktionsprototypen
// ============================================================

void initialisiereHardware();
void initialisiereEspNow();
void aktualisiereKommunikation();
void aktualisiereRelaisVerriegelung();
void sendeHello();
void sendeState();
void sendeEvent(uint8_t event_type, uint8_t trigger);
void schalteRelaisGesichert(uint8_t idx, bool an, uint8_t trigger);
void stoppeFahrt(uint8_t trigger);
void verarbeiteEingehendesEspNowPaket(const uint8_t* mac, const uint8_t* daten, int laenge);
void verarbeiteCmd(const SmartHome::CmdPayload& cmd);
void verarbeiteCfg(const SmartHome::CfgPayload& cfg);
void gibStartmeldungAus();
void debugInfo(const char* text);
void debugWarn(const char* text);

// ============================================================
// 9. Hilfsfunktionen
// ============================================================

void debugInfo(const char* text) {
    if (DEBUG_LOKAL_AKTIV) { Serial.print("[INFO] "); Serial.println(text); }
}
void debugWarn(const char* text) {
    if (DEBUG_LOKAL_AKTIV) { Serial.print("[WARN] "); Serial.println(text); }
}

// ============================================================
// 10. Hardware- und Sensorfunktionen
// ============================================================

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) { pinMode(PIN_STATUS_LED, OUTPUT); digitalWrite(PIN_STATUS_LED, LOW); }
    if (PIN_BUTTON_1 >= 0) pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    if (PIN_BUTTON_2 >= 0) pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    if (PIN_BUTTON_3 >= 0) pinMode(PIN_BUTTON_3, INPUT_PULLUP);
    if (PIN_RELAY_1 >= 0) { pinMode(PIN_RELAY_1, OUTPUT); digitalWrite(PIN_RELAY_1, LOW); }
    if (PIN_RELAY_2 >= 0) { pinMode(PIN_RELAY_2, OUTPUT); digitalWrite(PIN_RELAY_2, LOW); }
    nodeStatus.relais0An = false;
    nodeStatus.relais1An = false;
    nodeStatus.cover_zustand = COVER_GESTOPPT;
    nodeStatus.cover_position = 255; // unbekannt nach Boot
}

void initialisiereEspNow() {
    debugInfo("ESP-NOW: Node-Initialisierung (Platzhalter)");
}

// ============================================================
// 11. Kommunikationsfunktionen
// ============================================================

void sendeHello() {
    nodeStatus.letztesHelloMs = millis();
    debugInfo("HELLO: gesendet (Platzhalter)");
}

void sendeState() {
    nodeStatus.letzteStateMeldungMs = millis();
    debugInfo("STATE: gesendet (Platzhalter)");
}

void sendeEvent(uint8_t event_type, uint8_t trigger) {
    (void)event_type; (void)trigger;
    debugInfo("EVENT: gesendet (Platzhalter)");
}

void verarbeiteCmd(const SmartHome::CmdPayload& cmd) {
    switch (cmd.cmd_type) {
        case SH_CMD_COVER:
            if (!COVER_MODUS_AKTIV) {
                debugWarn("CMD: Cover-Befehl ignoriert, Cover-Modus ist aus");
                break;
            }
            // param1 = SH_COVER_STATE_*, param2 = Zielposition (optional)
            switch (cmd.param1) {
                case SH_COVER_STATE_MOVING_UP:
                    stoppeFahrt(SH_TRIGGER_MASTER_CMD); // Sicherheitsstopp vor Richtungswechsel
                    schalteRelaisGesichert(0, true, SH_TRIGGER_MASTER_CMD);
                    nodeStatus.cover_zustand = COVER_FAEHRT_AUF;
                    sendeEvent(SH_EVENT_COVER_UP, SH_TRIGGER_MASTER_CMD);
                    break;
                case SH_COVER_STATE_MOVING_DOWN:
                    stoppeFahrt(SH_TRIGGER_MASTER_CMD);
                    schalteRelaisGesichert(1, true, SH_TRIGGER_MASTER_CMD);
                    nodeStatus.cover_zustand = COVER_FAEHRT_AB;
                    sendeEvent(SH_EVENT_COVER_DOWN, SH_TRIGGER_MASTER_CMD);
                    break;
                case SH_COVER_STATE_STOPPED:
                    stoppeFahrt(SH_TRIGGER_MASTER_CMD);
                    break;
            }
            break;
        case SH_CMD_RELAY:
            if (cmd.param1 < 2) {
                bool an = (cmd.param2 == 2)
                    ? (cmd.param1 == 0 ? !nodeStatus.relais0An : !nodeStatus.relais1An)
                    : (cmd.param2 == 1);
                schalteRelaisGesichert(cmd.param1, an, SH_TRIGGER_MASTER_CMD);
            }
            break;
        case SH_CMD_STATE_REQUEST:
            sendeState();
            break;
        default:
            debugWarn("CMD: Unbekannter Typ");
            break;
    }
}

void verarbeiteCfg(const SmartHome::CfgPayload& cfg) {
    (void)cfg;
    debugInfo("CFG: empfangen (Platzhalter)");
}

void aktualisiereKommunikation() {
    // Platzhalter
}

void verarbeiteEingehendesEspNowPaket(const uint8_t* mac, const uint8_t* daten, int laenge) {
    (void)mac;
    if (laenge < (int)SH_HEADER_SIZE) return;
    const SmartHome::MsgHeader* h = reinterpret_cast<const SmartHome::MsgHeader*>(daten);
    if (!SmartHome::isValidHeader(*h)) return;
    const uint8_t* p = daten + SH_HEADER_SIZE;
    switch (h->msg_type) {
        case SH_MSG_HELLO_ACK: nodeStatus.masterBekannt = true; break;
        case SH_MSG_CMD:
            if (h->payload_len >= sizeof(SmartHome::CmdPayload))
                verarbeiteCmd(*reinterpret_cast<const SmartHome::CmdPayload*>(p));
            break;
        case SH_MSG_CFG:
            if (h->payload_len >= sizeof(SmartHome::CfgPayload))
                verarbeiteCfg(*reinterpret_cast<const SmartHome::CfgPayload*>(p));
            break;
        default: break;
    }
}

// ============================================================
// 12. Logikfunktionen
// ============================================================

/**
 * Schaltet ein Relais abhängig vom aktiven Gerätemodus.
 *
 * Rolladenmodus (COVER_MODUS_AKTIV = true):
 * - beide Relais dürfen nie gleichzeitig aktiv sein
 * - Reversier-Sperrzeit nach Stop/Richtungswechsel wird beachtet
 *
 * Allgemeiner Zwei-Relais-Modus (COVER_MODUS_AKTIV = false):
 * - beide Relais sind unabhängig schaltbar
 * - keine pauschale Sperrlogik
 */
void schalteRelaisGesichert(uint8_t idx, bool an, uint8_t trigger) {
    if (idx > 1) return;

    if (COVER_MODUS_AKTIV && an) {
        if (idx == 0 && nodeStatus.relais1An) {
            debugWarn("RELAY: Cover-Modus blockiert Relais 0, Relais 1 ist noch aktiv");
            return;
        }
        if (idx == 1 && nodeStatus.relais0An) {
            debugWarn("RELAY: Cover-Modus blockiert Relais 1, Relais 0 ist noch aktiv");
            return;
        }
        if (nodeStatus.relais_verriegelt && millis() < nodeStatus.verriegelungBisMs) {
            debugWarn("RELAY: Cover-Modus Reversier-Sperrzeit läuft noch");
            return;
        }
    }

    if (idx == 0) {
        nodeStatus.relais0An = an;
        if (PIN_RELAY_1 >= 0) digitalWrite(PIN_RELAY_1, an ? HIGH : LOW);
    } else {
        nodeStatus.relais1An = an;
        if (PIN_RELAY_2 >= 0) digitalWrite(PIN_RELAY_2, an ? HIGH : LOW);
    }

    (void)trigger;
}

/**
 * Stoppt den Cover-Betrieb und startet nur im Cover-Modus
 * eine Reversier-Sperrzeit.
 */
void stoppeFahrt(uint8_t trigger) {
    bool war_aktiv = nodeStatus.relais0An || nodeStatus.relais1An;

    if (PIN_RELAY_1 >= 0) digitalWrite(PIN_RELAY_1, LOW);
    if (PIN_RELAY_2 >= 0) digitalWrite(PIN_RELAY_2, LOW);
    nodeStatus.relais0An = false;
    nodeStatus.relais1An = false;
    nodeStatus.cover_zustand = COVER_GESTOPPT;

    if (COVER_MODUS_AKTIV && war_aktiv) {
        nodeStatus.relais_verriegelt = true;
        nodeStatus.verriegelungBisMs = millis() + COVER_REVERSE_LOCK_MS;
        sendeEvent(SH_EVENT_COVER_STOP, trigger);
    }
}

/**
 * Prüft, ob die Reversier-Sperrzeit abgelaufen ist.
 * Nur im Cover-Modus relevant.
 */
void aktualisiereRelaisVerriegelung() {
    if (!COVER_MODUS_AKTIV) {
        nodeStatus.relais_verriegelt = false;
        return;
    }

    if (nodeStatus.relais_verriegelt && millis() >= nodeStatus.verriegelungBisMs) {
        nodeStatus.relais_verriegelt = false;
    }
}

// ============================================================
// 13. Debugfunktionen
// ============================================================

void gibStartmeldungAus() {
    if (!DEBUG_LOKAL_AKTIV) return;
    Serial.println("================================");
    Serial.println(PROJECT_NAME);
    Serial.print(DATEI_GERAET); Serial.print(" v"); Serial.println(DATEI_VERSION);
    Serial.print("ID: "); Serial.println(DEVICE_ID);
    Serial.print("Cover-Modus: "); Serial.println(COVER_MODUS_AKTIV ? "aktiv" : "inaktiv");
    Serial.println("================================");
}

// ============================================================
// 14. setup()
// ============================================================

void setup() {
    if (DEBUG_LOKAL_AKTIV) { Serial.begin(115200); delay(150); }
    gibStartmeldungAus();
    nodeStatus = {};
    initialisiereHardware();
    initialisiereEspNow();
    sendeHello();
}

// ============================================================
// 15. loop()
// ============================================================

void loop() {
    unsigned long jetzt = millis();
    aktualisiereKommunikation();
    aktualisiereRelaisVerriegelung();
    if ((jetzt - nodeStatus.letzteStateMeldungMs) >= STATE_INTERVAL_MS) sendeState();
    if ((jetzt - nodeStatus.letztesHelloMs) >= HELLO_INTERVAL_MS) sendeHello();
    nodeStatus.letzteSchleifeMs = jetzt;
    delay(LOOP_INTERVAL_MS);
}
