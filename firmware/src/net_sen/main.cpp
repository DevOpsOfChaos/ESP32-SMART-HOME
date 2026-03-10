/*
====================================================================
 Projekt   : SmartHome ESP32
 Gerät     : NET-SEN (Netzbetrieben, Sensor-Node)
 Datei     : main.cpp
 Version   : 0.2.0
 Stand     : 2026-03-10

 Funktion:
 Netzbetriebener Sensor-Node ohne Relais.
 Erfasst periodisch Klimawerte und meldet relevante Änderungen
 sofort als STATE, Präsenzereignisse als EVENT.

 Hardware (Basisvariante):
 - ESP32-C3 SuperMini
 - I2C-Sensorbus (AHT20/ENS160/VEML7700 je nach Bestückung)
 - optionaler PIR/Radar-Sensor
 - optionaler WS2812-Ring

 Architekturebene: A – Node
 Sicherheitsverhalten bei Masterverlust:
 - Messung läuft weiter
 - Events werden im Puffer gehalten und beim nächsten
   Verbindungsaufbau gesendet (Platzhalter)
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

constexpr char DATEI_GERAET[]  = "NET-SEN";
constexpr char DATEI_VERSION[] = "0.2.0";
constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;

constexpr unsigned long LOOP_INTERVAL_MS  = 50UL;
constexpr unsigned long HELLO_INTERVAL_MS = 300000UL;

// Messwerte (ungültige Startwerte)
struct SensorWerte {
    int16_t  temp_01c   = INT16_MIN;  // INT16_MIN = noch nie gemessen
    uint16_t hum_01pct  = 0xFFFF;     // 0xFFFF = ungültig
    uint16_t lux        = 0;
    uint16_t aqi        = 0;
    bool     motion     = false;
};

struct NodeState {
    bool masterBekannt;
    SensorWerte aktuell;
    SensorWerte letzteGemeldet;
    bool hatNeueMesswerte;
    unsigned long letzteMessungMs;
    unsigned long letzteStateMeldungMs;
    unsigned long letztesHelloMs;
    unsigned long letzteSchleifeMs;
    uint8_t seq_zaehler;
};

NodeState nodeStatus = {};

void debugInfo(const char* t) { if(DEBUG_LOKAL_AKTIV){Serial.print("[INFO] ");Serial.println(t);} }
void debugWarn(const char* t) { if(DEBUG_LOKAL_AKTIV){Serial.print("[WARN] ");Serial.println(t);} }

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) { pinMode(PIN_STATUS_LED, OUTPUT); digitalWrite(PIN_STATUS_LED, LOW); }
    if (PIN_PIR >= 0) pinMode(PIN_PIR, INPUT);
    // Fester Hardwarestandard: SDA = GPIO0, SCL = GPIO1.
    // Wire.begin(PIN_SENSOR_SDA, PIN_SENSOR_SCL);
}

void initialisiereEspNow() {
    debugInfo("ESP-NOW: Node-Initialisierung (Platzhalter)");
}

void sendeHello() {
    nodeStatus.letztesHelloMs = millis();
    debugInfo("HELLO: gesendet (Platzhalter)");
}

void sendeState() {
    nodeStatus.letzteStateMeldungMs = millis();
    nodeStatus.hatNeueMesswerte = false;
    nodeStatus.letzteGemeldet = nodeStatus.aktuell;
    debugInfo("STATE: gesendet (Platzhalter)");
}

void sendeEvent(uint8_t event_type) {
    (void)event_type;
    debugInfo("EVENT: gesendet (Platzhalter)");
}

/**
 * Prüft, ob eine sofortige STATE-Meldung nötig ist
 * (Messwert-Differenz überschreitet Schwellwert).
 */
bool sofortMeldungNoetig() {
    const SensorWerte& a = nodeStatus.aktuell;
    const SensorWerte& g = nodeStatus.letzteGemeldet;
    if (a.temp_01c != INT16_MIN && g.temp_01c != INT16_MIN) {
        if (abs((int)a.temp_01c - (int)g.temp_01c) >= TEMP_DELTA_01C) return true;
    }
    if (a.hum_01pct != 0xFFFF && g.hum_01pct != 0xFFFF) {
        if (abs((int)a.hum_01pct - (int)g.hum_01pct) >= HUM_DELTA_01PCT) return true;
    }
    if (abs((int)a.lux - (int)g.lux) >= (int)LUX_DELTA) return true;
    return false;
}

/**
 * Sensormesswerte einlesen.
 * Platzhalter: echte Sensoransteuerung folgt in Phase 2.
 */
void messeWerte() {
    nodeStatus.letzteMessungMs = millis();
    // Platzhalter: AHT20, ENS160, VEML7700 auslesen
    // nodeStatus.aktuell.temp_01c = ...
    // nodeStatus.aktuell.hum_01pct = ...
    // nodeStatus.aktuell.lux = ...
}

/**
 * PIR oder Radar auslesen und EVENT senden bei Änderung.
 */
void aktualisiereMotion() {
    if (PIN_PIR < 0) return;
    bool jetzt = (digitalRead(PIN_PIR) == HIGH);
    if (jetzt && !nodeStatus.aktuell.motion) {
        nodeStatus.aktuell.motion = true;
        sendeEvent(SH_EVENT_MOTION_DETECTED);
    } else if (!jetzt) {
        nodeStatus.aktuell.motion = false;
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
    Serial.println("================================");
}

void setup() {
    if (DEBUG_LOKAL_AKTIV) { Serial.begin(115200); delay(150); }
    gibStartmeldungAus();
    nodeStatus = {};
    initialisiereHardware();
    initialisiereEspNow();
    sendeHello();
}

void loop() {
    unsigned long jetzt = millis();
    aktualisiereKommunikation();
    messeWerte();
    aktualisiereMotion();
    bool istZeit = (jetzt - nodeStatus.letzteStateMeldungMs) >= STATE_INTERVAL_MS;
    if (istZeit || sofortMeldungNoetig()) sendeState();
    if ((jetzt - nodeStatus.letztesHelloMs) >= HELLO_INTERVAL_MS) sendeHello();
    nodeStatus.letzteSchleifeMs = jetzt;
    delay(LOOP_INTERVAL_MS);
}
