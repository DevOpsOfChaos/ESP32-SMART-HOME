/*
====================================================================
 Projekt   : SmartHome ESP32
 Gerät     : BAT-SEN (Batteriebetrieben, Sensor/Event)
 Datei     : main.cpp
 Version   : 0.2.0
 Stand     : 2026-03-10

 Funktion:
 Batteriebetriebener Sensor- und Event-Node.
 Schläft die meiste Zeit. Wacht auf bei Ereignissen
 (Taster, Reed-Kontakt) oder nach dem Schlafintervall.

 Hardware (Basisvariante):
 - ESP32-C3 SuperMini
 - 1..4 Taster oder Reed-Kontakte
 - optionaler Regensensor (ADC)
 - Batteriespannungsmessung (ADC)

 Besonderes Kommunikationsverhalten:
 - Wacht auf -> sendet HELLO (wenn nötig) oder STATE/EVENT
 - Öffnet kurzes RX-Fenster (RX_WINDOW_MS)
 - Empfängt HELLO_ACK oder CFG vom Master
 - Schläft danach wieder ein

 Architekturebene: A – Node
 - spricht ausschließlich ESP-NOW mit dem Master
 - kein MQTT, kein Node-RED, kein Dashboard-Wissen

 Sicherheitsverhalten bei Masterverlust:
 - Lokale Events werden sofort gespeichert (Platzhalter)
 - Gerät schläft weiterhin planmäßig
 - Keine Panikstrecke

 Hinweise:
 - Secrets.h nicht im Repository.
====================================================================
*/

#include <Arduino.h>
#include "AppConfig.h"
#include "PinConfig.h"
#include "../../include/ProjectVersion.h"
#include "../../include/BuildConfig.h"
#include "../../include/DebugConfig.h"
#include "../../lib/ShProtocol/src/Protocol.h"
#include "../../lib/ShProtocol/src/DeviceTypes.h"

constexpr char DATEI_GERAET[]  = "BAT-SEN";
constexpr char DATEI_VERSION[] = "0.2.0";
constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;

constexpr unsigned long LOOP_INTERVAL_MS = 10UL;

// Aufwachursachen
enum WakeReason : uint8_t {
    WAKE_TIMER       = 0x00,  // periodischer Schlafintervall
    WAKE_BUTTON_1    = 0x01,  // Taster 1 oder Reed ausgelöst
    WAKE_BUTTON_2    = 0x02,
    WAKE_BUTTON_3    = 0x03,
    WAKE_BUTTON_4    = 0x04,
    WAKE_REED        = 0x05,  // Fensterkontakt
    WAKE_RAIN        = 0x06,  // Regensensor
};

struct NodeState {
    bool masterBekannt;
    bool fensterOffen;
    bool regenErkannt;
    bool letzterButton1;
    bool letzterButton2;
    bool letzterButton3;
    bool letzterButton4;
    uint8_t  batterie_pct;
    uint16_t batterie_mv;
    WakeReason aufwachGrund;
    unsigned long aufgewachtMs;
    unsigned long letzteSchleifeMs;
    uint8_t seq_zaehler;
};

NodeState nodeStatus = {};

void debugInfo(const char* t) { if(DEBUG_LOKAL_AKTIV){Serial.print("[INFO] ");Serial.println(t);} }
void debugWarn(const char* t) { if(DEBUG_LOKAL_AKTIV){Serial.print("[WARN] ");Serial.println(t);} }

void initialisiereHardware() {
    if (PIN_BUTTON_1 >= 0) pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    if (PIN_BUTTON_2 >= 0) pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    if (PIN_BUTTON_3 >= 0) pinMode(PIN_BUTTON_3, INPUT_PULLUP);
    if (PIN_BUTTON_4 >= 0) pinMode(PIN_BUTTON_4, INPUT_PULLUP);
    if (PIN_REED >= 0)     pinMode(PIN_REED, INPUT_PULLUP);
}

void initialisiereEspNow() {
    debugInfo("ESP-NOW: Node-Initialisierung (Platzhalter)");
}

void sendeHello() {
    debugInfo("HELLO: gesendet (Platzhalter)");
}

void sendeState() {
    debugInfo("STATE: gesendet (Platzhalter)");
}

void sendeEvent(uint8_t event_type, uint8_t trigger) {
    (void)event_type; (void)trigger;
    debugInfo("EVENT: gesendet (Platzhalter)");
}

/**
 * Batteriespannung messen und Prozentwert berechnen.
 * Platzhalter: ADC-Kalibrierung und Spannungsteiler-Berechnung
 * müssen je Platine angepasst werden.
 */
void messeBatterie() {
    if (PIN_BATTERY_ADC < 0) {
        nodeStatus.batterie_pct = 255; // nicht vorhanden
        nodeStatus.batterie_mv  = 0;
        return;
    }
    // Platzhalter: analogRead + Umrechnung
    nodeStatus.batterie_pct = 100; // Platzhalter
    nodeStatus.batterie_mv  = 3700; // Platzhalter: 3.7 V Li-Ion
}

/**
 * Reed-Kontakt auslesen und EVENT senden bei Zustandsänderung.
 * LOW = Fenster offen (Magnet entfernt, INPUT_PULLUP)
 */
void aktualisiereFensterKontakt() {
    if (PIN_REED < 0) return;
    bool jetzt_offen = (digitalRead(PIN_REED) == LOW);
    if (jetzt_offen != nodeStatus.fensterOffen) {
        nodeStatus.fensterOffen = jetzt_offen;
        sendeEvent(
            jetzt_offen ? SH_EVENT_WINDOW_OPENED : SH_EVENT_WINDOW_CLOSED,
            SH_TRIGGER_UNKNOWN);
    }
}

/**
 * Taster auslesen und EVENT senden bei Betätigung.
 * Nur fallende Flanke (LOW bei INPUT_PULLUP) auswerten.
 */
void aktualisiereTaster() {
    if (PIN_BUTTON_1 >= 0) {
        bool jetzt = (digitalRead(PIN_BUTTON_1) == LOW);
        if (jetzt && !nodeStatus.letzterButton1) {
            sendeEvent(SH_EVENT_BUTTON_PRESS, SH_TRIGGER_MANUAL_BUTTON);
        }
        nodeStatus.letzterButton1 = jetzt;
    }
    // Kanal 2..4 analog (Platzhalter)
}

/**
 * Entscheidet, ob das Gerät in den Deep-Sleep gehen soll.
 * Erst schlafen, wenn:
 *  - WACH_NACH_EVENT_MS nach letztem Ereignis abgelaufen
 *  - alle ausstehenden ACKs erledigt (Platzhalter)
 */
void pruefeSchlaf() {
    if ((millis() - nodeStatus.aufgewachtMs) >= WACH_NACH_EVENT_MS) {
        debugInfo("Deep-Sleep einleiten (Platzhalter)");
        // Platzhalter: esp_deep_sleep(SLEEP_INTERVAL_S * 1000000ULL);
    }
}

void aktualisiereKommunikation() { /* Platzhalter */ }

void verarbeiteEingehendesEspNowPaket(const uint8_t* mac, const uint8_t* daten, int laenge) {
    (void)mac; if (laenge < (int)SH_HEADER_SIZE) return;
    const SmartHome::MsgHeader* h = reinterpret_cast<const SmartHome::MsgHeader*>(daten);
    if (!SmartHome::isValidHeader(*h)) return;
    if (h->msg_type == SH_MSG_HELLO_ACK) nodeStatus.masterBekannt = true;
}

void gibStartmeldungAus() {
    if (!DEBUG_LOKAL_AKTIV) return;
    Serial.println("================================");
    Serial.print(DATEI_GERAET); Serial.print(" v"); Serial.println(DATEI_VERSION);
    Serial.print("ID: "); Serial.println(DEVICE_ID);
    Serial.print("Schlafintervall: "); Serial.print(SLEEP_INTERVAL_S); Serial.println(" s");
    Serial.println("================================");
}

void setup() {
    if (DEBUG_LOKAL_AKTIV) { Serial.begin(115200); delay(50); }
    gibStartmeldungAus();
    nodeStatus = {};
    nodeStatus.aufgewachtMs = millis();
    // Aufwachursache bestimmen (Platzhalter: esp_sleep_get_wakeup_cause())
    nodeStatus.aufwachGrund = WAKE_TIMER;
    initialisiereHardware();
    initialisiereEspNow();
    messeBatterie();
    // HELLO bei erstem Boot oder nach längerer Pause
    sendeHello();
}

void loop() {
    unsigned long jetzt = millis();
    aktualisiereKommunikation();
    aktualisiereFensterKontakt();
    aktualisiereTaster();

    // Periodischer STATE: immer beim Aufwachen senden
    static bool ersterState = true;
    if (ersterState) {
        sendeState();
        ersterState = false;
    }

    // Schlaf einleiten wenn Wachhaltezeit abgelaufen
    pruefeSchlaf();

    nodeStatus.letzteSchleifeMs = jetzt;
    delay(LOOP_INTERVAL_MS);
}
