/*
====================================================================
 Projekt   : SmartHome ESP32
 Geraet    : NET-ZRL (Netzbetrieben, 2 Relais)
 Datei     : main.cpp
 Version   : 0.2.2
 Stand     : 2026-03-15

 Funktion:
 Allgemeine Zwei-Relais-Basis in derselben ESP-NOW-Linie wie NET-ERL.
 - feste node_id net_zrl_01
 - HELLO beim Start
 - HEARTBEAT zyklisch
 - STATE_REPORT beim Start, periodisch und nach Relaisaenderung
 - COMMAND_SET_RELAY fuer Relais 1/2
 - lokaler manueller Tasterpfad fuer Relais-/Cover-Bedienung
 - Eventmeldungen fuer lokale und serverseitige Schaltgrunde
 - Cover-Schutzlogik nur bei aktivem COVER_MODUS_AKTIV
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
constexpr char DATEI_GERAET[] = "NET-ZRL";
constexpr char DATEI_VERSION[] = "0.2.2";
const uint8_t BROADCAST_MAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
SmartHome::ShNodeBase::NodeProvisioning provisioning;

struct NodeState {
    bool master_bekannt;
    bool master_mac_gueltig;
    bool relay_1;
    bool relay_2;
    bool fault;
    bool state_report_offen;
    bool relais_verriegelt;
    bool letzter_cmd_gueltig;
    uint8_t cover_state;
    uint8_t cover_position;
    unsigned long verriegelung_bis_ms;
    unsigned long letztes_hello_ms;
    unsigned long letzter_heartbeat_ms;
    unsigned long letzter_state_ms;
    uint8_t master_mac[6];
    uint8_t naechste_seq;
    uint8_t letzter_cmd_seq;
    SmartHome::CmdPayload letzter_cmd_payload;
};

NodeState nodeStatus = {};
SmartHome::ShSensors::DigitalInputChannel lokalerTaster1(
    {PIN_BUTTON_1, true, true, INPUT_EVENT_DEBOUNCE_MS, LOCAL_BUTTON_LONGPRESS_MS});
SmartHome::ShSensors::DigitalInputChannel lokalerTaster2(
    {PIN_BUTTON_2, true, true, INPUT_EVENT_DEBOUNCE_MS, LOCAL_BUTTON_LONGPRESS_MS});
SmartHome::ShSensors::DigitalInputChannel lokalerTaster3(
    {PIN_BUTTON_3, true, true, INPUT_EVENT_DEBOUNCE_MS, LOCAL_BUTTON_LONGPRESS_MS});

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

void logProvisioning(const char* level, const char* message) {
    logf(level, "%s", message);
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

    logMac("Gesendet an: ", zielMac);
    logf("INFO", "%s erfolgreich", label);
    return true;
}

bool sendeAck(const uint8_t* zielMac, uint8_t ackSeq, uint8_t ackMsgType, uint8_t status) {
    SmartHome::AckPayload payload = {};
    payload.ack_seq = ackSeq;
    payload.ack_msg_type = ackMsgType;
    payload.status = status;
    return sendePaket(zielMac, SH_MSG_ACK, &payload, sizeof(payload), "ACK");
}

bool sendeHello() {
    SmartHome::HelloPayload payload = {};
    copyText(payload.device_id, sizeof(payload.device_id), DEVICE_ID);
    copyText(payload.device_name, sizeof(payload.device_name), provisioning.deviceName());
    payload.device_class = SH_CLASS_NET_ZRL;

    uint16_t caps = SH_CAP_RELAY | SH_CAP_RELAY2;
    if (COVER_MODUS_AKTIV) caps |= SH_CAP_COVER;
    payload.caps_hi = (uint8_t)((caps >> 8) & 0xFFU);
    payload.caps_lo = (uint8_t)(caps & 0xFFU);
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

    SmartHome::ZrlStateReportPayload payload = {};
    copyText(payload.node_id, sizeof(payload.node_id), DEVICE_ID);
    payload.relay_1 = nodeStatus.relay_1 ? 1U : 0U;
    payload.relay_2 = nodeStatus.relay_2 ? 1U : 0U;
    payload.cover_mode = COVER_MODUS_AKTIV ? 1U : 0U;
    payload.cover_state = nodeStatus.cover_state;
    payload.cover_position = nodeStatus.cover_position;
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

bool istDoppeltesCmd(uint8_t seq, const SmartHome::CmdPayload& cmd) {
    return nodeStatus.letzter_cmd_gueltig &&
           nodeStatus.letzter_cmd_seq == seq &&
           memcmp(&nodeStatus.letzter_cmd_payload, &cmd, sizeof(cmd)) == 0;
}

void schreibeRelaisPin(uint8_t relayIndex, bool einschalten) {
    if (relayIndex == 0U && PIN_RELAY_1 >= 0) {
        digitalWrite(PIN_RELAY_1, einschalten ? HIGH : LOW);
    }
    if (relayIndex == 1U && PIN_RELAY_2 >= 0) {
        digitalWrite(PIN_RELAY_2, einschalten ? HIGH : LOW);
    }
}

void aktualisiereCoverState() {
    if (!COVER_MODUS_AKTIV) {
        nodeStatus.cover_state = SH_COVER_STATE_STOPPED;
        nodeStatus.cover_position = 0xFFU;
        return;
    }

    if (nodeStatus.relay_1 && !nodeStatus.relay_2) {
        nodeStatus.cover_state = SH_COVER_STATE_MOVING_UP;
    } else if (nodeStatus.relay_2 && !nodeStatus.relay_1) {
        nodeStatus.cover_state = SH_COVER_STATE_MOVING_DOWN;
    } else {
        nodeStatus.cover_state = SH_COVER_STATE_STOPPED;
    }
}

bool manualAllowed(uint8_t buttonIndex) {
    switch (buttonIndex) {
        case 1U:
            return PIN_BUTTON_1 >= 0;
        case 2U:
            return PIN_BUTTON_2 >= 0;
        case 3U:
            return PIN_BUTTON_3 >= 0;
        default:
            return false;
    }
}

bool relayCommandAllowed(uint8_t relayIndex, uint8_t trigger) {
    if (relayIndex > 1U) return false;

    switch (trigger) {
        case SH_TRIGGER_MASTER_CMD:
            return true;
        case SH_TRIGGER_MANUAL_BUTTON:
            return manualAllowed((uint8_t)(relayIndex + 1U));
        default:
            return false;
    }
}

uint8_t relayEventType(uint8_t trigger, uint8_t relayIndex, bool einschalten) {
    if (COVER_MODUS_AKTIV) {
        if (!einschalten) {
            return SH_EVENT_COVER_STOP;
        }
        return relayIndex == 0U ? SH_EVENT_COVER_UP : SH_EVENT_COVER_DOWN;
    }
    (void)trigger;
    return SH_EVENT_RELAY_CHANGED;
}

bool schalteRelaisGesichert(uint8_t relayIndex, bool einschalten, uint8_t trigger, const char* quelle) {
    if (!relayCommandAllowed(relayIndex, trigger)) {
        logf("WARN", "Relais %u blockiert: unzulaessiger Trigger %u (%s)", (unsigned)(relayIndex + 1U), (unsigned)trigger, quelle);
        return false;
    }

    const bool warBereitsAn = (relayIndex == 0U) ? nodeStatus.relay_1 : nodeStatus.relay_2;

    if (COVER_MODUS_AKTIV && einschalten) {
        if ((relayIndex == 0U && nodeStatus.relay_2) || (relayIndex == 1U && nodeStatus.relay_1)) {
            logf("WARN", "Relais %u blockiert: Gegenrichtung aktiv (%s)", (unsigned)(relayIndex + 1U), quelle);
            return false;
        }
        if (nodeStatus.relais_verriegelt && millis() < nodeStatus.verriegelung_bis_ms) {
            logf("WARN", "Relais %u blockiert: Reversiersperrzeit aktiv", (unsigned)(relayIndex + 1U));
            return false;
        }
    }

    if (relayIndex == 0U) {
        nodeStatus.relay_1 = einschalten;
    } else {
        nodeStatus.relay_2 = einschalten;
    }

    schreibeRelaisPin(relayIndex, einschalten);
    aktualisiereCoverState();
    nodeStatus.state_report_offen = true;
    logf("INFO", "Relais %u -> %s (%s)", (unsigned)(relayIndex + 1U), einschalten ? "EIN" : "AUS", quelle);
    if (warBereitsAn != einschalten) {
        sendeEvent(relayEventType(trigger, relayIndex, einschalten), trigger, relayIndex, einschalten ? 1U : 0U);
    }
    return true;
}

bool stoppeFahrt(const char* quelle, uint8_t trigger) {
    const bool warAktiv = nodeStatus.relay_1 || nodeStatus.relay_2;

    nodeStatus.relay_1 = false;
    nodeStatus.relay_2 = false;
    schreibeRelaisPin(0U, false);
    schreibeRelaisPin(1U, false);
    aktualisiereCoverState();

    if (COVER_MODUS_AKTIV && warAktiv) {
        nodeStatus.relais_verriegelt = true;
        nodeStatus.verriegelung_bis_ms = millis() + COVER_REVERSE_LOCK_MS;
    }

    nodeStatus.state_report_offen = true;
    logf("INFO", "Cover-Stopp (%s)", quelle);
    if (warAktiv) {
        sendeEvent(SH_EVENT_COVER_STOP, trigger, 0U, 0U);
    }
    return true;
}

void aktualisiereRelaisVerriegelung() {
    if (!COVER_MODUS_AKTIV) {
        nodeStatus.relais_verriegelt = false;
        return;
    }

    if (nodeStatus.relais_verriegelt && millis() >= nodeStatus.verriegelung_bis_ms) {
        nodeStatus.relais_verriegelt = false;
    }
}

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) {
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, LOW);
    }

    if (PIN_RELAY_1 >= 0) {
        pinMode(PIN_RELAY_1, OUTPUT);
        digitalWrite(PIN_RELAY_1, RELAY_1_DEFAULT_ON_BOOT ? HIGH : LOW);
    }
    if (PIN_RELAY_2 >= 0) {
        pinMode(PIN_RELAY_2, OUTPUT);
        digitalWrite(PIN_RELAY_2, RELAY_2_DEFAULT_ON_BOOT ? HIGH : LOW);
    }

    nodeStatus.relay_1 = RELAY_1_DEFAULT_ON_BOOT;
    nodeStatus.relay_2 = RELAY_2_DEFAULT_ON_BOOT;
    nodeStatus.cover_state = SH_COVER_STATE_STOPPED;
    nodeStatus.cover_position = 0xFFU;
    nodeStatus.fault = false;
    nodeStatus.state_report_offen = false;

    lokalerTaster1.begin();
    lokalerTaster2.begin();
    lokalerTaster3.begin();
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
    Serial.print("Cover-Modus: ");
    Serial.println(COVER_MODUS_AKTIV ? "aktiv" : "inaktiv");
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

void verarbeiteLokalenButtonEvent(
    uint8_t buttonIndex,
    const SmartHome::ShSensors::DigitalInputUpdate& update,
    const char* pressLabel,
    const char* stopLabel)
{
    if (!manualAllowed(buttonIndex)) {
        return;
    }

    if (update.activated()) {
        sendeEvent(SH_EVENT_BUTTON_PRESS, SH_TRIGGER_MANUAL_BUTTON, buttonIndex, 0U);

        if (COVER_MODUS_AKTIV) {
            if (buttonIndex == 1U) {
                if (nodeStatus.relay_1 && !nodeStatus.relay_2) {
                    stoppeFahrt(stopLabel, SH_TRIGGER_MANUAL_BUTTON);
                } else {
                    stoppeFahrt(stopLabel, SH_TRIGGER_MANUAL_BUTTON);
                    schalteRelaisGesichert(0U, true, SH_TRIGGER_MANUAL_BUTTON, pressLabel);
                }
            } else if (buttonIndex == 2U) {
                if (nodeStatus.relay_2 && !nodeStatus.relay_1) {
                    stoppeFahrt(stopLabel, SH_TRIGGER_MANUAL_BUTTON);
                } else {
                    stoppeFahrt(stopLabel, SH_TRIGGER_MANUAL_BUTTON);
                    schalteRelaisGesichert(1U, true, SH_TRIGGER_MANUAL_BUTTON, pressLabel);
                }
            } else if (buttonIndex == 3U) {
                stoppeFahrt(stopLabel, SH_TRIGGER_MANUAL_BUTTON);
            }
        } else {
            if (buttonIndex == 1U) {
                schalteRelaisGesichert(0U, !nodeStatus.relay_1, SH_TRIGGER_MANUAL_BUTTON, pressLabel);
            } else if (buttonIndex == 2U) {
                schalteRelaisGesichert(1U, !nodeStatus.relay_2, SH_TRIGGER_MANUAL_BUTTON, pressLabel);
            } else {
                logf("INFO", "Taster 3 ohne Funktion im allgemeinen Zwei-Relais-Modus");
            }
        }
    }

    if (update.longHold()) {
        sendeEvent(SH_EVENT_BUTTON_LONG_PRESS, SH_TRIGGER_MANUAL_BUTTON, buttonIndex, update.active_ms);
    }
    if (update.deactivated()) {
        sendeEvent(SH_EVENT_BUTTON_RELEASE, SH_TRIGGER_MANUAL_BUTTON, buttonIndex, update.active_ms);
    }
}

void verarbeiteLokaleTaster(unsigned long jetzt) {
    verarbeiteLokalenButtonEvent(1U, lokalerTaster1.poll(jetzt), "Lokaler Taster 1", "Lokaler Stop/Reset 1");
    verarbeiteLokalenButtonEvent(2U, lokalerTaster2.poll(jetzt), "Lokaler Taster 2", "Lokaler Stop/Reset 2");
    verarbeiteLokalenButtonEvent(3U, lokalerTaster3.poll(jetzt), "Lokaler Stop-Taster", "Lokaler Stop-Taster");
}

bool verarbeiteRelayCmd(const SmartHome::CmdPayload& cmd) {
    if (cmd.param1 > 1U) {
        logf("WARN", "Relay-CMD ignoriert: relay_index=%u", (unsigned)cmd.param1);
        return false;
    }

    bool einschalten = (cmd.param2 == 2U)
        ? (cmd.param1 == 0U ? !nodeStatus.relay_1 : !nodeStatus.relay_2)
        : (cmd.param2 == 1U);

    return schalteRelaisGesichert(cmd.param1, einschalten, SH_TRIGGER_MASTER_CMD, "Master-Kommando");
}

bool verarbeiteCoverCmd(const SmartHome::CmdPayload& cmd) {
    if (!COVER_MODUS_AKTIV) {
        logf("WARN", "Cover-CMD ignoriert: COVER_MODUS_AKTIV=false");
        return false;
    }

    switch (cmd.param1) {
        case SH_COVER_STATE_STOPPED:
            return stoppeFahrt("Cover-Stop", SH_TRIGGER_MASTER_CMD);
        case SH_COVER_STATE_MOVING_UP:
            stoppeFahrt("Cover-Auf", SH_TRIGGER_MASTER_CMD);
            return schalteRelaisGesichert(0U, true, SH_TRIGGER_MASTER_CMD, "Cover-Auf");
        case SH_COVER_STATE_MOVING_DOWN:
            stoppeFahrt("Cover-Ab", SH_TRIGGER_MASTER_CMD);
            return schalteRelaisGesichert(1U, true, SH_TRIGGER_MASTER_CMD, "Cover-Ab");
        default:
            logf("WARN", "Cover-CMD ignoriert: param1=%u", (unsigned)cmd.param1);
            return false;
    }
}

void verarbeiteCmd(const uint8_t* senderMac, const SmartHome::MsgHeader& header, const SmartHome::CmdPayload& cmd) {
    if (istDoppeltesCmd(header.seq, cmd)) {
        logf("WARN", "Duplikat-CMD erkannt (seq=%u)", (unsigned)header.seq);
        if (header.flags & SH_FLAG_ACK_REQUEST) {
            sendeAck(senderMac, header.seq, header.msg_type, SH_ACK_OK);
        }
        return;
    }

    bool erfolgreich = false;
    switch (cmd.cmd_type) {
        case SH_CMD_SET_RELAY:
            logf(
                "INFO",
                "COMMAND_SET_RELAY empfangen: relay_%u=%s%s",
                (unsigned)(cmd.param1 + 1U),
                (cmd.param2 == 2U)
                    ? "toggle"
                    : ((cmd.param2 == 1U) ? "true" : "false"),
                (header.flags & SH_FLAG_RETRANSMIT) ? " (retry)" : "");
            erfolgreich = verarbeiteRelayCmd(cmd);
            break;
        case SH_CMD_COVER:
            logf(
                "INFO",
                "COMMAND_COVER empfangen: state=%u%s",
                (unsigned)cmd.param1,
                (header.flags & SH_FLAG_RETRANSMIT) ? " (retry)" : "");
            erfolgreich = verarbeiteCoverCmd(cmd);
            break;
        case SH_CMD_STATE_REQUEST:
            nodeStatus.state_report_offen = true;
            erfolgreich = true;
            break;
        default:
            logf("WARN", "Unbekanntes CMD empfangen (cmd=%u)", (unsigned)cmd.cmd_type);
            erfolgreich = false;
            break;
    }

    if (!erfolgreich) {
        if (header.flags & SH_FLAG_ACK_REQUEST) {
            sendeAck(senderMac, header.seq, header.msg_type, SH_ACK_ERROR);
        }
        return;
    }

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
    initialisiereHardware();
    provisioning.begin(
        {
            DEVICE_ID,
            DEVICE_NAME,
            STATE_INTERVAL_MS / 1000UL,
            STATE_INTERVAL_MS / 1000UL,
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
    esp_now_register_recv_cb(onEspNowReceive);

    sendeHello();
}

void loop() {
    provisioning.update();
    if (provisioning.isSetupModeActive()) {
        delay(LOOP_INTERVAL_MS);
        return;
    }

    unsigned long jetzt = millis();
    verarbeiteLokaleTaster(jetzt);

    if (!nodeStatus.master_bekannt) {
        if ((jetzt - nodeStatus.letztes_hello_ms) >= HELLO_RETRY_INTERVAL_MS) {
            sendeHello();
        }
    } else if ((jetzt - nodeStatus.letzter_heartbeat_ms) >= HEARTBEAT_INTERVAL_MS) {
        sendeHeartbeat();
    }

    aktualisiereRelaisVerriegelung();

    if (nodeStatus.state_report_offen || (jetzt - nodeStatus.letzter_state_ms) >= STATE_INTERVAL_MS) {
        sendeStateReport();
    }

    delay(LOOP_INTERVAL_MS);
}
