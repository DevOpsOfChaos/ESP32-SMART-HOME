/*
====================================================================
 Projekt   : SmartHome ESP32
 Gerät     : NET-ERL (Netzbetrieben, 1 Relais)
 Datei     : main.cpp
 Version   : 0.2.0
 Stand     : 2026-03-10

 Funktion:
 Netzbetriebener Node mit einem Relais.
 Basis für Lampensteuerung, Schaltaktoren und
 erweiterte Varianten (Flurlicht, Küchenlampe).

 Hardware (Basisvariante):
 - ESP32-C3 SuperMini
 - 1 Relais
 - optionaler lokaler Taster
 - optionaler Status-LED
 - optionale Sensoren (je nach fw_variant)

 Architekturebene: A – Node
 - spricht ausschließlich ESP-NOW mit dem Master
 - kein MQTT, kein Node-RED, kein Dashboard-Wissen

 Node-Aufgaben:
 1. HELLO an Master senden (Boot + periodisch)
 2. Lokalen Taster entprellen und als EVENT melden
 3. Relais schalten und Zustandsänderung als EVENT + STATE melden
 4. Periodischen STATE senden
 5. CMD/CFG vom Master entgegennehmen und verarbeiten
 6. Lokale Bedienung bei Masterverlust aufrechterhalten

 Sicherheitsverhalten bei Masterverlust:
 - Aktueller Relaiszustand bleibt erhalten
 - Lokaler Taster bleibt weiterhin funktionsfähig
 - Keine Panikabschaltung, keine unkontrollierten Resets

 Hinweise:
 - Secrets.h nicht im Repository.
 - Vorlage: firmware/include/Secrets.example.h
 - Debug nur in Testständen (DEVICE_DEBUG_AKTIV in AppConfig.h).
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

constexpr char DATEI_GERAET[]  = "NET-ERL";
constexpr char DATEI_VERSION[] = "0.2.0";

// ============================================================
// 3. Einstellungen vor dem Upload
// ============================================================
// Gerätespezifische Einstellungen in AppConfig.h.
// Vor dem Flashen prüfen:
//   - DEVICE_ID und DEVICE_NAME (AppConfig.h)
//   - Pinbelegung (PinConfig.h)
//   - FW_VARIANT korrekt gesetzt?

// ============================================================
// 4. Debug-Konfiguration
// ============================================================

constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;

// ============================================================
// 5. Konstanten, Zeitwerte und Grenzwerte
// ============================================================

// Hauptloop-Takt (ms).
// Begründung: 10 ms ermöglicht flüssige Taster-Entprellung
// und ACK-Timeout-Prüfung.
constexpr unsigned long LOOP_INTERVAL_MS = 10UL;

// Intervall für HELLO-Wiederholung im Normalbetrieb (ms).
// Begründung: 5 Minuten reichen, um dem Master die Verbindung
// zu bestätigen, ohne die Funkstrecke zu belasten.
constexpr unsigned long HELLO_INTERVAL_MS = 300000UL;

// ============================================================
// 6. Pinbelegung
// ============================================================
// Alle Pins in PinConfig.h.

// ============================================================
// 7. Datenstrukturen und Statusvariablen
// ============================================================

struct NodeState {
    bool     masterBekannt;          // HELLO_ACK bereits erhalten
    bool     relais0An;              // aktueller Relaiszustand
    bool     letzterTasterZustand;   // für Flankenauswertung
    bool     hatNeueMesswerte;       // STATE-Sendung ausstehend
    unsigned long letzteStateMeldungMs;
    unsigned long letztesHelloMs;
    unsigned long letzteSchleifeMs;
    uint8_t  seq_zaehler;            // ausgehende Sequenznummer
};

NodeState nodeStatus = {};

// ============================================================
// 8. Funktionsprototypen
// ============================================================

void initialisiereHardware();
void initialisiereEspNow();
void aktualisiereKommunikation();
void aktualisiereTaster();
void aktualisiereRelaisLogik();
void sendeHello();
void sendeState();
void sendeEvent(uint8_t event_type, uint8_t trigger);
void schalteRelais(bool an, uint8_t trigger);
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

/**
 * Pins initialisieren und Startzustand setzen.
 */
void initialisiereHardware() {
    // Status-LED
    if (PIN_STATUS_LED >= 0) {
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, LOW);
    }

    // Lokaler Taster (INPUT_PULLUP: LOW bei Betätigung)
    if (PIN_BUTTON_1 >= 0) {
        pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    }

    // Relais (Startzustand aus AppConfig)
    if (PIN_RELAY_1 >= 0) {
        pinMode(PIN_RELAY_1, OUTPUT);
        digitalWrite(PIN_RELAY_1, RELAY_DEFAULT_ON_BOOT ? HIGH : LOW);
    }
    nodeStatus.relais0An = RELAY_DEFAULT_ON_BOOT;

    // PIR-Eingang
    if (PIN_PIR >= 0) {
        pinMode(PIN_PIR, INPUT);
    }
}

/**
 * ESP-NOW für Node initialisieren.
 * Platzhalter: folgt in Phase 2.
 */
void initialisiereEspNow() {
    debugInfo("ESP-NOW: Node-Initialisierung (Platzhalter)");
}

// ============================================================
// 11. Kommunikationsfunktionen
// ============================================================

/**
 * HELLO-Paket an Master senden.
 * Enthält: device_id, device_name, Geräteklasse, Fähigkeiten.
 * Platzhalter: folgt in Phase 2.
 */
void sendeHello() {
    nodeStatus.letztesHelloMs = millis();
    debugInfo("HELLO: gesendet (Platzhalter)");
}

/**
 * Periodischen STATE senden.
 * STATE enthält aktuellen Gesamtzustand als TLV-Payload.
 * Platzhalter: TLV-Aufbau folgt in Phase 2.
 */
void sendeState() {
    nodeStatus.letzteStateMeldungMs = millis();
    nodeStatus.hatNeueMesswerte = false;
    debugInfo("STATE: gesendet (Platzhalter)");
}

/**
 * EVENT-Paket an Master senden.
 *
 * Parameter:
 *   event_type - SH_EVENT_* Konstante
 *   trigger    - SH_TRIGGER_* Ursache des Ereignisses
 *
 * Platzhalter: TLV-Aufbau folgt in Phase 2.
 */
void sendeEvent(uint8_t event_type, uint8_t trigger) {
    (void)event_type; (void)trigger;
    debugInfo("EVENT: gesendet (Platzhalter)");
}

/**
 * Eingehendes ESP-NOW-Paket verarbeiten.
 * Dispatcht CMD und CFG.
 */
void verarbeiteEingehendesEspNowPaket(
    const uint8_t* mac,
    const uint8_t* daten,
    int laenge)
{
    (void)mac;
    if (laenge < (int)SH_HEADER_SIZE) return;

    const SmartHome::MsgHeader* header =
        reinterpret_cast<const SmartHome::MsgHeader*>(daten);
    if (!SmartHome::isValidHeader(*header)) return;

    const uint8_t* payload = daten + SH_HEADER_SIZE;

    switch (header->msg_type) {
        case SH_MSG_HELLO_ACK:
            nodeStatus.masterBekannt = true;
            debugInfo("HELLO_ACK: Master bestätigt");
            break;
        case SH_MSG_CMD:
            if (header->payload_len >= sizeof(SmartHome::CmdPayload)) {
                verarbeiteCmd(*reinterpret_cast<const SmartHome::CmdPayload*>(payload));
            }
            break;
        case SH_MSG_CFG:
            if (header->payload_len >= sizeof(SmartHome::CfgPayload)) {
                verarbeiteCfg(*reinterpret_cast<const SmartHome::CfgPayload*>(payload));
            }
            break;
        case SH_MSG_TIME:
            debugInfo("TIME: empfangen (Platzhalter)");
            break;
        default:
            break;
    }
}

/**
 * CMD vom Master verarbeiten.
 * Dispatcht nach cmd_type.
 */
void verarbeiteCmd(const SmartHome::CmdPayload& cmd) {
    switch (cmd.cmd_type) {
        case SH_CMD_RELAY:
            // param1 = relay_index, param2 = 0/1/2 (aus/an/toggle)
            if (cmd.param1 == 0) {
                bool neuerZustand;
                if (cmd.param2 == 2) {
                    neuerZustand = !nodeStatus.relais0An;
                } else {
                    neuerZustand = (cmd.param2 == 1);
                }
                schalteRelais(neuerZustand, SH_TRIGGER_MASTER_CMD);
            }
            break;
        case SH_CMD_STATE_REQUEST:
            sendeState();
            break;
        default:
            debugWarn("CMD: Unbekannter cmd_type");
            break;
    }
}

/**
 * CFG vom Master verarbeiten.
 * Platzhalter: NVS-Speicherung folgt in Phase 2.
 */
void verarbeiteCfg(const SmartHome::CfgPayload& cfg) {
    (void)cfg;
    debugInfo("CFG: empfangen (Platzhalter)");
}

void aktualisiereKommunikation() {
    // Platzhalter: ESP-NOW-Statusprüfung, ACK-Retry-Verwaltung
}

// ============================================================
// 12. Logikfunktionen
// ============================================================

/**
 * Taster-Flankenauswertung.
 * Erkennt steigende Flanke (Loslassen bei INPUT_PULLUP),
 * sendet EVENT und toggelt Relais.
 */
void aktualisiereTaster() {
    if (PIN_BUTTON_1 < 0) return;

    bool jetzt = (digitalRead(PIN_BUTTON_1) == LOW);

    // Steigende Flanke (LOW -> HIGH nach Loslassen)
    if (!jetzt && nodeStatus.letzterTasterZustand) {
        schalteRelais(!nodeStatus.relais0An, SH_TRIGGER_MANUAL_BUTTON);
    }

    nodeStatus.letzterTasterZustand = jetzt;
}

/**
 * Relais schalten und Zustandsänderung melden.
 * Sendet immer EVENT, dann STATE.
 *
 * Parameter:
 *   an      - true = einschalten, false = ausschalten
 *   trigger - SH_TRIGGER_* Ursache
 */
void schalteRelais(bool an, uint8_t trigger) {
    if (nodeStatus.relais0An == an) return; // Keine Änderung

    nodeStatus.relais0An = an;

    if (PIN_RELAY_1 >= 0) {
        digitalWrite(PIN_RELAY_1, an ? HIGH : LOW);
    }

    // Zustandsänderung sofort als EVENT melden
    uint8_t event_type = an ? SH_EVENT_RELAY_CHANGED : SH_EVENT_RELAY_CHANGED;
    if (trigger == SH_TRIGGER_MANUAL_BUTTON) {
        event_type = SH_EVENT_RELAY_CHANGED;
    } else if (trigger == SH_TRIGGER_AUTO) {
        event_type = an ? SH_EVENT_LIGHT_AUTO_ON : SH_EVENT_LIGHT_AUTO_OFF;
    }

    sendeEvent(event_type, trigger);

    // STATE danach senden
    nodeStatus.hatNeueMesswerte = true;
}

void aktualisiereRelaisLogik() {
    // Platzhalter: spätere Automatik (PIR + Lichtschwellwert)
    // und konfigurierbarer Auto-Off-Timer hier einbauen.
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
    Serial.print("Variante: "); Serial.println(FW_VARIANT);
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
    aktualisiereTaster();
    aktualisiereRelaisLogik();

    // Periodischer STATE
    if ((jetzt - nodeStatus.letzteStateMeldungMs) >= STATE_INTERVAL_MS
        || nodeStatus.hatNeueMesswerte) {
        sendeState();
    }

    // Periodisches HELLO
    if ((jetzt - nodeStatus.letztesHelloMs) >= HELLO_INTERVAL_MS) {
        sendeHello();
    }

    nodeStatus.letzteSchleifeMs = jetzt;
    delay(LOOP_INTERVAL_MS);
}
