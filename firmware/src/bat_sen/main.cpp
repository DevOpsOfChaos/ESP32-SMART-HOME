/*
====================================================================
 Projekt   : SmartHome ESP32
 Geraet    : BAT-SEN (Batteriebetrieben, Sensor/Event-Basis)
 Datei     : main.cpp
 Version   : 0.2.4
 Stand     : 2026-03-12

 Funktion:
 Allgemeine Batterie-Basis in derselben ESP-NOW-Linie wie die
 netzbetriebenen Basen.
 - feste node_id bat_sen_01
 - HELLO beim Start
 - HEARTBEAT im batterievertraeglichen Intervall
 - STATE_REPORT fuer Batterie- und Eingangszustand
 - EVENT fuer Reed/Taster-Aenderungen

 Hinweis:
 Deep-Sleep wird in diesem Stand bewusst nicht aktiviert. Die Basis
 bleibt bei Build- und Integrationslogik statt ungetesteter Sleep-Show.
====================================================================
*/

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <stdarg.h>
#include <string.h>

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
constexpr char DATEI_GERAET[] = "BAT-SEN";
constexpr char DATEI_VERSION[] = "0.2.4";
const uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct BatteryProfileConfig {
    const char* name;
    uint16_t leer_mv;
    uint16_t voll_mv;
};

struct NodeState {
    bool master_bekannt;
    bool master_mac_gueltig;
    bool fault;
    bool window_offen;
    bool letzter_button_1;
    bool letzter_button_2;
    bool letzter_button_3;
    bool letzter_button_4;
    bool state_report_offen;
    uint8_t batterie_pct;
    uint16_t batterie_adc_mv;
    uint16_t batterie_mv;
    uint16_t rain_raw;
    uint8_t button_flags;
    uint8_t master_mac[6];
    unsigned long letztes_hello_ms;
    unsigned long letzter_heartbeat_ms;
    unsigned long letzter_state_ms;
    uint8_t naechste_seq;
};

NodeState nodeStatus = {};

BatteryProfileConfig holeBatterieProfil() {
    switch (BATTERY_PROFILE) {
        case BAT_PROFILE_COIN_3V_PRIMARY:
            return {"coin_3v_primary", BATTERY_PROFILE_COIN_3V_PRIMARY_RANGE.leer_mv, BATTERY_PROFILE_COIN_3V_PRIMARY_RANGE.voll_mv};

        case BAT_PROFILE_ALKALINE_2X:
            return {"alkaline_2x", BATTERY_PROFILE_ALKALINE_2X_RANGE.leer_mv, BATTERY_PROFILE_ALKALINE_2X_RANGE.voll_mv};

        case BAT_PROFILE_LIION_1S:
            return {"liion_1s", BATTERY_PROFILE_LIION_1S_RANGE.leer_mv, BATTERY_PROFILE_LIION_1S_RANGE.voll_mv};
    }

    return {"coin_3v_primary", BATTERY_PROFILE_COIN_3V_PRIMARY_RANGE.leer_mv, BATTERY_PROFILE_COIN_3V_PRIMARY_RANGE.voll_mv};
}

uint8_t berechneBatterieProzent(uint16_t batterieMv) {
    BatteryProfileConfig profil = holeBatterieProfil();

    if (batterieMv <= profil.leer_mv) return 0U;
    if (batterieMv >= profil.voll_mv) return 100U;

    return (uint8_t)((((uint32_t)batterieMv - profil.leer_mv) * 100UL) / ((uint32_t)profil.voll_mv - profil.leer_mv));
}

uint16_t leseBatterieMillivolt(uint16_t* adcEingangMvOut) {
    // V1 knows only the fixed GPIO4 battery divider: 100k / 100k => ADC sees VBAT / 2.
    // `analogReadMilliVolts()` uses the Arduino/ESP32 calibration path when available,
    // but board-specific ADC accuracy is still not hardware-validated for this device.
    uint32_t adcSummeMv = 0UL;
    for (uint8_t i = 0U; i < BATTERY_ADC_SAMPLE_COUNT; ++i) {
        adcSummeMv += analogReadMilliVolts(PIN_BATTERY_ADC);
    }

    uint32_t adcEingangMv = (adcSummeMv + (BATTERY_ADC_SAMPLE_COUNT / 2U)) / BATTERY_ADC_SAMPLE_COUNT;
    if (adcEingangMvOut != nullptr) {
        *adcEingangMvOut = (adcEingangMv > 65535UL) ? 65535U : (uint16_t)adcEingangMv;
    }
    if (adcEingangMv == 0UL) {
        return 0U;
    }

    // The 100 uF cap on the divider node keeps the input slow but stable, so a short average is enough here.
    uint32_t batterieMv =
        (adcEingangMv * (BATTERY_DIVIDER_TOP_OHM + BATTERY_DIVIDER_BOTTOM_OHM)) / BATTERY_DIVIDER_BOTTOM_OHM;

    if (batterieMv > 65535UL) {
        batterieMv = 65535UL;
    }

    return (uint16_t)batterieMv;
}

void logf(const char* level, const char* format, ...);

void logBatterieMessung(const char* kontext) {
    if (!DEBUG_LOKAL_AKTIV) return;

    BatteryProfileConfig profil = holeBatterieProfil();
    if (nodeStatus.batterie_mv == 0U || nodeStatus.batterie_pct == 0xFFU) {
        logf(
            "INFO",
            "%s profil=%s adc_mv=%u battery_mv=0 battery_pct=unbekannt",
            kontext,
            profil.name,
            (unsigned)nodeStatus.batterie_adc_mv);
        return;
    }

    logf(
        "INFO",
        "%s profil=%s adc_mv=%u battery_mv=%u battery_pct=%u",
        kontext,
        profil.name,
        (unsigned)nodeStatus.batterie_adc_mv,
        (unsigned)nodeStatus.batterie_mv,
        (unsigned)nodeStatus.batterie_pct);
}

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

uint16_t ermittleCaps() {
    uint16_t caps = SH_CAP_BATTERY;

    uint8_t buttonCount = 0U;
    if (PIN_BUTTON_1 >= 0) buttonCount++;
    if (PIN_BUTTON_2 >= 0) buttonCount++;
    if (PIN_BUTTON_3 >= 0) buttonCount++;
    if (PIN_BUTTON_4 >= 0) buttonCount++;

    if (buttonCount == 1U) caps |= SH_CAP_BUTTON;
    if (buttonCount > 1U) caps |= SH_CAP_MULTIBUTTON;
    if (PIN_REED >= 0) caps |= SH_CAP_WINDOW;
    if (PIN_RAIN_ADC >= 0) caps |= SH_CAP_RAIN;
    return caps;
}

bool stellePeerSicher(const uint8_t* mac) {
    if (mac == nullptr || !SmartHome::isValidMac(mac)) return false;
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
    payload.device_class = SH_CLASS_BAT_SEN;

    uint16_t caps = ermittleCaps();
    payload.caps_hi = (uint8_t)((caps >> 8) & 0xFFU);
    payload.caps_lo = (uint8_t)(caps & 0xFFU);
    payload.power_type = SH_POWER_BATTERY;
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

    SmartHome::BatteryStateReportPayload payload = {};
    copyText(payload.node_id, sizeof(payload.node_id), DEVICE_ID);
    payload.battery_pct = nodeStatus.batterie_pct;
    payload.battery_mv = nodeStatus.batterie_mv;
    payload.window_open = (PIN_REED >= 0) ? (nodeStatus.window_offen ? 1U : 0U) : 0xFFU;
    payload.rain_raw = (PIN_RAIN_ADC >= 0) ? nodeStatus.rain_raw : 0xFFFFU;
    payload.button_flags = nodeStatus.button_flags;
    payload.fault = nodeStatus.fault ? 1U : 0U;
    logBatterieMessung("STATE_REPORT Werte");

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
    if (PIN_BUTTON_1 >= 0) pinMode(PIN_BUTTON_1, INPUT_PULLUP);
    if (PIN_BUTTON_2 >= 0) pinMode(PIN_BUTTON_2, INPUT_PULLUP);
    if (PIN_BUTTON_3 >= 0) pinMode(PIN_BUTTON_3, INPUT_PULLUP);
    if (PIN_BUTTON_4 >= 0) pinMode(PIN_BUTTON_4, INPUT_PULLUP);
    if (PIN_REED >= 0) pinMode(PIN_REED, INPUT_PULLUP);
    if (PIN_BATTERY_ADC >= 0) {
        adcAttachPin(PIN_BATTERY_ADC);
        // 11 dB is the safest minimal V1 choice until the exact board attenuation
        // and calibration are proven on real bat_sen hardware.
        analogSetPinAttenuation(PIN_BATTERY_ADC, ADC_11db);
    }

    nodeStatus.batterie_pct = 0xFFU;
    nodeStatus.batterie_adc_mv = 0U;
    nodeStatus.batterie_mv = 0U;
    nodeStatus.rain_raw = 0xFFFFU;
    nodeStatus.button_flags = 0U;
    nodeStatus.window_offen = false;
    nodeStatus.fault = false;
    nodeStatus.state_report_offen = false;
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

    stellePeerSicher(BROADCAST_MAC);
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
    Serial.print("Sleep-Intervall reserviert: ");
    Serial.print(SLEEP_INTERVAL_S);
    Serial.println(" s");
    BatteryProfileConfig profil = holeBatterieProfil();
    Serial.print("Batterieprofil: ");
    Serial.println(profil.name);
    Serial.print("Batterieprofil-Fenster: ");
    Serial.print(profil.leer_mv);
    Serial.print("..");
    Serial.print(profil.voll_mv);
    Serial.println(" mV");
    Serial.println("================================");
}

void messeBatterie() {
    if (PIN_BATTERY_ADC < 0) {
        nodeStatus.batterie_pct = 0xFFU;
        nodeStatus.batterie_adc_mv = 0U;
        nodeStatus.batterie_mv = 0U;
        return;
    }

    uint16_t adcEingangMv = 0U;
    uint16_t batterieMv = leseBatterieMillivolt(&adcEingangMv);
    nodeStatus.batterie_adc_mv = adcEingangMv;
    if (batterieMv == 0U) {
        nodeStatus.batterie_pct = 0xFFU;
        nodeStatus.batterie_mv = 0U;
        return;
    }

    nodeStatus.batterie_mv = batterieMv;
    nodeStatus.batterie_pct = berechneBatterieProzent(batterieMv);
}

uint8_t liesButtonFlags() {
    uint8_t flags = 0U;
    if (PIN_BUTTON_1 >= 0 && digitalRead(PIN_BUTTON_1) == LOW) flags |= 0x01U;
    if (PIN_BUTTON_2 >= 0 && digitalRead(PIN_BUTTON_2) == LOW) flags |= 0x02U;
    if (PIN_BUTTON_3 >= 0 && digitalRead(PIN_BUTTON_3) == LOW) flags |= 0x04U;
    if (PIN_BUTTON_4 >= 0 && digitalRead(PIN_BUTTON_4) == LOW) flags |= 0x08U;
    return flags;
}

void aktualisiereFensterKontakt() {
    if (PIN_REED < 0) return;

    bool jetztOffen = (digitalRead(PIN_REED) == LOW);
    if (jetztOffen != nodeStatus.window_offen) {
        nodeStatus.window_offen = jetztOffen;
        nodeStatus.state_report_offen = true;
        sendeEvent(
            jetztOffen ? SH_EVENT_WINDOW_OPENED : SH_EVENT_WINDOW_CLOSED,
            SH_TRIGGER_UNKNOWN,
            0U,
            0U);
    }
}

void verarbeiteButton(bool jetztGedrueckt, bool* letzterZustand, uint8_t buttonIndex) {
    if (jetztGedrueckt && !*letzterZustand) {
        sendeEvent(SH_EVENT_BUTTON_PRESS, SH_TRIGGER_MANUAL_BUTTON, buttonIndex, 0U);
        nodeStatus.state_report_offen = true;
    }
    *letzterZustand = jetztGedrueckt;
}

void aktualisiereTaster() {
    if (PIN_BUTTON_1 >= 0) verarbeiteButton(digitalRead(PIN_BUTTON_1) == LOW, &nodeStatus.letzter_button_1, 1U);
    if (PIN_BUTTON_2 >= 0) verarbeiteButton(digitalRead(PIN_BUTTON_2) == LOW, &nodeStatus.letzter_button_2, 2U);
    if (PIN_BUTTON_3 >= 0) verarbeiteButton(digitalRead(PIN_BUTTON_3) == LOW, &nodeStatus.letzter_button_3, 3U);
    if (PIN_BUTTON_4 >= 0) verarbeiteButton(digitalRead(PIN_BUTTON_4) == LOW, &nodeStatus.letzter_button_4, 4U);

    nodeStatus.button_flags = liesButtonFlags();
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
    nodeStatus.state_report_offen = true;
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
    gibStartmeldungAus();
    initialisiereHardware();
    initialisiereFunk();
    messeBatterie();
    logBatterieMessung("BOOT Batterie");

    esp_now_register_send_cb(onEspNowSent);
    esp_now_register_recv_cb(onEspNowReceive);

    sendeHello();
}

void loop() {
    unsigned long jetzt = millis();

    messeBatterie();
    aktualisiereFensterKontakt();
    aktualisiereTaster();

    if (!nodeStatus.master_bekannt) {
        if ((jetzt - nodeStatus.letztes_hello_ms) >= HELLO_RETRY_INTERVAL_MS) {
            sendeHello();
        }
    } else if ((jetzt - nodeStatus.letzter_heartbeat_ms) >= HEARTBEAT_INTERVAL_MS) {
        sendeHeartbeat();
    }

    if (nodeStatus.state_report_offen || (jetzt - nodeStatus.letzter_state_ms) >= STATE_INTERVAL_MS) {
        sendeStateReport();
    }

    delay(LOOP_INTERVAL_MS);
}
