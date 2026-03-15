/*
====================================================================
 Projekt   : SmartHome ESP32
 Gerät     : NET-ERL (Netzbetrieben, 1 Relais)
 Datei     : main.cpp
 Version   : 0.2.2
 Stand     : 2026-03-15

 Funktion:
 Erste lauffaehige Vertikalstrecke fuer genau einen Pilot-Node:
 net_erl -> master -> MQTT -> master -> net_erl

 Dieser Stand implementiert bewusst nur:
 - feste node_id net_erl_01
 - HELLO beim Start
 - HEARTBEAT zyklisch
 - STATE_REPORT beim Start und nach Relaisaenderung
 - COMMAND_SET_RELAY vom Master
 - lokaler manueller Tasterpfad
 - optionale lokale PIR-Automatik unter Servervorrang
 - Relaisstatus- und Eventmeldungen mit Trigger-Trennung
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
#include "../../lib/ShSensors/src/DigitalInputSupport.h"
#include <ShNodeProvisioning.h>

constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;
constexpr char DATEI_GERAET[] = "NET-ERL";
constexpr char DATEI_VERSION[] = "0.2.2";
const uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
SmartHome::ShNodeBase::NodeProvisioning provisioning;

struct NodeState {
    bool master_bekannt;
    bool master_mac_gueltig;
    bool auto_blockiert_bis_idle;
    bool relay_1_auto_aktiv;
    bool relay_1;
    bool fault;
    bool state_report_offen;
    bool letzter_cmd_gueltig;
    unsigned long auto_off_faellig_ms;
    unsigned long letztes_hello_ms;
    unsigned long letzter_heartbeat_ms;
    uint8_t master_mac[6];
    uint8_t naechste_seq;
    uint8_t letzter_cmd_seq;
    SmartHome::CmdPayload letzter_cmd_payload;
};

NodeState nodeStatus = {};
SmartHome::ShSensors::DigitalInputChannel lokalerTaster(
    {PIN_BUTTON_1, true, true, INPUT_EVENT_DEBOUNCE_MS, LOCAL_BUTTON_LONGPRESS_MS});
SmartHome::ShSensors::DigitalInputChannel pirEingang(
    {PIN_PIR, false, false, PIR_EVENT_DEBOUNCE_MS, 0UL});

bool istBroadcastMac(const uint8_t* mac) {
    return mac != nullptr && memcmp(mac, BROADCAST_MAC, sizeof(BROADCAST_MAC)) == 0;
}

const uint8_t* holeHelloZielMac() {
    return nodeStatus.master_mac_gueltig ? nodeStatus.master_mac : BROADCAST_MAC;
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

void logMac(const char* prefix, const uint8_t* mac) {
    char macText[18] = {0};
    SmartHome::macToString(mac, macText);
    logf("INFO", "%s%s", prefix, macText);
}

void logProvisioning(const char* level, const char* message) {
    logf(level, "%s", message);
}

void copyText(char* target, size_t targetSize, const char* source) {
    if (!target || targetSize == 0) return;
    if (!source) {
        target[0] = '\0';
        return;
    }

    strncpy(target, source, targetSize - 1U);
    target[targetSize - 1U] = '\0';
}

bool gleicheNodeId(const char* nodeId) {
    return nodeId != nullptr && strncmp(nodeId, DEVICE_ID, SH_DEVICE_ID_LEN) == 0;
}

void uebernehmeProvisionierteMasterMac() {
    if (!provisioning.hasStoredMasterMac()) {
        nodeStatus.master_bekannt = false;
        nodeStatus.master_mac_gueltig = false;
        memset(nodeStatus.master_mac, 0, sizeof(nodeStatus.master_mac));
        return;
    }

    memcpy(nodeStatus.master_mac, provisioning.masterMac(), sizeof(nodeStatus.master_mac));
    nodeStatus.master_bekannt = false;
    nodeStatus.master_mac_gueltig = true;
    nodeStatus.state_report_offen = true;
    logMac("Persistierte Master-MAC: ", nodeStatus.master_mac);
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
    if (zielMac == nullptr) return false;
    if (payloadLen > SH_MAX_PAYLOAD_BYTES) return false;
    if (!stellePeerSicher(zielMac)) return false;

    uint8_t buffer[SH_ESPNOW_MAX_BYTES] = {0};
    SmartHome::MsgHeader header = {};
    SmartHome::fillHeader(header, msgType, nodeStatus.naechste_seq++, 0, (uint16_t)payloadLen);

    uint8_t* payloadBuffer = buffer + SH_HEADER_SIZE;
    if (payloadLen > 0 && payload != nullptr) {
        memcpy(payloadBuffer, payload, payloadLen);
    }

    SmartHome::finalizePacketCrc(header, payloadBuffer);
    memcpy(buffer, &header, sizeof(header));

    esp_err_t err = esp_now_send(zielMac, buffer, SH_HEADER_SIZE + payloadLen);
    if (err != ESP_OK) {
        logf("WARN", "%s konnte nicht gesendet werden (err=%d)", label, (int)err);
        return false;
    }

    char macText[18] = {0};
    SmartHome::macToString(zielMac, macText);
    logf("INFO", "%s gesendet an %s", label, macText);
    return true;
}

bool sendeHello() {
    SmartHome::HelloPayload payload = {};
    copyText(payload.device_id, sizeof(payload.device_id), DEVICE_ID);
    copyText(payload.device_name, sizeof(payload.device_name), provisioning.deviceName());
    payload.device_class = SH_CLASS_NET_ERL;
    payload.caps_hi = (uint8_t)((SH_CAP_RELAY >> 8) & 0xFFU);
    payload.caps_lo = (uint8_t)(SH_CAP_RELAY & 0xFFU);
    payload.power_type = SH_POWER_MAINS;
    payload.fw_version = 0;
    payload.boot_counter = 1U;

    nodeStatus.letztes_hello_ms = millis();
    return sendePaket(holeHelloZielMac(), SH_MSG_HELLO, &payload, sizeof(payload), "HELLO");
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

    SmartHome::StateReportPayload payload = {};
    copyText(payload.node_id, sizeof(payload.node_id), DEVICE_ID);
    payload.relay_1 = nodeStatus.relay_1 ? 1U : 0U;
    payload.fault = nodeStatus.fault ? 1U : 0U;

    if (sendePaket(nodeStatus.master_mac, SH_MSG_STATE, &payload, sizeof(payload), "STATE_REPORT")) {
        nodeStatus.state_report_offen = false;
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

bool sendeAck(const uint8_t* zielMac, uint8_t ackSeq, uint8_t ackMsgType, uint8_t status) {
    SmartHome::AckPayload payload = {};
    payload.ack_seq = ackSeq;
    payload.ack_msg_type = ackMsgType;
    payload.status = status;
    return sendePaket(zielMac, SH_MSG_ACK, &payload, sizeof(payload), "ACK");
}

bool istDoppeltesCmd(uint8_t seq, const SmartHome::CmdPayload& cmd) {
    return nodeStatus.letzter_cmd_gueltig &&
           nodeStatus.letzter_cmd_seq == seq &&
           memcmp(&nodeStatus.letzter_cmd_payload, &cmd, sizeof(cmd)) == 0;
}

bool zeitpunktErreicht(unsigned long jetzt, unsigned long zielMs) {
    return (long)(jetzt - zielMs) >= 0L;
}

bool manualAllowed() {
    return PIN_BUTTON_1 >= 0;
}

bool autoAllowed() {
    return PIN_PIR >= 0 && !nodeStatus.auto_blockiert_bis_idle;
}

bool relayCommandAllowed(uint8_t trigger) {
    switch (trigger) {
        case SH_TRIGGER_MASTER_CMD:
            return true;
        case SH_TRIGGER_MANUAL_BUTTON:
            return manualAllowed();
        case SH_TRIGGER_AUTO:
        case SH_TRIGGER_AUTO_OFF_TIMER:
            return autoAllowed();
        default:
            return false;
    }
}

uint8_t relayEventType(uint8_t trigger, bool einschalten) {
    if (trigger == SH_TRIGGER_AUTO || trigger == SH_TRIGGER_AUTO_OFF_TIMER) {
        return einschalten ? SH_EVENT_LIGHT_AUTO_ON : SH_EVENT_LIGHT_AUTO_OFF;
    }
    return SH_EVENT_RELAY_CHANGED;
}

void setRelayOutput(bool einschalten) {
    if (PIN_RELAY_1 >= 0) {
        digitalWrite(PIN_RELAY_1, einschalten ? HIGH : LOW);
    }
}

void markiereServerVorrang() {
    nodeStatus.auto_blockiert_bis_idle = true;
    nodeStatus.relay_1_auto_aktiv = false;
    nodeStatus.auto_off_faellig_ms = 0UL;
}

void aktualisiereAutomatikFreigabe(bool pirAktiv) {
    if (nodeStatus.auto_blockiert_bis_idle && !pirAktiv) {
        nodeStatus.auto_blockiert_bis_idle = false;
        logf("INFO", "Lokale Automatik wieder freigegeben: PIR idle");
    }
}

bool schalteRelais(bool einschalten, uint8_t trigger, const char* quelle) {
    if (!relayCommandAllowed(trigger)) {
        logf("WARN", "Relaiskommando blockiert (%s, trigger=%u)", quelle, (unsigned)trigger);
        return false;
    }

    const bool geaendert = (nodeStatus.relay_1 != einschalten);
    nodeStatus.relay_1 = einschalten;
    setRelayOutput(einschalten);

    if (trigger == SH_TRIGGER_MASTER_CMD) {
        markiereServerVorrang();
    } else if (trigger == SH_TRIGGER_MANUAL_BUTTON) {
        nodeStatus.auto_blockiert_bis_idle = false;
        nodeStatus.relay_1_auto_aktiv = false;
        nodeStatus.auto_off_faellig_ms = 0UL;
    } else if (trigger == SH_TRIGGER_AUTO && einschalten) {
        if (geaendert || nodeStatus.relay_1_auto_aktiv) {
            nodeStatus.relay_1_auto_aktiv = true;
            if (LOCAL_AUTO_OFF_DELAY_MS > 0UL) {
                nodeStatus.auto_off_faellig_ms = millis() + LOCAL_AUTO_OFF_DELAY_MS;
            }
        }
    } else if (trigger == SH_TRIGGER_AUTO_OFF_TIMER) {
        nodeStatus.relay_1_auto_aktiv = false;
        nodeStatus.auto_off_faellig_ms = 0UL;
    }

    if (!geaendert) {
        logf("INFO", "Relaiszustand war bereits %s", einschalten ? "EIN" : "AUS");
    }

    if (PIN_RELAY_1 >= 0) {
        logf("INFO", "Relais physisch %s auf GPIO%d (%s)", einschalten ? "EIN" : "AUS", PIN_RELAY_1, quelle);
    } else {
        logf("INFO", "Relais logisch %s (%s, kein Pin gesetzt)", einschalten ? "EIN" : "AUS", quelle);
    }

    if (geaendert) {
        sendeEvent(relayEventType(trigger, einschalten), trigger, 0U, einschalten ? 1U : 0U);
    }

    nodeStatus.state_report_offen = true;
    return true;
}

void verarbeiteLokalenTaster(unsigned long jetzt) {
    if (!manualAllowed()) {
        return;
    }

    const SmartHome::ShSensors::DigitalInputUpdate update = lokalerTaster.poll(jetzt);
    if (update.activated()) {
        sendeEvent(SH_EVENT_BUTTON_PRESS, SH_TRIGGER_MANUAL_BUTTON, 1U, 0U);
        schalteRelais(!nodeStatus.relay_1, SH_TRIGGER_MANUAL_BUTTON, "Lokaler Taster");
    }
    if (update.longHold()) {
        sendeEvent(SH_EVENT_BUTTON_LONG_PRESS, SH_TRIGGER_MANUAL_BUTTON, 1U, update.active_ms);
    }
    if (update.deactivated()) {
        sendeEvent(SH_EVENT_BUTTON_RELEASE, SH_TRIGGER_MANUAL_BUTTON, 1U, update.active_ms);
    }
}

void aktualisiereLokaleAutomatik(unsigned long jetzt) {
    if (PIN_PIR < 0) {
        return;
    }

    const SmartHome::ShSensors::DigitalInputUpdate update = pirEingang.poll(jetzt);
    aktualisiereAutomatikFreigabe(update.active);

    if (update.activated()) {
        sendeEvent(SH_EVENT_MOTION_DETECTED, SH_TRIGGER_AUTO, 0U, 0U);
        if (!autoAllowed()) {
            logf("INFO", "PIR erkannt, aber lokale Automatik bleibt wegen Servervorrang geparkt");
            return;
        }

        schalteRelais(true, SH_TRIGGER_AUTO, "Lokale PIR-Automatik");
    }

    if (update.active && nodeStatus.relay_1_auto_aktiv && LOCAL_AUTO_OFF_DELAY_MS > 0UL) {
        nodeStatus.auto_off_faellig_ms = jetzt + LOCAL_AUTO_OFF_DELAY_MS;
    }

    if (!update.active &&
        nodeStatus.relay_1_auto_aktiv &&
        nodeStatus.auto_off_faellig_ms != 0UL &&
        zeitpunktErreicht(jetzt, nodeStatus.auto_off_faellig_ms)) {
        schalteRelais(false, SH_TRIGGER_AUTO_OFF_TIMER, "Lokaler Auto-Off");
    }
}

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) {
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, LOW);
    }

    if (PIN_RELAY_1 >= 0) {
        pinMode(PIN_RELAY_1, OUTPUT);
        digitalWrite(PIN_RELAY_1, RELAY_DEFAULT_ON_BOOT ? HIGH : LOW);
    }

    nodeStatus.relay_1 = RELAY_DEFAULT_ON_BOOT;
    nodeStatus.auto_blockiert_bis_idle = false;
    nodeStatus.relay_1_auto_aktiv = false;
    nodeStatus.auto_off_faellig_ms = 0UL;
    nodeStatus.fault = false;
    nodeStatus.state_report_offen = false;

    lokalerTaster.begin();
    pirEingang.begin();
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
    Serial.print("Name: ");
    Serial.println(provisioning.deviceName());
    Serial.print("FW: ");
    Serial.println(PROJECT_VERSION);
    Serial.print("Relay1 GPIO: ");
    Serial.println(PIN_RELAY_1);
    Serial.print("I2C: SDA=GPIO");
    Serial.print(PIN_SENSOR_SDA);
    Serial.print(" SCL=GPIO");
    Serial.println(PIN_SENSOR_SCL);
    Serial.print("NeoPixel GPIO: ");
    Serial.println(PIN_INTERNAL_NEOPIXEL);
    Serial.println("Minimalstrecke: HELLO -> HEARTBEAT -> STATE_REPORT");
    Serial.println("================================");
}

void verarbeiteHelloAck(const uint8_t* senderMac, const SmartHome::HelloAckPayload& payload) {
    if (payload.ack_status != SH_ACK_OK) {
        logf("WARN", "HELLO_ACK abgelehnt");
        return;
    }

    memcpy(nodeStatus.master_mac, senderMac, 6);
    nodeStatus.master_mac_gueltig = true;
    nodeStatus.master_bekannt = true;
    provisioning.persistMasterMac(senderMac);
    stellePeerSicher(nodeStatus.master_mac);
    logf("INFO", "HELLO_ACK empfangen, Master-Kanal=%u", payload.channel);
    logMac("Master-MAC: ", senderMac);
    nodeStatus.state_report_offen = true;
}

void verarbeiteCmd(const uint8_t* senderMac, const SmartHome::MsgHeader& header, const SmartHome::CmdPayload& cmd) {
    if (istDoppeltesCmd(header.seq, cmd)) {
        logf("WARN", "Duplikat-CMD erkannt (seq=%u)", (unsigned)header.seq);
        if (header.flags & SH_FLAG_ACK_REQUEST) {
            sendeAck(senderMac, header.seq, header.msg_type, SH_ACK_OK);
        }
        return;
    }

    if (cmd.cmd_type != SH_CMD_SET_RELAY || cmd.param1 != 0U) {
        logf("WARN", "Unbekanntes CMD empfangen (cmd=%u, param1=%u)", cmd.cmd_type, cmd.param1);
        if (header.flags & SH_FLAG_ACK_REQUEST) {
            sendeAck(senderMac, header.seq, header.msg_type, SH_ACK_ERROR);
        }
        return;
    }

    bool einschalten = false;
    if (cmd.param2 == 2U) {
        einschalten = !nodeStatus.relay_1;
    } else {
        einschalten = (cmd.param2 == 1U);
    }

    logf(
        "INFO",
        "COMMAND_SET_RELAY empfangen: relay_1=%s%s",
        einschalten ? "true" : "false",
        (header.flags & SH_FLAG_RETRANSMIT) ? " (retry)" : "");
    schalteRelais(einschalten, SH_TRIGGER_MASTER_CMD, "Master-Kommando");

    nodeStatus.letzter_cmd_gueltig = true;
    nodeStatus.letzter_cmd_seq = header.seq;
    nodeStatus.letzter_cmd_payload = cmd;

    if (header.flags & SH_FLAG_ACK_REQUEST) {
        sendeAck(senderMac, header.seq, header.msg_type, SH_ACK_OK);
    }
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
                verarbeiteCmd(senderMac, *header, *reinterpret_cast<const SmartHome::CmdPayload*>(payload));
            }
            break;

        default:
            logf("WARN", "ESP-NOW Nachricht ignoriert (msg_type=%u)", header->msg_type);
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
    initialisiereHardware();
    provisioning.begin(
        {
            DEVICE_ID,
            DEVICE_NAME,
            HEARTBEAT_INTERVAL_MS / 1000UL,
            HEARTBEAT_INTERVAL_MS / 1000UL,
            false,
            false,
            WLAN_KANAL,
            PIN_BOOT_BUTTON,
            true,
            SETUP_REENTRY_HOLD_MS,
            FACTORY_RESET_HOLD_MS,
        },
        logProvisioning);
    uebernehmeProvisionierteMasterMac();
    gibStartmeldungAus();

    if (provisioning.isSetupModeActive()) {
        logf("WARN", "Setup-Modus aktiv. Funkpfad wartet auf lokale Provisionierung.");
        return;
    }

    initialisiereFunk();

    esp_now_register_send_cb(onEspNowSent);
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    esp_now_register_recv_cb(onEspNowReceive);
#else
    esp_now_register_recv_cb(onEspNowReceive);
#endif

    sendeHello();
}

void loop() {
    provisioning.update();
    if (provisioning.isSetupModeActive()) {
        delay(LOOP_INTERVAL_MS);
        return;
    }

    unsigned long jetzt = millis();
    verarbeiteLokalenTaster(jetzt);
    aktualisiereLokaleAutomatik(jetzt);

    if (!nodeStatus.master_bekannt) {
        if ((jetzt - nodeStatus.letztes_hello_ms) >= HELLO_RETRY_INTERVAL_MS) {
            sendeHello();
        }
    } else if ((jetzt - nodeStatus.letzter_heartbeat_ms) >= HEARTBEAT_INTERVAL_MS) {
        sendeHeartbeat();
    }

    if (nodeStatus.state_report_offen) {
        sendeStateReport();
    }

    delay(LOOP_INTERVAL_MS);
}
