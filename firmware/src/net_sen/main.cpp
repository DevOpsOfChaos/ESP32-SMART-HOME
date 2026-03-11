/*
====================================================================
 Projekt   : SmartHome ESP32
 Geraet    : NET-SEN (Netzbetrieben, Sensor-Basis)
 Datei     : main.cpp
 Version   : 0.2.2
 Stand     : 2026-03-10

 Funktion:
 Allgemeine netzbetriebene Sensor-Basis in derselben ESP-NOW-Linie
 wie NET-ERL.
 - feste node_id net_sen_01
 - HELLO beim Start
 - HEARTBEAT zyklisch
 - STATE_REPORT mit generischen Sensorfeldern
 - optionales Motion-EVENT bei konfiguriertem PIR-Pin
====================================================================
*/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include <DHT.h>

#if __has_include(<esp_arduino_version.h>)
  #include <esp_arduino_version.h>
#endif

#ifndef ESP_ARDUINO_VERSION_MAJOR
  #define ESP_ARDUINO_VERSION_MAJOR 2
#endif

#include "AppConfig.h"
#include "PinConfig.h"
#include "../../include/ProjectVersion.h"
#include "../../include/DebugConfig.h"
#include "../../lib/ShProtocol/src/Protocol.h"
#include "../../lib/ShProtocol/src/DeviceTypes.h"

constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;
constexpr char DATEI_GERAET[] = "NET-SEN";
constexpr char DATEI_VERSION[] = "0.2.2";
const uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
constexpr uint8_t DHT_SENSOR_TYP = DHT22;
constexpr unsigned long DHT_WARMUP_MS = 2500UL;
constexpr unsigned long DHT_READ_INTERVAL_MS = 2500UL;

DHT dht(PIN_DHT22_DATA, DHT_SENSOR_TYP);

struct NodeState {
    bool master_bekannt;
    bool master_mac_gueltig;
    bool fault;
    bool motion;
    bool state_report_offen;
    int16_t temp_01c;
    uint16_t hum_01pct;
    uint16_t lux;
    unsigned long letztes_hello_ms;
    unsigned long letzter_heartbeat_ms;
    unsigned long letzter_state_ms;
    unsigned long letzter_state_wait_log_ms;
    uint8_t master_mac[6];
    uint8_t naechste_seq;
};

struct SensorRuntime {
    unsigned long boot_ms;
    unsigned long letzter_read_ms;
    bool erster_read_abgeschlossen;
};

NodeState nodeStatus = {};
SensorRuntime sensorStatus = {};

void logf(const char* level, const char* format, ...) {
    if (!DEBUG_LOKAL_AKTIV) return;

    char message[192];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    Serial.print("[");
    Serial.print(level);
    Serial.print("] ");
    Serial.println(message);
}

void copyText(char* target, size_t targetSize, const char* source) {
    if (!target || targetSize == 0U) return;
    if (!source) {
        target[0] = '\0';
        return;
    }

    strncpy(target, source, targetSize - 1U);
    target[targetSize - 1U] = '\0';
}

void logMac(const char* prefix, const uint8_t* mac) {
    char macText[18] = {0};
    SmartHome::macToString(mac, macText);
    logf("INFO", "%s%s", prefix, macText);
}

bool istBroadcastMac(const uint8_t* mac) {
    return mac != nullptr && memcmp(mac, BROADCAST_MAC, sizeof(BROADCAST_MAC)) == 0;
}

uint16_t ermittleCaps() {
    uint16_t caps = SH_CAP_TEMP | SH_CAP_HUM;
    if (PIN_PIR >= 0) {
        caps |= SH_CAP_MOTION;
    }
    return caps;
}

bool stellePeerSicher(const uint8_t* mac) {
    if (mac == nullptr) return false;
    if (!istBroadcastMac(mac) && !SmartHome::isValidMac(mac)) return false;
    if (esp_now_is_peer_exist(mac)) return true;

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, mac, 6);
    peerInfo.channel = (uint8_t)WLAN_KANAL;
    peerInfo.encrypt = false;

    esp_err_t err = esp_now_add_peer(&peerInfo);
    if (err != ESP_OK) {
        logf("WARN", "Peer konnte nicht angelegt werden (err=%d)", (int)err);
        return false;
    }

    logMac("Peer angelegt: ", mac);
    return true;
}

bool sendePaket(const uint8_t* zielMac, uint8_t msgType, const void* payload, size_t payloadLen, const char* label) {
    if (zielMac == nullptr || payloadLen > SH_MAX_PAYLOAD_BYTES) return false;
    if (!stellePeerSicher(zielMac)) return false;

    uint8_t buffer[SH_ESPNOW_MAX_BYTES] = {0};
    SmartHome::MsgHeader header = {};
    SmartHome::fillHeader(header, msgType, nodeStatus.naechste_seq++, 0, (uint16_t)payloadLen);

    uint8_t* payloadBuffer = buffer + SH_HEADER_SIZE;
    if (payloadLen > 0U && payload != nullptr) {
        memcpy(payloadBuffer, payload, payloadLen);
    }

    SmartHome::finalizePacketCrc(header, payloadBuffer);
    memcpy(buffer, &header, sizeof(header));

    esp_err_t err = esp_now_send(zielMac, buffer, SH_HEADER_SIZE + payloadLen);
    if (err != ESP_OK) {
        logf("WARN", "%s konnte nicht gesendet werden (err=%d)", label, (int)err);
        return false;
    }

    logf("INFO", "%s gesendet", label);
    return true;
}

bool sendeHello() {
    SmartHome::HelloPayload payload = {};
    copyText(payload.device_id, sizeof(payload.device_id), DEVICE_ID);
    copyText(payload.device_name, sizeof(payload.device_name), DEVICE_NAME);
    payload.device_class = SH_CLASS_NET_SEN;

    uint16_t caps = ermittleCaps();
    payload.caps_hi = (uint8_t)((caps >> 8) & 0xFFU);
    payload.caps_lo = (uint8_t)(caps & 0xFFU);
    payload.power_type = SH_POWER_MAINS;
    payload.fw_version = 0;
    payload.boot_counter = 1U;

    nodeStatus.letztes_hello_ms = millis();
    return sendePaket(BROADCAST_MAC, SH_MSG_HELLO, &payload, sizeof(payload), "HELLO");
}

bool sendeHeartbeat() {
    if (!nodeStatus.master_mac_gueltig) {
        logf("WARN", "HEARTBEAT verschoben: Master-MAC unbekannt");
        return false;
    }

    SmartHome::HeartbeatPayload payload = {};
    copyText(payload.node_id, sizeof(payload.node_id), DEVICE_ID);
    payload.uptime_s = millis() / 1000UL;

    if (sendePaket(nodeStatus.master_mac, SH_MSG_HEARTBEAT, &payload, sizeof(payload), "HEARTBEAT")) {
        nodeStatus.letzter_heartbeat_ms = millis();
        return true;
    }

    return false;
}

bool sendeStateReport() {
    if (!nodeStatus.master_mac_gueltig) {
        logf("WARN", "STATE_REPORT wartet auf HELLO_ACK");
        return false;
    }

    SmartHome::SensorStateReportPayload payload = {};
    copyText(payload.node_id, sizeof(payload.node_id), DEVICE_ID);
    payload.temp_01c = nodeStatus.temp_01c;
    payload.hum_01pct = nodeStatus.hum_01pct;
    payload.lux = nodeStatus.lux;
    payload.motion = nodeStatus.motion ? 1U : 0U;
    payload.fault = nodeStatus.fault ? 1U : 0U;

    if (sendePaket(nodeStatus.master_mac, SH_MSG_STATE, &payload, sizeof(payload), "STATE_REPORT")) {
        nodeStatus.state_report_offen = false;
        nodeStatus.letzter_state_ms = millis();
        return true;
    }

    return false;
}

bool sendeEvent(uint8_t eventType, uint8_t trigger, uint8_t param1, uint16_t param2) {
    if (!nodeStatus.master_mac_gueltig) {
        return false;
    }

    SmartHome::EventReportPayload payload = {};
    copyText(payload.node_id, sizeof(payload.node_id), DEVICE_ID);
    payload.event_type = eventType;
    payload.trigger = trigger;
    payload.param1 = param1;
    payload.param2 = param2;

    return sendePaket(nodeStatus.master_mac, SH_MSG_EVENT, &payload, sizeof(payload), "EVENT");
}

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) {
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, LOW);
    }
    if (PIN_PIR >= 0) {
        pinMode(PIN_PIR, INPUT);
    }

    nodeStatus.temp_01c = INT16_MIN;
    nodeStatus.hum_01pct = 0xFFFFU;
    nodeStatus.lux = 0xFFFFU;
    nodeStatus.motion = false;
    nodeStatus.fault = false;
    nodeStatus.state_report_offen = false;

    dht.begin();
    sensorStatus.boot_ms = millis();
    sensorStatus.letzter_read_ms = 0UL;
    sensorStatus.erster_read_abgeschlossen = false;
}

void initialisiereFunk() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.setSleep(false);

    esp_err_t kanalErr = esp_wifi_set_channel((uint8_t)WLAN_KANAL, WIFI_SECOND_CHAN_NONE);
    if (kanalErr != ESP_OK) {
        logf("WARN", "WLAN-Kanal %d konnte nicht gesetzt werden (err=%d)", WLAN_KANAL, (int)kanalErr);
    }

    if (esp_now_init() != ESP_OK) {
        logf("WARN", "ESP-NOW Initialisierung fehlgeschlagen");
        return;
    }

    if (!stellePeerSicher(BROADCAST_MAC)) {
        logf("WARN", "Broadcast-Peer konnte nicht vorbereitet werden");
    }
    logf("INFO", "ESP-NOW bereit auf Kanal %d", WLAN_KANAL);
}

void gibStartmeldungAus() {
    if (!DEBUG_LOKAL_AKTIV) return;

    Serial.println("================================");
    Serial.println(PROJECT_NAME);
    Serial.print(DATEI_GERAET);
    Serial.print(" v");
    Serial.println(DATEI_VERSION);
    Serial.print("Node-ID: ");
    Serial.println(DEVICE_ID);
    Serial.print("FW: ");
    Serial.println(PROJECT_VERSION);
    Serial.print("Minimalpfad: DHT22 an GPIO");
    Serial.println(PIN_DHT22_DATA);
    Serial.println("================================");
}

int16_t rundeTempZu01C(float temperaturC) {
    return static_cast<int16_t>(lroundf(temperaturC * 10.0f));
}

uint16_t rundeHumZu01Pct(float feuchtePct) {
    long gerundet = lroundf(feuchtePct * 10.0f);
    if (gerundet < 0L) gerundet = 0L;
    if (gerundet > 1000L) gerundet = 1000L;
    return static_cast<uint16_t>(gerundet);
}

bool dhtStateBereit() {
    return sensorStatus.erster_read_abgeschlossen;
}

void setzeDhtLesefehler(const char* grund) {
    const bool zustandGeaendert =
        !nodeStatus.fault ||
        nodeStatus.temp_01c != INT16_MIN ||
        nodeStatus.hum_01pct != 0xFFFFU;

    nodeStatus.temp_01c = INT16_MIN;
    nodeStatus.hum_01pct = 0xFFFFU;
    nodeStatus.fault = true;
    sensorStatus.erster_read_abgeschlossen = true;

    if (zustandGeaendert) {
        nodeStatus.state_report_offen = true;
    }

    logf("WARN", "DHT22 Lesefehler an GPIO%d (%s)", PIN_DHT22_DATA, grund);
}

void uebernehmeDhtMessung(float temperaturC, float feuchtePct) {
    const int16_t neueTemp = rundeTempZu01C(temperaturC);
    const uint16_t neueHum = rundeHumZu01Pct(feuchtePct);
    const bool tempNeuOderAnders =
        nodeStatus.temp_01c == INT16_MIN ||
        abs((int)neueTemp - (int)nodeStatus.temp_01c) >= TEMP_DELTA_01C;
    const bool humNeuOderAnders =
        nodeStatus.hum_01pct == 0xFFFFU ||
        abs((int)neueHum - (int)nodeStatus.hum_01pct) >= HUM_DELTA_01PCT;

    nodeStatus.temp_01c = neueTemp;
    nodeStatus.hum_01pct = neueHum;
    nodeStatus.fault = false;
    sensorStatus.erster_read_abgeschlossen = true;

    if (tempNeuOderAnders || humNeuOderAnders) {
        nodeStatus.state_report_offen = true;
    }

    logf("INFO", "DHT22 temp=%d.%dC hum=%u.%u%%", neueTemp / 10, abs(neueTemp % 10), neueHum / 10, neueHum % 10);
}

void messeSensoren() {
    const unsigned long jetzt = millis();

    // DHT22 braucht nach dem Booten und zwischen zwei Messungen echte Ruhe.
    if ((jetzt - sensorStatus.boot_ms) < DHT_WARMUP_MS) return;
    if (sensorStatus.letzter_read_ms != 0UL &&
        (jetzt - sensorStatus.letzter_read_ms) < DHT_READ_INTERVAL_MS) {
        return;
    }

    sensorStatus.letzter_read_ms = jetzt;

    const float feuchtePct = dht.readHumidity();
    const float temperaturC = dht.readTemperature();

    if (isnan(feuchtePct) || isnan(temperaturC)) {
        setzeDhtLesefehler("NaN");
        return;
    }

    if (feuchtePct < 0.0f || feuchtePct > 100.0f || temperaturC < -40.0f || temperaturC > 80.0f) {
        setzeDhtLesefehler("ausserhalb DHT22-Bereich");
        return;
    }

    uebernehmeDhtMessung(temperaturC, feuchtePct);
}

void aktualisiereMotion() {
    if (PIN_PIR < 0) return;

    bool jetzt = (digitalRead(PIN_PIR) == HIGH);
    if (jetzt != nodeStatus.motion) {
        nodeStatus.motion = jetzt;
        nodeStatus.state_report_offen = true;
        if (jetzt) {
            sendeEvent(SH_EVENT_MOTION_DETECTED, SH_TRIGGER_AUTO, 0U, 0U);
        }
    }
}

void verarbeiteHelloAck(const uint8_t* senderMac, const SmartHome::HelloAckPayload& payload) {
    if (payload.ack_status != SH_ACK_OK) {
        logf("WARN", "HELLO_ACK abgelehnt");
        return;
    }

    memcpy(nodeStatus.master_mac, senderMac, 6);
    nodeStatus.master_mac_gueltig = true;
    nodeStatus.master_bekannt = true;
    stellePeerSicher(nodeStatus.master_mac);
    logf("INFO", "HELLO_ACK empfangen, Master-Kanal=%u", payload.channel);
    logMac("Master-MAC: ", senderMac);
    nodeStatus.state_report_offen = dhtStateBereit();
}

void verarbeiteCmd(const SmartHome::CmdPayload& cmd) {
    if (cmd.cmd_type == SH_CMD_STATE_REQUEST) {
        nodeStatus.state_report_offen = true;
        return;
    }

    logf("WARN", "CMD ignoriert (cmd=%u)", (unsigned)cmd.cmd_type);
}

void verarbeiteEspNowPaket(const uint8_t* senderMac, const uint8_t* daten, int laenge) {
    if (senderMac == nullptr || daten == nullptr || laenge < (int)sizeof(SmartHome::MsgHeader)) {
        logf("WARN", "ESP-NOW Paket verworfen: ungueltige Eingabe");
        return;
    }

    if (!SmartHome::hasValidPacketCrc(daten, (size_t)laenge)) {
        logf("WARN", "ESP-NOW Paket verworfen: CRC/Header ungueltig");
        return;
    }

    const SmartHome::MsgHeader* header = reinterpret_cast<const SmartHome::MsgHeader*>(daten);
    const uint8_t* payload = daten + SH_HEADER_SIZE;

    switch (header->msg_type) {
        case SH_MSG_HELLO_ACK:
            if (header->payload_len == sizeof(SmartHome::HelloAckPayload)) {
                verarbeiteHelloAck(senderMac, *reinterpret_cast<const SmartHome::HelloAckPayload*>(payload));
            }
            break;

        case SH_MSG_CMD:
            if (header->payload_len == sizeof(SmartHome::CmdPayload)) {
                verarbeiteCmd(*reinterpret_cast<const SmartHome::CmdPayload*>(payload));
            }
            break;

        default:
            logf("WARN", "ESP-NOW Nachricht ignoriert (msg_type=%u)", (unsigned)header->msg_type);
            break;
    }
}

#if ESP_ARDUINO_VERSION_MAJOR >= 3
void onEspNowReceive(const esp_now_recv_info_t* info, const uint8_t* daten, int laenge) {
    if (info == nullptr) return;
    verarbeiteEspNowPaket(info->src_addr, daten, laenge);
}
#else
void onEspNowReceive(const uint8_t* senderMac, const uint8_t* daten, int laenge) {
    verarbeiteEspNowPaket(senderMac, daten, laenge);
}
#endif

void onEspNowSent(const uint8_t* mac, esp_now_send_status_t status) {
    char macText[18] = {0};
    if (mac != nullptr) {
        SmartHome::macToString(mac, macText);
    } else {
        copyText(macText, sizeof(macText), "unbekannt");
    }

    logf(
        status == ESP_NOW_SEND_SUCCESS ? "INFO" : "WARN",
        "ESP-NOW Sendestatus an %s: %s",
        macText,
        status == ESP_NOW_SEND_SUCCESS ? "OK" : "FEHLER");
}

void setup() {
    if (DEBUG_LOKAL_AKTIV) {
        Serial.begin(115200);
        delay(150);
    }

    nodeStatus = {};
    logf("INFO", "setup() gestartet");
    gibStartmeldungAus();
    logf("INFO", "Initialisiere Hardware");
    initialisiereHardware();
    logf("INFO", "Initialisiere Funk");
    initialisiereFunk();

    esp_now_register_send_cb(onEspNowSent);
    esp_now_register_recv_cb(onEspNowReceive);

    if (!sendeHello()) {
        logf("WARN", "Erstes HELLO konnte lokal nicht gesendet werden");
    }
}

void loop() {
    unsigned long jetzt = millis();

    messeSensoren();
    aktualisiereMotion();

    if (!nodeStatus.master_bekannt) {
        if ((jetzt - nodeStatus.letztes_hello_ms) >= HELLO_RETRY_INTERVAL_MS) {
            sendeHello();
        }
    } else if ((jetzt - nodeStatus.letzter_heartbeat_ms) >= HEARTBEAT_INTERVAL_MS) {
        sendeHeartbeat();
    }

    if (dhtStateBereit() &&
        !nodeStatus.master_mac_gueltig &&
        nodeStatus.state_report_offen &&
        (jetzt - nodeStatus.letzter_state_wait_log_ms) >= HELLO_RETRY_INTERVAL_MS) {
        nodeStatus.letzter_state_wait_log_ms = jetzt;
        logf("WARN", "STATE_REPORT geparkt: Master-MAC unbekannt");
    }

    if (nodeStatus.master_mac_gueltig &&
        dhtStateBereit() &&
        (nodeStatus.state_report_offen || (jetzt - nodeStatus.letzter_state_ms) >= STATE_INTERVAL_MS)) {
        sendeStateReport();
    }

    delay(LOOP_INTERVAL_MS);
}
