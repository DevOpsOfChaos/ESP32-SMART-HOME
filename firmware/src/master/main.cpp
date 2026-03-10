/*
====================================================================
 Projekt   : SmartHome ESP32
 Geraet    : Master (ESP32-C3)
 Datei     : main.cpp
 Version   : 0.2.1
 Stand     : 2026-03-10

 Funktion:
 Erste lauffaehige Vertikalstrecke fuer genau ein Pilotgeraet:
 net_erl -> master -> MQTT -> master -> net_erl

 Dieser Stand implementiert bewusst nur:
 - Empfang von HELLO, HEARTBEAT und STATE_REPORT via ESP-NOW
 - Verwaltung genau eines Nodes: net_erl_01
 - MQTT-Publish fuer Master-Status, Node-Status und Node-State
 - MQTT-Subscribe fuer set_relay
 - Weiterleitung COMMAND_SET_RELAY an den Pilot-Node
 - Offline-Timeout und klare Logs
====================================================================
*/

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

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

constexpr bool DEBUG_LOKAL_AKTIV = DEVICE_DEBUG_AKTIV && DEBUG_AKTIV;
constexpr char DATEI_GERAET[] = "MASTER";
constexpr char DATEI_VERSION[] = "0.2.1";
constexpr char PILOT_NODE_ID[] = "net_erl_01";
constexpr char PILOT_DEVICE_TYPE[] = "net_erl";
constexpr char MQTT_TOPIC_MASTER_STATUS[] = "smarthome/master/status";
constexpr char MQTT_TOPIC_NODE_STATUS[] = "smarthome/node/net_erl_01/status";
constexpr char MQTT_TOPIC_NODE_STATE[] = "smarthome/node/net_erl_01/state";
constexpr char MQTT_TOPIC_NODE_COMMAND[] = "smarthome/node/net_erl_01/command";

struct PilotNodeState {
    bool mac_bekannt;
    bool online;
    bool state_bekannt;
    bool relay_1;
    bool fault;
    unsigned long letzter_kontakt_ms;
    uint8_t mac[6];
};

struct MasterState {
    bool wlan_verbunden;
    bool mqtt_verbunden;
    bool espnow_bereit;
    unsigned long letzter_wlan_versuch_ms;
    unsigned long letzter_mqtt_versuch_ms;
    uint8_t naechste_seq;
};

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
PilotNodeState pilotNode = {};
MasterState masterStatus = {};

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
    if (!target || targetSize == 0) return;
    if (!source) {
        target[0] = '\0';
        return;
    }

    strncpy(target, source, targetSize - 1U);
    target[targetSize - 1U] = '\0';
}

void macText(const uint8_t* mac, char* buffer, size_t bufferSize) {
    if (!buffer || bufferSize == 0) return;
    if (!mac) {
        copyText(buffer, bufferSize, "unbekannt");
        return;
    }

    char local[18] = {0};
    SmartHome::macToString(mac, local);
    copyText(buffer, bufferSize, local);
}

bool gleicheNodeId(const char* nodeId) {
    return nodeId != nullptr && strncmp(nodeId, PILOT_NODE_ID, SH_DEVICE_ID_LEN) == 0;
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

    char text[18] = {0};
    macText(mac, text, sizeof(text));
    logf("INFO", "Peer aktiv: %s", text);
    return true;
}

void baueNodeStatusJson(char* buffer, size_t bufferSize, bool online) {
    snprintf(
        buffer,
        bufferSize,
        "{\"node_id\":\"%s\",\"online\":%s,\"device_type\":\"%s\",\"fw\":\"%s\"}",
        PILOT_NODE_ID,
        online ? "true" : "false",
        PILOT_DEVICE_TYPE,
        PROJECT_VERSION);
}

void baueNodeStateJson(char* buffer, size_t bufferSize) {
    snprintf(
        buffer,
        bufferSize,
        "{\"node_id\":\"%s\",\"relay_1\":%s,\"fault\":%s}",
        PILOT_NODE_ID,
        pilotNode.relay_1 ? "true" : "false",
        pilotNode.fault ? "true" : "false");
}

void baueMasterStatusJson(char* buffer, size_t bufferSize, bool online) {
    snprintf(
        buffer,
        bufferSize,
        "{\"online\":%s,\"wifi\":%s,\"mqtt\":%s,\"espnow\":%s,\"fw\":\"%s\"}",
        online ? "true" : "false",
        masterStatus.wlan_verbunden ? "true" : "false",
        masterStatus.mqtt_verbunden ? "true" : "false",
        masterStatus.espnow_bereit ? "true" : "false",
        PROJECT_VERSION);
}

void publishRetained(const char* topic, const char* payload) {
    if (!masterStatus.mqtt_verbunden) return;

    if (!mqttClient.publish(topic, payload, true)) {
        logf("WARN", "MQTT publish fehlgeschlagen: %s", topic);
        return;
    }

    logf("INFO", "MQTT publish %s -> %s", topic, payload);
}

void publishMasterStatus() {
    char payload[160] = {0};
    baueMasterStatusJson(payload, sizeof(payload), true);
    publishRetained(MQTT_TOPIC_MASTER_STATUS, payload);
}

void publishNodeStatus(bool online) {
    char payload[160] = {0};
    baueNodeStatusJson(payload, sizeof(payload), online);
    publishRetained(MQTT_TOPIC_NODE_STATUS, payload);
}

void publishNodeState() {
    if (!pilotNode.state_bekannt) return;

    char payload[128] = {0};
    baueNodeStateJson(payload, sizeof(payload));
    publishRetained(MQTT_TOPIC_NODE_STATE, payload);
}

void aktualisierePilotNode(const uint8_t* mac) {
    pilotNode.letzter_kontakt_ms = millis();

    if (mac != nullptr) {
        bool neueMac = !pilotNode.mac_bekannt || memcmp(pilotNode.mac, mac, 6) != 0;
        memcpy(pilotNode.mac, mac, 6);
        pilotNode.mac_bekannt = true;
        stellePeerSicher(pilotNode.mac);

        if (neueMac) {
            char text[18] = {0};
            macText(mac, text, sizeof(text));
            logf("INFO", "Pilot-Node MAC aktualisiert: %s", text);
        }
    }

    if (!pilotNode.online) {
        pilotNode.online = true;
        publishNodeStatus(true);
        logf("INFO", "Node %s ist online", PILOT_NODE_ID);
    }
}

bool sendePaket(const uint8_t* zielMac, uint8_t msgType, const void* payload, size_t payloadLen, const char* label) {
    if (zielMac == nullptr || payloadLen > SH_MAX_PAYLOAD_BYTES) return false;
    if (!stellePeerSicher(zielMac)) return false;

    uint8_t buffer[SH_ESPNOW_MAX_BYTES] = {0};
    SmartHome::MsgHeader header = {};
    SmartHome::fillHeader(header, msgType, masterStatus.naechste_seq++, 0, (uint16_t)payloadLen);

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

    char text[18] = {0};
    macText(zielMac, text, sizeof(text));
    logf("INFO", "%s gesendet an %s", label, text);
    return true;
}

void sendeHelloAck(const uint8_t* zielMac) {
    SmartHome::HelloAckPayload payload = {};
    payload.channel = (uint8_t)(masterStatus.wlan_verbunden ? WiFi.channel() : WLAN_KANAL);
    payload.ack_status = SH_ACK_OK;
    sendePaket(zielMac, SH_MSG_HELLO_ACK, &payload, sizeof(payload), "HELLO_ACK");
}

bool skipWhitespace(const char*& cursor) {
    if (cursor == nullptr) return false;
    while (*cursor != '\0' && isspace((unsigned char)*cursor)) {
        cursor++;
    }
    return *cursor != '\0';
}

bool jsonHoleString(const char* json, const char* key, char* ziel, size_t zielGroesse) {
    if (!json || !key || !ziel || zielGroesse == 0) return false;

    char muster[48] = {0};
    snprintf(muster, sizeof(muster), "\"%s\"", key);
    const char* fund = strstr(json, muster);
    if (!fund) return false;

    const char* cursor = strchr(fund + strlen(muster), ':');
    if (!cursor) return false;
    cursor++;
    if (!skipWhitespace(cursor) || *cursor != '"') return false;
    cursor++;

    const char* ende = strchr(cursor, '"');
    if (!ende) return false;

    size_t len = (size_t)(ende - cursor);
    if (len >= zielGroesse) len = zielGroesse - 1U;
    memcpy(ziel, cursor, len);
    ziel[len] = '\0';
    return true;
}

bool jsonHoleBool(const char* json, const char* key, bool* wert) {
    if (!json || !key || !wert) return false;

    char muster[48] = {0};
    snprintf(muster, sizeof(muster), "\"%s\"", key);
    const char* fund = strstr(json, muster);
    if (!fund) return false;

    const char* cursor = strchr(fund + strlen(muster), ':');
    if (!cursor) return false;
    cursor++;
    if (!skipWhitespace(cursor)) return false;

    if (strncmp(cursor, "true", 4) == 0) {
        *wert = true;
        return true;
    }
    if (strncmp(cursor, "false", 5) == 0) {
        *wert = false;
        return true;
    }
    return false;
}

void verarbeiteHello(const uint8_t* senderMac, const SmartHome::HelloPayload& payload) {
    if (!gleicheNodeId(payload.device_id)) {
        logf("WARN", "HELLO ignoriert: unerwartete node_id=%s", payload.device_id);
        return;
    }

    aktualisierePilotNode(senderMac);
    logf("INFO", "HELLO von %s (%s)", payload.device_id, payload.device_name);
    publishNodeStatus(true);
    sendeHelloAck(senderMac);
}

void verarbeiteHeartbeat(const uint8_t* senderMac, const SmartHome::HeartbeatPayload& payload) {
    if (!gleicheNodeId(payload.node_id)) {
        logf("WARN", "HEARTBEAT ignoriert: unerwartete node_id=%s", payload.node_id);
        return;
    }

    aktualisierePilotNode(senderMac);
    logf("INFO", "HEARTBEAT von %s (uptime=%lus)", payload.node_id, (unsigned long)payload.uptime_s);
}

void verarbeiteStateReport(const uint8_t* senderMac, const SmartHome::StateReportPayload& payload) {
    if (!gleicheNodeId(payload.node_id)) {
        logf("WARN", "STATE_REPORT ignoriert: unerwartete node_id=%s", payload.node_id);
        return;
    }

    aktualisierePilotNode(senderMac);
    pilotNode.relay_1 = (payload.relay_1 != 0U);
    pilotNode.fault = (payload.fault != 0U);
    pilotNode.state_bekannt = true;

    logf(
        "INFO",
        "STATE_REPORT von %s: relay_1=%s fault=%s",
        payload.node_id,
        pilotNode.relay_1 ? "true" : "false",
        pilotNode.fault ? "true" : "false");

    publishNodeState();
}

void verarbeiteEspNowPaket(const uint8_t* senderMac, const uint8_t* daten, int laenge) {
    if (!senderMac || !daten || laenge < (int)sizeof(SmartHome::MsgHeader)) {
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
        case SH_MSG_HELLO:
            if (header->payload_len == sizeof(SmartHome::HelloPayload)) {
                verarbeiteHello(senderMac, *reinterpret_cast<const SmartHome::HelloPayload*>(payload));
            }
            break;

        case SH_MSG_HEARTBEAT:
            if (header->payload_len == sizeof(SmartHome::HeartbeatPayload)) {
                verarbeiteHeartbeat(senderMac, *reinterpret_cast<const SmartHome::HeartbeatPayload*>(payload));
            }
            break;

        case SH_MSG_STATE:
            if (header->payload_len == sizeof(SmartHome::StateReportPayload)) {
                verarbeiteStateReport(senderMac, *reinterpret_cast<const SmartHome::StateReportPayload*>(payload));
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
    char text[18] = {0};
    macText(mac, text, sizeof(text));
    logf(
        status == ESP_NOW_SEND_SUCCESS ? "INFO" : "WARN",
        "ESP-NOW Sendestatus an %s: %s",
        text,
        status == ESP_NOW_SEND_SUCCESS ? "OK" : "FEHLER");
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    char json[256] = {0};
    size_t copyLen = length;
    if (copyLen >= sizeof(json)) copyLen = sizeof(json) - 1U;
    memcpy(json, payload, copyLen);
    json[copyLen] = '\0';

    logf("INFO", "MQTT empfangen %s -> %s", topic, json);

    if (strcmp(topic, MQTT_TOPIC_NODE_COMMAND) != 0) {
        logf("WARN", "MQTT Topic ignoriert: %s", topic);
        return;
    }

    char cmd[32] = {0};
    char requestId[48] = {0};
    bool relayState = false;

    if (!jsonHoleString(json, "cmd", cmd, sizeof(cmd)) || strcmp(cmd, "set_relay") != 0) {
        logf("WARN", "MQTT Command ignoriert: cmd fehlt oder ist nicht set_relay");
        return;
    }

    if (!jsonHoleBool(json, "relay_1", &relayState)) {
        logf("WARN", "MQTT Command ignoriert: relay_1 fehlt");
        return;
    }

    jsonHoleString(json, "request_id", requestId, sizeof(requestId));

    if (!pilotNode.mac_bekannt) {
        logf("WARN", "COMMAND_SET_RELAY verworfen: Node-MAC unbekannt");
        return;
    }

    SmartHome::CmdPayload cmdPayload = {};
    cmdPayload.cmd_type = SH_CMD_SET_RELAY;
    cmdPayload.param1 = 0U;
    cmdPayload.param2 = relayState ? 1U : 0U;

    logf(
        "INFO",
        "MQTT -> COMMAND_SET_RELAY relay_1=%s request_id=%s",
        relayState ? "true" : "false",
        requestId[0] != '\0' ? requestId : "-");

    sendePaket(pilotNode.mac, SH_MSG_CMD, &cmdPayload, sizeof(cmdPayload), "COMMAND_SET_RELAY");
}

void initialisiereHardware() {
    if (PIN_STATUS_LED >= 0) {
        pinMode(PIN_STATUS_LED, OUTPUT);
        digitalWrite(PIN_STATUS_LED, LOW);
    }
}

void initialisiereWlan() {
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    masterStatus.letzter_wlan_versuch_ms = millis();
    logf("INFO", "WLAN-Verbindung gestartet: SSID=%s", WIFI_SSID);
}

void pruefeWlanVerbindung() {
    bool verbunden = (WiFi.status() == WL_CONNECTED);
    if (verbunden != masterStatus.wlan_verbunden) {
        masterStatus.wlan_verbunden = verbunden;
        if (verbunden) {
            logf("INFO", "WLAN verbunden: IP=%s Kanal=%d", WiFi.localIP().toString().c_str(), WiFi.channel());
        } else {
            logf("WARN", "WLAN getrennt");
        }
    }

    if (!verbunden && (millis() - masterStatus.letzter_wlan_versuch_ms) >= WIFI_RECONNECT_INTERVAL_MS) {
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        masterStatus.letzter_wlan_versuch_ms = millis();
        logf("INFO", "WLAN-Reconnect gestartet");
    }
}

void initialisiereEspNow() {
    if (!masterStatus.wlan_verbunden) {
        esp_wifi_set_channel((uint8_t)WLAN_KANAL, WIFI_SECOND_CHAN_NONE);
    }

    if (esp_now_init() != ESP_OK) {
        masterStatus.espnow_bereit = false;
        logf("WARN", "ESP-NOW Initialisierung fehlgeschlagen");
        return;
    }

    esp_now_register_send_cb(onEspNowSent);
    esp_now_register_recv_cb(onEspNowReceive);
    masterStatus.espnow_bereit = true;
    logf("INFO", "ESP-NOW bereit");
}

void initialisiereMqtt() {
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setBufferSize(256);
    logf("INFO", "MQTT konfiguriert: %s:%d", MQTT_HOST, MQTT_PORT);
}

void pruefeMqttVerbindung() {
    if (!masterStatus.wlan_verbunden) {
        if (masterStatus.mqtt_verbunden) {
            mqttClient.disconnect();
            masterStatus.mqtt_verbunden = false;
        }
        return;
    }

    if (mqttClient.connected()) {
        mqttClient.loop();
        if (!masterStatus.mqtt_verbunden) {
            masterStatus.mqtt_verbunden = true;
        }
        return;
    }

    masterStatus.mqtt_verbunden = false;
    if ((millis() - masterStatus.letzter_mqtt_versuch_ms) < MQTT_RECONNECT_INTERVAL_MS) {
        return;
    }

    masterStatus.letzter_mqtt_versuch_ms = millis();

    char willPayload[160] = {0};
    baueMasterStatusJson(willPayload, sizeof(willPayload), false);

    bool verbunden = mqttClient.connect(
        DEVICE_ID,
        MQTT_USER,
        MQTT_PASSWORD,
        MQTT_TOPIC_MASTER_STATUS,
        0,
        true,
        willPayload,
        true);

    if (!verbunden) {
        logf("WARN", "MQTT connect fehlgeschlagen (state=%d)", mqttClient.state());
        return;
    }

    masterStatus.mqtt_verbunden = true;
    logf("INFO", "MQTT verbunden");

    if (!mqttClient.subscribe(MQTT_TOPIC_NODE_COMMAND)) {
        logf("WARN", "MQTT Subscribe fehlgeschlagen: %s", MQTT_TOPIC_NODE_COMMAND);
    } else {
        logf("INFO", "MQTT Subscribe aktiv: %s", MQTT_TOPIC_NODE_COMMAND);
    }

    publishMasterStatus();
    publishNodeStatus(pilotNode.online);
    publishNodeState();
}

void pruefeOfflineTimeout() {
    if (!pilotNode.online) return;

    if ((millis() - pilotNode.letzter_kontakt_ms) > NODE_OFFLINE_TIMEOUT_MS) {
        pilotNode.online = false;
        publishNodeStatus(false);
        logf("WARN", "Node %s nach Timeout offline", PILOT_NODE_ID);
    }
}

void gibStartmeldungAus() {
    if (!DEBUG_LOKAL_AKTIV) return;

    Serial.println("================================");
    Serial.println(PROJECT_NAME);
    Serial.print(DATEI_GERAET);
    Serial.print(" v");
    Serial.println(DATEI_VERSION);
    Serial.print("Pilot-Node: ");
    Serial.println(PILOT_NODE_ID);
    Serial.print("FW: ");
    Serial.println(PROJECT_VERSION);
    Serial.println("Minimalstrecke: ESP-NOW <-> MQTT <-> ESP-NOW");
    Serial.println("================================");
}

void setup() {
    if (DEBUG_LOKAL_AKTIV) {
        Serial.begin(115200);
        delay(150);
    }

    pilotNode = {};
    masterStatus = {};
    gibStartmeldungAus();
    initialisiereHardware();
    initialisiereWlan();
    delay(500);
    pruefeWlanVerbindung();
    initialisiereEspNow();
    initialisiereMqtt();
}

void loop() {
    pruefeWlanVerbindung();
    pruefeMqttVerbindung();
    pruefeOfflineTimeout();
    delay(LOOP_INTERVAL_MS);
}
