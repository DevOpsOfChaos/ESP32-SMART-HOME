/*
====================================================================
 Projekt   : SmartHome ESP32
 Geraet    : Master (ESP32-C3)
 Datei     : main.cpp
 Version   : 0.2.2
 Stand     : 2026-03-10

 Funktion:
 Feste Firmware-Basis fuer vier bekannte Basisgeraete:
 - net_erl_01
 - net_zrl_01
 - net_sen_01
 - bat_sen_01

 Dieser Stand erweitert die reale Minimalstrecke kontrolliert:
 - HELLO / HELLO_ACK / HEARTBEAT fuer alle vier Basisgeraete
 - STATE_REPORT fuer Relais-, Sensor- und Batterie-Basis
 - EVENT-Weitergabe fuer eventfaehige Basen
 - MQTT Topics gemaess docs/04_mqtt_topics.md
 - Keine offene Mehrgeraete-Architektur und keine Sonderlogik je Endgeraet
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
#include <stdlib.h>
#include <limits.h>

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
constexpr char DATEI_VERSION[] = "0.2.2";
constexpr char MQTT_TOPIC_CMD_SET_SUB[] = "smarthome/node/+/cmd/set";
constexpr char MQTT_TOPIC_CMD_GET_SUB[] = "smarthome/node/+/cmd/get";
constexpr uint8_t WINDOW_STATE_UNKNOWN = 0xFFU;
constexpr size_t REQUEST_ID_LEN = 96U;

struct NodeDefinition {
    const char* node_id;
    const char* device_type;
    uint8_t device_class;
    uint8_t power_type;
    unsigned long offline_timeout_ms;
};

struct NodeRuntime {
    bool meta_bekannt;
    bool online;
    bool state_bekannt;
    bool mac_bekannt;
    bool fault;
    bool relay_1;
    bool relay_2;
    bool cover_mode;
    uint8_t cover_state;
    uint8_t cover_position;
    int16_t temp_01c;
    uint16_t hum_01pct;
    uint16_t lux;
    bool motion;
    uint8_t battery_pct;
    uint16_t battery_mv;
    uint8_t window_open;
    uint16_t rain_raw;
    uint8_t button_flags;
    uint32_t uptime_s;
    uint16_t caps;
    uint16_t fw_version;
    char device_name[SH_DEVICE_NAME_LEN];
    uint8_t mac[6];
    unsigned long letzter_kontakt_ms;
};

struct MasterState {
    bool wlan_verbunden;
    bool mqtt_verbunden;
    bool espnow_bereit;
    unsigned long letzter_wlan_versuch_ms;
    unsigned long letzter_mqtt_versuch_ms;
    uint8_t naechste_seq;
};

struct PendingCommand {
    bool aktiv;
    size_t node_index;
    uint8_t seq;
    uint8_t relay_index;
    bool relay_state;
    uint8_t versuche;
    unsigned long letztes_senden_ms;
    char request_id[REQUEST_ID_LEN];
};

struct CommandAckCache {
    bool gueltig;
    size_t node_index;
    unsigned long gespeichert_ms;
    char request_id[REQUEST_ID_LEN];
    char ack_payload[256];
};

constexpr NodeDefinition NODE_DEFINITIONS[] = {
    {"net_erl_01", "net_erl", SH_CLASS_NET_ERL, SH_POWER_MAINS, NODE_OFFLINE_TIMEOUT_MS},
    {"net_zrl_01", "net_zrl", SH_CLASS_NET_ZRL, SH_POWER_MAINS, NODE_OFFLINE_TIMEOUT_MS},
    {"net_sen_01", "net_sen", SH_CLASS_NET_SEN, SH_POWER_MAINS, NODE_OFFLINE_TIMEOUT_MS},
    {"bat_sen_01", "bat_sen", SH_CLASS_BAT_SEN, SH_POWER_BATTERY, BATTERY_NODE_OFFLINE_TIMEOUT_MS},
};

constexpr size_t NODE_COUNT = sizeof(NODE_DEFINITIONS) / sizeof(NODE_DEFINITIONS[0]);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
MasterState masterStatus = {};
NodeRuntime nodeStates[NODE_COUNT] = {};
PendingCommand pendingCommand = {};
CommandAckCache commandAckCache = {};

void logf(const char* level, const char* format, ...) {
    if (!DEBUG_LOKAL_AKTIV) return;

    char message[224];
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

void macText(const uint8_t* mac, char* buffer, size_t bufferSize) {
    if (!buffer || bufferSize == 0U) return;
    if (!mac) {
        copyText(buffer, bufferSize, "unbekannt");
        return;
    }

    char local[18] = {0};
    SmartHome::macToString(mac, local);
    copyText(buffer, bufferSize, local);
}

const char* powerTypeText(uint8_t powerType) {
    return powerType == SH_POWER_BATTERY ? "battery" : "mains";
}

const char* eventTypeText(uint8_t eventType) {
    switch (eventType) {
        case SH_EVENT_BUTTON_PRESS: return "button_press";
        case SH_EVENT_MOTION_DETECTED: return "motion_detected";
        case SH_EVENT_WINDOW_OPENED: return "window_opened";
        case SH_EVENT_WINDOW_CLOSED: return "window_closed";
        case SH_EVENT_RAIN_DETECTED: return "rain_detected";
        case SH_EVENT_RELAY_CHANGED: return "relay_changed";
        case SH_EVENT_LIGHT_AUTO_ON: return "light_auto_on";
        case SH_EVENT_LIGHT_AUTO_OFF: return "light_auto_off";
        case SH_EVENT_COVER_UP: return "cover_up";
        case SH_EVENT_COVER_DOWN: return "cover_down";
        case SH_EVENT_COVER_STOP: return "cover_stop";
        case SH_EVENT_NODE_BOOT: return "node_boot";
        case SH_EVENT_SENSOR_FAULT: return "sensor_fault";
        case SH_EVENT_COMM_FAULT: return "comm_fault";
        default: return "unknown";
    }
}

void schreibeIntOrNull(char* buffer, size_t bufferSize, long value, long invalidValue) {
    if (value == invalidValue) {
        copyText(buffer, bufferSize, "null");
        return;
    }
    snprintf(buffer, bufferSize, "%ld", value);
}

void schreibeUIntOrNull(char* buffer, size_t bufferSize, unsigned long value, unsigned long invalidValue) {
    if (value == invalidValue) {
        copyText(buffer, bufferSize, "null");
        return;
    }
    snprintf(buffer, bufferSize, "%lu", value);
}

void baueMasterTopic(const char* channel, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize, "smarthome/master/%s/%s", DEVICE_ID, channel);
}

void baueNodeTopic(size_t nodeIndex, const char* suffix, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize, "smarthome/node/%s/%s", NODE_DEFINITIONS[nodeIndex].node_id, suffix);
}

void initialisiereNodeStates() {
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        nodeStates[i] = {};
        nodeStates[i].cover_state = SH_COVER_STATE_STOPPED;
        nodeStates[i].cover_position = 0xFFU;
        nodeStates[i].temp_01c = INT16_MIN;
        nodeStates[i].hum_01pct = 0xFFFFU;
        nodeStates[i].lux = 0xFFFFU;
        nodeStates[i].battery_pct = 0xFFU;
        nodeStates[i].window_open = WINDOW_STATE_UNKNOWN;
        nodeStates[i].rain_raw = 0xFFFFU;
        copyText(nodeStates[i].device_name, sizeof(nodeStates[i].device_name), NODE_DEFINITIONS[i].device_type);
    }
}

int findeNodeIndex(const char* nodeId) {
    if (!nodeId) return -1;

    for (size_t i = 0; i < NODE_COUNT; ++i) {
        if (strncmp(nodeId, NODE_DEFINITIONS[i].node_id, SH_DEVICE_ID_LEN) == 0) {
            return (int)i;
        }
    }

    return -1;
}

bool istRelayNode(size_t nodeIndex) {
    return NODE_DEFINITIONS[nodeIndex].device_class == SH_CLASS_NET_ERL ||
           NODE_DEFINITIONS[nodeIndex].device_class == SH_CLASS_NET_ZRL;
}

uint16_t holeHelloCaps(const SmartHome::HelloPayload& payload) {
    return (uint16_t)(((uint16_t)payload.caps_hi << 8) | payload.caps_lo);
}

bool nutztCommandAckPfad(size_t nodeIndex) {
    return strcmp(NODE_DEFINITIONS[nodeIndex].node_id, "net_erl_01") == 0;
}

bool gleicheRequestId(const char* links, const char* rechts) {
    return links != nullptr &&
           rechts != nullptr &&
           links[0] != '\0' &&
           rechts[0] != '\0' &&
           strcmp(links, rechts) == 0;
}

void pruefeCommandAckCacheAblauf() {
    if (!commandAckCache.gueltig) return;

    if ((millis() - commandAckCache.gespeichert_ms) > COMMAND_ACK_CACHE_MS) {
        commandAckCache = {};
    }
}

int findeNodeIndexPerMac(const uint8_t* mac) {
    if (mac == nullptr) return -1;

    for (size_t i = 0; i < NODE_COUNT; ++i) {
        if (nodeStates[i].mac_bekannt && memcmp(nodeStates[i].mac, mac, 6) == 0) {
            return (int)i;
        }
    }

    return -1;
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

bool sendePaketMitOptionen(
    const uint8_t* zielMac,
    uint8_t msgType,
    const void* payload,
    size_t payloadLen,
    const char* label,
    uint8_t flags,
    bool festeSeq,
    uint8_t seq,
    uint8_t* verwendeteSeq)
{
    if (zielMac == nullptr || payloadLen > SH_MAX_PAYLOAD_BYTES) return false;
    if (!stellePeerSicher(zielMac)) return false;

    uint8_t buffer[SH_ESPNOW_MAX_BYTES] = {0};
    SmartHome::MsgHeader header = {};
    const uint8_t effektiveSeq = festeSeq ? seq : masterStatus.naechste_seq++;
    SmartHome::fillHeader(header, msgType, effektiveSeq, flags, (uint16_t)payloadLen);

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

    char text[18] = {0};
    macText(zielMac, text, sizeof(text));
    logf("INFO", "%s gesendet an %s", label, text);
    if (verwendeteSeq != nullptr) {
        *verwendeteSeq = effektiveSeq;
    }
    return true;
}

bool sendePaket(const uint8_t* zielMac, uint8_t msgType, const void* payload, size_t payloadLen, const char* label) {
    return sendePaketMitOptionen(zielMac, msgType, payload, payloadLen, label, 0U, false, 0U, nullptr);
}

void baueMasterStatusJson(char* buffer, size_t bufferSize, bool online) {
    snprintf(
        buffer,
        bufferSize,
        "{\"master_id\":\"%s\",\"online\":%s,\"wifi\":%s,\"mqtt\":%s,\"espnow\":%s,\"fw\":\"%s\"}",
        DEVICE_ID,
        online ? "true" : "false",
        masterStatus.wlan_verbunden ? "true" : "false",
        masterStatus.mqtt_verbunden ? "true" : "false",
        masterStatus.espnow_bereit ? "true" : "false",
        PROJECT_VERSION);
}

void baueMasterEventJson(char* buffer, size_t bufferSize, const char* eventName) {
    snprintf(
        buffer,
        bufferSize,
        "{\"master_id\":\"%s\",\"event\":\"%s\",\"online\":true,\"wifi\":%s,\"mqtt\":%s,\"espnow\":%s,\"fw\":\"%s\"}",
        DEVICE_ID,
        eventName ? eventName : "unknown",
        masterStatus.wlan_verbunden ? "true" : "false",
        masterStatus.mqtt_verbunden ? "true" : "false",
        masterStatus.espnow_bereit ? "true" : "false",
        PROJECT_VERSION);
}

void baueNodeMetaJson(size_t nodeIndex, char* buffer, size_t bufferSize) {
    char macBuffer[18] = {0};
    macText(nodeStates[nodeIndex].mac_bekannt ? nodeStates[nodeIndex].mac : nullptr, macBuffer, sizeof(macBuffer));

    snprintf(
        buffer,
        bufferSize,
        "{\"node_id\":\"%s\",\"device_name\":\"%s\",\"device_type\":\"%s\",\"device_class\":%u,\"power_type\":\"%s\",\"caps\":%u,\"fw_version\":%u,\"mac\":\"%s\"}",
        NODE_DEFINITIONS[nodeIndex].node_id,
        nodeStates[nodeIndex].device_name,
        NODE_DEFINITIONS[nodeIndex].device_type,
        (unsigned)NODE_DEFINITIONS[nodeIndex].device_class,
        powerTypeText(NODE_DEFINITIONS[nodeIndex].power_type),
        (unsigned)nodeStates[nodeIndex].caps,
        (unsigned)nodeStates[nodeIndex].fw_version,
        macBuffer);
}

void baueNodeStatusJson(size_t nodeIndex, char* buffer, size_t bufferSize, bool online) {
    snprintf(
        buffer,
        bufferSize,
        "{\"node_id\":\"%s\",\"online\":%s,\"device_type\":\"%s\",\"power_type\":\"%s\",\"fw\":\"%s\"}",
        NODE_DEFINITIONS[nodeIndex].node_id,
        online ? "true" : "false",
        NODE_DEFINITIONS[nodeIndex].device_type,
        powerTypeText(NODE_DEFINITIONS[nodeIndex].power_type),
        PROJECT_VERSION);
}

void baueNodeStateJson(size_t nodeIndex, char* buffer, size_t bufferSize) {
    char tempText[16] = {0};
    char humText[16] = {0};
    char luxText[16] = {0};
    char batteryPctText[16] = {0};
    char batteryMvText[16] = {0};
    char windowText[16] = {0};
    char rainText[16] = {0};
    char coverPositionText[16] = {0};

    switch (NODE_DEFINITIONS[nodeIndex].device_class) {
        case SH_CLASS_NET_ERL:
            snprintf(
                buffer,
                bufferSize,
                "{\"node_id\":\"%s\",\"relay_1\":%s,\"fault\":%s}",
                NODE_DEFINITIONS[nodeIndex].node_id,
                nodeStates[nodeIndex].relay_1 ? "true" : "false",
                nodeStates[nodeIndex].fault ? "true" : "false");
            return;

        case SH_CLASS_NET_ZRL:
            schreibeUIntOrNull(coverPositionText, sizeof(coverPositionText), nodeStates[nodeIndex].cover_position, 0xFFU);
            snprintf(
                buffer,
                bufferSize,
                "{\"node_id\":\"%s\",\"relay_1\":%s,\"relay_2\":%s,\"cover_mode\":%s,\"cover_state\":%u,\"cover_position\":%s,\"fault\":%s}",
                NODE_DEFINITIONS[nodeIndex].node_id,
                nodeStates[nodeIndex].relay_1 ? "true" : "false",
                nodeStates[nodeIndex].relay_2 ? "true" : "false",
                nodeStates[nodeIndex].cover_mode ? "true" : "false",
                (unsigned)nodeStates[nodeIndex].cover_state,
                coverPositionText,
                nodeStates[nodeIndex].fault ? "true" : "false");
            return;

        case SH_CLASS_NET_SEN:
            schreibeIntOrNull(tempText, sizeof(tempText), nodeStates[nodeIndex].temp_01c, INT16_MIN);
            schreibeUIntOrNull(humText, sizeof(humText), nodeStates[nodeIndex].hum_01pct, 0xFFFFU);
            schreibeUIntOrNull(luxText, sizeof(luxText), nodeStates[nodeIndex].lux, 0xFFFFU);
            snprintf(
                buffer,
                bufferSize,
                "{\"node_id\":\"%s\",\"temp_01c\":%s,\"hum_01pct\":%s,\"lux\":%s,\"motion\":%s,\"fault\":%s}",
                NODE_DEFINITIONS[nodeIndex].node_id,
                tempText,
                humText,
                luxText,
                nodeStates[nodeIndex].motion ? "true" : "false",
                nodeStates[nodeIndex].fault ? "true" : "false");
            return;

        case SH_CLASS_BAT_SEN:
            schreibeUIntOrNull(batteryPctText, sizeof(batteryPctText), nodeStates[nodeIndex].battery_pct, 0xFFU);
            schreibeUIntOrNull(batteryMvText, sizeof(batteryMvText), nodeStates[nodeIndex].battery_mv, 0U);
            schreibeUIntOrNull(windowText, sizeof(windowText), nodeStates[nodeIndex].window_open, WINDOW_STATE_UNKNOWN);
            schreibeUIntOrNull(rainText, sizeof(rainText), nodeStates[nodeIndex].rain_raw, 0xFFFFU);
            snprintf(
                buffer,
                bufferSize,
                "{\"node_id\":\"%s\",\"battery_pct\":%s,\"battery_mv\":%s,\"window_open\":%s,\"rain_raw\":%s,\"button_flags\":%u,\"fault\":%s}",
                NODE_DEFINITIONS[nodeIndex].node_id,
                batteryPctText,
                batteryMvText,
                windowText,
                rainText,
                (unsigned)nodeStates[nodeIndex].button_flags,
                nodeStates[nodeIndex].fault ? "true" : "false");
            return;

        default:
            copyText(buffer, bufferSize, "{}");
            return;
    }
}

void baueNodeEventJson(size_t nodeIndex, const SmartHome::EventReportPayload& payload, char* buffer, size_t bufferSize) {
    snprintf(
        buffer,
        bufferSize,
        "{\"node_id\":\"%s\",\"event\":\"%s\",\"event_type\":%u,\"trigger\":%u,\"param1\":%u,\"param2\":%u}",
        NODE_DEFINITIONS[nodeIndex].node_id,
        eventTypeText(payload.event_type),
        (unsigned)payload.event_type,
        (unsigned)payload.trigger,
        (unsigned)payload.param1,
        (unsigned)payload.param2);
}

void publishRetained(const char* topic, const char* payload) {
    if (!masterStatus.mqtt_verbunden) return;

    if (!mqttClient.publish(topic, payload, true)) {
        logf("WARN", "MQTT publish fehlgeschlagen: %s", topic);
        return;
    }

    logf("INFO", "MQTT retained %s -> %s", topic, payload);
}

void publishTransient(const char* topic, const char* payload) {
    if (!masterStatus.mqtt_verbunden) return;

    if (!mqttClient.publish(topic, payload, false)) {
        logf("WARN", "MQTT publish fehlgeschlagen: %s", topic);
        return;
    }

    logf("INFO", "MQTT publish %s -> %s", topic, payload);
}

void publishMasterStatus() {
    char topic[96] = {0};
    char payload[192] = {0};
    baueMasterTopic("status", topic, sizeof(topic));
    baueMasterStatusJson(payload, sizeof(payload), true);
    publishRetained(topic, payload);
}

void publishMasterEvent(const char* eventName) {
    char topic[96] = {0};
    char payload[224] = {0};
    baueMasterTopic("event", topic, sizeof(topic));
    baueMasterEventJson(payload, sizeof(payload), eventName);
    publishTransient(topic, payload);
}

void publishNodeMeta(size_t nodeIndex) {
    if (!nodeStates[nodeIndex].meta_bekannt) return;

    char topic[96] = {0};
    char payload[224] = {0};
    baueNodeTopic(nodeIndex, "meta", topic, sizeof(topic));
    baueNodeMetaJson(nodeIndex, payload, sizeof(payload));
    publishRetained(topic, payload);
}

void publishNodeStatus(size_t nodeIndex, bool online) {
    char topic[96] = {0};
    char payload[192] = {0};
    baueNodeTopic(nodeIndex, "status", topic, sizeof(topic));
    baueNodeStatusJson(nodeIndex, payload, sizeof(payload), online);
    publishRetained(topic, payload);
}

void publishNodeState(size_t nodeIndex) {
    if (!nodeStates[nodeIndex].state_bekannt) return;

    char topic[96] = {0};
    char payload[224] = {0};
    baueNodeTopic(nodeIndex, "state", topic, sizeof(topic));
    baueNodeStateJson(nodeIndex, payload, sizeof(payload));
    publishRetained(topic, payload);
}

void publishNodeEvent(size_t nodeIndex, const SmartHome::EventReportPayload& payload) {
    char topic[96] = {0};
    char json[224] = {0};
    baueNodeTopic(nodeIndex, "event", topic, sizeof(topic));
    baueNodeEventJson(nodeIndex, payload, json, sizeof(json));
    publishTransient(topic, json);
}

void publishNodeAckPayload(size_t nodeIndex, const char* ackPayload) {
    if (!ackPayload || ackPayload[0] == '\0') return;

    char topic[96] = {0};
    baueNodeTopic(nodeIndex, "ack", topic, sizeof(topic));
    publishTransient(topic, ackPayload);
}

void speichereCommandAckCache(size_t nodeIndex, const char* requestId, const char* ackPayload) {
    if (!requestId || requestId[0] == '\0' || !ackPayload || ackPayload[0] == '\0') {
        return;
    }

    commandAckCache = {};
    commandAckCache.gueltig = true;
    commandAckCache.node_index = nodeIndex;
    commandAckCache.gespeichert_ms = millis();
    copyText(commandAckCache.request_id, sizeof(commandAckCache.request_id), requestId);
    copyText(commandAckCache.ack_payload, sizeof(commandAckCache.ack_payload), ackPayload);
}

void sendeCommandAckMqtt(
    size_t nodeIndex,
    const char* requestId,
    const char* statusText,
    int statusCode,
    uint8_t ackMsgType,
    uint8_t ackSeq,
    uint8_t versuche,
    const char* source,
    bool retryExhausted,
    bool inCacheMerken)
{
    char ackPayload[256] = {0};
    const uint8_t retryCount = versuche > 0U ? (uint8_t)(versuche - 1U) : 0U;
    snprintf(
        ackPayload,
        sizeof(ackPayload),
        "{\"node_id\":\"%s\",\"request_id\":\"%s\",\"channel\":\"cmd/set\",\"status\":\"%s\",\"status_code\":%d,\"ack_msg_type\":%u,\"ack_seq\":%u,\"attempts\":%u,\"retry_count\":%u,\"retry_exhausted\":%s,\"source\":\"%s\"}",
        NODE_DEFINITIONS[nodeIndex].node_id,
        requestId ? requestId : "",
        statusText ? statusText : "unknown",
        statusCode,
        (unsigned)ackMsgType,
        (unsigned)ackSeq,
        (unsigned)versuche,
        (unsigned)retryCount,
        retryExhausted ? "true" : "false",
        source ? source : "master");

    publishNodeAckPayload(nodeIndex, ackPayload);
    if (inCacheMerken) {
        speichereCommandAckCache(nodeIndex, requestId, ackPayload);
    }
}

void publishBekannteNodesNachReconnect() {
    publishMasterStatus();
    publishMasterEvent("mqtt_connected");

    for (size_t i = 0; i < NODE_COUNT; ++i) {
        publishNodeMeta(i);
        publishNodeStatus(i, nodeStates[i].online);
        publishNodeState(i);
    }
}

void aktualisiereNodeKontakt(size_t nodeIndex, const uint8_t* mac) {
    nodeStates[nodeIndex].letzter_kontakt_ms = millis();

    if (mac != nullptr) {
        bool neueMac = !nodeStates[nodeIndex].mac_bekannt ||
                       memcmp(nodeStates[nodeIndex].mac, mac, 6) != 0;
        memcpy(nodeStates[nodeIndex].mac, mac, 6);
        nodeStates[nodeIndex].mac_bekannt = true;
        stellePeerSicher(nodeStates[nodeIndex].mac);

        if (neueMac) {
            char text[18] = {0};
            macText(mac, text, sizeof(text));
            logf("INFO", "%s MAC aktualisiert: %s", NODE_DEFINITIONS[nodeIndex].node_id, text);
        }
    }

    if (!nodeStates[nodeIndex].online) {
        nodeStates[nodeIndex].online = true;
        publishNodeStatus(nodeIndex, true);
        logf("INFO", "Node %s ist online", NODE_DEFINITIONS[nodeIndex].node_id);
    }
}

void sendeHelloAck(const uint8_t* zielMac, uint8_t ackStatus) {
    SmartHome::HelloAckPayload payload = {};
    payload.channel = (uint8_t)(masterStatus.wlan_verbunden ? WiFi.channel() : WLAN_KANAL);
    payload.ack_status = ackStatus;
    sendePaket(zielMac, SH_MSG_HELLO_ACK, &payload, sizeof(payload), "HELLO_ACK");
}

bool wiederholePendingCommand() {
    if (!pendingCommand.aktiv) return false;

    const size_t nodeIndex = pendingCommand.node_index;
    if (nodeIndex >= NODE_COUNT || !nodeStates[nodeIndex].mac_bekannt) {
        logf("WARN", "Retry verworfen: MAC fuer %s unbekannt", NODE_DEFINITIONS[nodeIndex].node_id);
        return false;
    }

    SmartHome::CmdPayload payload = {};
    payload.cmd_type = SH_CMD_SET_RELAY;
    payload.param1 = pendingCommand.relay_index;
    payload.param2 = pendingCommand.relay_state ? 1U : 0U;

    const bool erfolgreich = sendePaketMitOptionen(
        nodeStates[nodeIndex].mac,
        SH_MSG_CMD,
        &payload,
        sizeof(payload),
        "COMMAND_SET_RELAY retry",
        (uint8_t)(SH_FLAG_ACK_REQUEST | SH_FLAG_RETRANSMIT),
        true,
        pendingCommand.seq,
        nullptr);

    if (erfolgreich) {
        pendingCommand.versuche++;
        pendingCommand.letztes_senden_ms = millis();
        logf(
            "WARN",
            "Retry %u/%u fuer %s request_id=%s seq=%u",
            (unsigned)(pendingCommand.versuche - 1U),
            (unsigned)COMMAND_MAX_RETRIES,
            NODE_DEFINITIONS[nodeIndex].node_id,
            pendingCommand.request_id,
            (unsigned)pendingCommand.seq);
    }

    return erfolgreich;
}

void verarbeiteAck(const uint8_t* senderMac, const SmartHome::AckPayload& payload) {
    const int nodeIndex = findeNodeIndexPerMac(senderMac);
    if (nodeIndex < 0) {
        logf("WARN", "ACK ignoriert: unbekannte Sender-MAC");
        return;
    }

    if (!pendingCommand.aktiv || pendingCommand.node_index != (size_t)nodeIndex) {
        logf("WARN", "ACK ignoriert: kein offenes Kommando fuer %s", NODE_DEFINITIONS[nodeIndex].node_id);
        return;
    }

    if (payload.ack_msg_type != SH_MSG_CMD || payload.ack_seq != pendingCommand.seq) {
        logf(
            "WARN",
            "ACK ignoriert: seq/msg_type passen nicht (seq=%u, msg_type=%u)",
            (unsigned)payload.ack_seq,
            (unsigned)payload.ack_msg_type);
        return;
    }

    const char* statusText = "error";
    switch (payload.status) {
        case SH_ACK_OK:
            statusText = "ok";
            break;
        case SH_ACK_REJECTED:
            statusText = "rejected";
            break;
        default:
            statusText = "error";
            break;
    }

    sendeCommandAckMqtt(
        (size_t)nodeIndex,
        pendingCommand.request_id,
        statusText,
        (int)payload.status,
        payload.ack_msg_type,
        payload.ack_seq,
        pendingCommand.versuche,
        "node_ack",
        false,
        true);

    logf(
        "INFO",
        "ACK von %s fuer request_id=%s seq=%u status=%u",
        NODE_DEFINITIONS[nodeIndex].node_id,
        pendingCommand.request_id,
        (unsigned)payload.ack_seq,
        (unsigned)payload.status);

    pendingCommand = {};
}

bool skipWhitespace(const char*& cursor) {
    if (cursor == nullptr) return false;
    while (*cursor != '\0' && isspace((unsigned char)*cursor)) {
        cursor++;
    }
    return *cursor != '\0';
}

bool jsonHoleString(const char* json, const char* key, char* ziel, size_t zielGroesse) {
    if (!json || !key || !ziel || zielGroesse == 0U) return false;

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

bool extrahiereNodeCommandTopic(const char* topic, char* nodeId, size_t nodeIdSize, bool* istSet, bool* istGet) {
    if (!topic || !nodeId || nodeIdSize == 0U || !istSet || !istGet) return false;

    const char* prefix = "smarthome/node/";
    const size_t prefixLen = strlen(prefix);
    if (strncmp(topic, prefix, prefixLen) != 0) return false;

    const char* start = topic + prefixLen;
    const char* slash = strchr(start, '/');
    if (!slash) return false;

    size_t len = (size_t)(slash - start);
    if (len == 0U || len >= nodeIdSize) return false;

    memcpy(nodeId, start, len);
    nodeId[len] = '\0';

    *istSet = strcmp(slash, "/cmd/set") == 0;
    *istGet = strcmp(slash, "/cmd/get") == 0;
    return *istSet || *istGet;
}

void verarbeiteHello(const uint8_t* senderMac, const SmartHome::HelloPayload& payload) {
    int nodeIndex = findeNodeIndex(payload.device_id);
    if (nodeIndex < 0) {
        logf("WARN", "HELLO ignoriert: unbekannte node_id=%s", payload.device_id);
        sendeHelloAck(senderMac, SH_ACK_REJECTED);
        return;
    }

    if (payload.device_class != NODE_DEFINITIONS[nodeIndex].device_class ||
        payload.power_type != NODE_DEFINITIONS[nodeIndex].power_type) {
        logf(
            "WARN",
            "HELLO abgelehnt: node=%s class=%u power=%u",
            payload.device_id,
            (unsigned)payload.device_class,
            (unsigned)payload.power_type);
        sendeHelloAck(senderMac, SH_ACK_REJECTED);
        return;
    }

    nodeStates[nodeIndex].meta_bekannt = true;
    nodeStates[nodeIndex].caps = holeHelloCaps(payload);
    nodeStates[nodeIndex].fw_version = payload.fw_version;
    copyText(nodeStates[nodeIndex].device_name, sizeof(nodeStates[nodeIndex].device_name), payload.device_name);

    aktualisiereNodeKontakt((size_t)nodeIndex, senderMac);
    publishNodeMeta((size_t)nodeIndex);
    publishNodeStatus((size_t)nodeIndex, true);
    sendeHelloAck(senderMac, SH_ACK_OK);

    logf("INFO", "HELLO von %s (%s)", payload.device_id, payload.device_name);
}

void verarbeiteHeartbeat(const uint8_t* senderMac, const SmartHome::HeartbeatPayload& payload) {
    int nodeIndex = findeNodeIndex(payload.node_id);
    if (nodeIndex < 0) {
        logf("WARN", "HEARTBEAT ignoriert: unbekannte node_id=%s", payload.node_id);
        return;
    }

    aktualisiereNodeKontakt((size_t)nodeIndex, senderMac);
    nodeStates[nodeIndex].uptime_s = payload.uptime_s;
    logf("INFO", "HEARTBEAT von %s (uptime=%lus)", payload.node_id, (unsigned long)payload.uptime_s);
}

void verarbeiteStateReport(const uint8_t* senderMac, const uint8_t* payload, uint16_t payloadLen) {
    if (!payload || payloadLen < SH_DEVICE_ID_LEN) {
        logf("WARN", "STATE_REPORT verworfen: payload ungueltig");
        return;
    }

    const char* nodeId = reinterpret_cast<const char*>(payload);
    int nodeIndex = findeNodeIndex(nodeId);
    if (nodeIndex < 0) {
        logf("WARN", "STATE_REPORT ignoriert: unbekannte node_id=%s", nodeId);
        return;
    }

    aktualisiereNodeKontakt((size_t)nodeIndex, senderMac);

    switch (NODE_DEFINITIONS[nodeIndex].device_class) {
        case SH_CLASS_NET_ERL:
            if (payloadLen != sizeof(SmartHome::StateReportPayload)) {
                logf("WARN", "STATE_REPORT Laenge ungueltig fuer %s", nodeId);
                return;
            }
            {
                const SmartHome::StateReportPayload& state = *reinterpret_cast<const SmartHome::StateReportPayload*>(payload);
                nodeStates[nodeIndex].relay_1 = (state.relay_1 != 0U);
                nodeStates[nodeIndex].fault = (state.fault != 0U);
            }
            break;

        case SH_CLASS_NET_ZRL:
            if (payloadLen != sizeof(SmartHome::ZrlStateReportPayload)) {
                logf("WARN", "STATE_REPORT Laenge ungueltig fuer %s", nodeId);
                return;
            }
            {
                const SmartHome::ZrlStateReportPayload& state = *reinterpret_cast<const SmartHome::ZrlStateReportPayload*>(payload);
                nodeStates[nodeIndex].relay_1 = (state.relay_1 != 0U);
                nodeStates[nodeIndex].relay_2 = (state.relay_2 != 0U);
                nodeStates[nodeIndex].cover_mode = (state.cover_mode != 0U);
                nodeStates[nodeIndex].cover_state = state.cover_state;
                nodeStates[nodeIndex].cover_position = state.cover_position;
                nodeStates[nodeIndex].fault = (state.fault != 0U);
            }
            break;

        case SH_CLASS_NET_SEN:
            if (payloadLen != sizeof(SmartHome::SensorStateReportPayload)) {
                logf("WARN", "STATE_REPORT Laenge ungueltig fuer %s", nodeId);
                return;
            }
            {
                const SmartHome::SensorStateReportPayload& state = *reinterpret_cast<const SmartHome::SensorStateReportPayload*>(payload);
                nodeStates[nodeIndex].temp_01c = state.temp_01c;
                nodeStates[nodeIndex].hum_01pct = state.hum_01pct;
                nodeStates[nodeIndex].lux = state.lux;
                nodeStates[nodeIndex].motion = (state.motion != 0U);
                nodeStates[nodeIndex].fault = (state.fault != 0U);
            }
            break;

        case SH_CLASS_BAT_SEN:
            if (payloadLen != sizeof(SmartHome::BatteryStateReportPayload)) {
                logf("WARN", "STATE_REPORT Laenge ungueltig fuer %s", nodeId);
                return;
            }
            {
                const SmartHome::BatteryStateReportPayload& state = *reinterpret_cast<const SmartHome::BatteryStateReportPayload*>(payload);
                nodeStates[nodeIndex].battery_pct = state.battery_pct;
                nodeStates[nodeIndex].battery_mv = state.battery_mv;
                nodeStates[nodeIndex].window_open = state.window_open;
                nodeStates[nodeIndex].rain_raw = state.rain_raw;
                nodeStates[nodeIndex].button_flags = state.button_flags;
                nodeStates[nodeIndex].fault = (state.fault != 0U);
            }
            break;

        default:
            logf("WARN", "STATE_REPORT ohne Handler fuer %s", nodeId);
            return;
    }

    nodeStates[nodeIndex].state_bekannt = true;
    publishNodeState((size_t)nodeIndex);
    logf("INFO", "STATE_REPORT von %s verarbeitet", nodeId);
}

void verarbeiteEventReport(const uint8_t* senderMac, const SmartHome::EventReportPayload& payload) {
    int nodeIndex = findeNodeIndex(payload.node_id);
    if (nodeIndex < 0) {
        logf("WARN", "EVENT ignoriert: unbekannte node_id=%s", payload.node_id);
        return;
    }

    aktualisiereNodeKontakt((size_t)nodeIndex, senderMac);
    publishNodeEvent((size_t)nodeIndex, payload);
    logf("INFO", "EVENT von %s: type=%u", payload.node_id, (unsigned)payload.event_type);
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
            verarbeiteStateReport(senderMac, payload, header->payload_len);
            break;

        case SH_MSG_EVENT:
            if (header->payload_len == sizeof(SmartHome::EventReportPayload)) {
                verarbeiteEventReport(senderMac, *reinterpret_cast<const SmartHome::EventReportPayload*>(payload));
            }
            break;

        case SH_MSG_ACK:
            if (header->payload_len == sizeof(SmartHome::AckPayload)) {
                verarbeiteAck(senderMac, *reinterpret_cast<const SmartHome::AckPayload*>(payload));
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

bool sendeStateRequest(size_t nodeIndex) {
    if (!nodeStates[nodeIndex].mac_bekannt) {
        logf("WARN", "STATE_REQUEST verworfen: MAC fuer %s unbekannt", NODE_DEFINITIONS[nodeIndex].node_id);
        return false;
    }

    SmartHome::CmdPayload payload = {};
    payload.cmd_type = SH_CMD_STATE_REQUEST;
    return sendePaket(nodeStates[nodeIndex].mac, SH_MSG_CMD, &payload, sizeof(payload), "STATE_REQUEST");
}

bool sendeRelayCommand(size_t nodeIndex, uint8_t relayIndex, bool relayState, const char* requestId) {
    if (!nodeStates[nodeIndex].mac_bekannt) {
        logf("WARN", "COMMAND_SET_RELAY verworfen: MAC fuer %s unbekannt", NODE_DEFINITIONS[nodeIndex].node_id);
        return false;
    }

    SmartHome::CmdPayload payload = {};
    payload.cmd_type = SH_CMD_SET_RELAY;
    payload.param1 = relayIndex;
    payload.param2 = relayState ? 1U : 0U;

    if (!nutztCommandAckPfad(nodeIndex)) {
        return sendePaket(nodeStates[nodeIndex].mac, SH_MSG_CMD, &payload, sizeof(payload), "COMMAND_SET_RELAY");
    }

    uint8_t seq = 0U;
    const bool erfolgreich = sendePaketMitOptionen(
        nodeStates[nodeIndex].mac,
        SH_MSG_CMD,
        &payload,
        sizeof(payload),
        "COMMAND_SET_RELAY",
        SH_FLAG_ACK_REQUEST,
        false,
        0U,
        &seq);

    if (!erfolgreich) {
        return false;
    }

    pendingCommand = {};
    pendingCommand.aktiv = true;
    pendingCommand.node_index = nodeIndex;
    pendingCommand.seq = seq;
    pendingCommand.relay_index = relayIndex;
    pendingCommand.relay_state = relayState;
    pendingCommand.versuche = 1U;
    pendingCommand.letztes_senden_ms = millis();
    copyText(pendingCommand.request_id, sizeof(pendingCommand.request_id), requestId);
    return true;
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    char json[256] = {0};
    size_t copyLen = length;
    if (copyLen >= sizeof(json)) copyLen = sizeof(json) - 1U;
    memcpy(json, payload, copyLen);
    json[copyLen] = '\0';

    char nodeId[SH_DEVICE_ID_LEN] = {0};
    bool istSet = false;
    bool istGet = false;
    if (!extrahiereNodeCommandTopic(topic, nodeId, sizeof(nodeId), &istSet, &istGet)) {
        logf("WARN", "MQTT Topic ignoriert: %s", topic);
        return;
    }

    int nodeIndex = findeNodeIndex(nodeId);
    if (nodeIndex < 0) {
        logf("WARN", "MQTT Command fuer unbekannte node_id=%s", nodeId);
        return;
    }

    logf("INFO", "MQTT empfangen %s -> %s", topic, json);

    char cmd[32] = {0};
    jsonHoleString(json, "cmd", cmd, sizeof(cmd));

    if (istGet) {
        if (strcmp(cmd, "state") != 0 && strcmp(cmd, "get_state") != 0) {
            logf("WARN", "MQTT cmd/get ignoriert fuer %s", nodeId);
            return;
        }

        sendeStateRequest((size_t)nodeIndex);
        return;
    }

    if (!istRelayNode((size_t)nodeIndex)) {
        logf("WARN", "MQTT set_relay ignoriert: %s ist kein Relais-Node", nodeId);
        return;
    }

    if (strcmp(cmd, "set_relay") != 0) {
        logf("WARN", "MQTT cmd/set ignoriert fuer %s", nodeId);
        return;
    }

    char requestId[REQUEST_ID_LEN] = {0};
    const bool ackPfadAktiv = nutztCommandAckPfad((size_t)nodeIndex);
    if (ackPfadAktiv) {
        if (!jsonHoleString(json, "request_id", requestId, sizeof(requestId))) {
            snprintf(requestId, sizeof(requestId), "master_%s_%lu", nodeId, millis());
            logf("WARN", "MQTT cmd/set ohne request_id fuer %s, fallback=%s", nodeId, requestId);
        }

        pruefeCommandAckCacheAblauf();

        if (pendingCommand.aktiv && pendingCommand.node_index == (size_t)nodeIndex) {
            if (gleicheRequestId(pendingCommand.request_id, requestId)) {
                logf("WARN", "Duplikat request_id waehrend Pending ignoriert: %s", requestId);
                return;
            }

            sendeCommandAckMqtt(
                (size_t)nodeIndex,
                requestId,
                "busy",
                -2,
                SH_MSG_CMD,
                pendingCommand.seq,
                pendingCommand.versuche,
                "master_busy",
                false,
                false);
            logf("WARN", "Neues Kommando fuer %s verworfen, solange request_id=%s offen ist", nodeId, pendingCommand.request_id);
            return;
        }

        if (commandAckCache.gueltig &&
            commandAckCache.node_index == (size_t)nodeIndex &&
            gleicheRequestId(commandAckCache.request_id, requestId)) {
            publishNodeAckPayload((size_t)nodeIndex, commandAckCache.ack_payload);
            logf("INFO", "Duplikat request_id erkannt, cached ACK erneut publiziert: %s", requestId);
            return;
        }
    }

    bool relayState = false;
    if (jsonHoleBool(json, "relay_1", &relayState)) {
        sendeRelayCommand((size_t)nodeIndex, 0U, relayState, requestId);
        return;
    }

    if (NODE_DEFINITIONS[nodeIndex].device_class == SH_CLASS_NET_ZRL &&
        jsonHoleBool(json, "relay_2", &relayState)) {
        sendeRelayCommand((size_t)nodeIndex, 1U, relayState, requestId);
        return;
    }

    logf("WARN", "MQTT set_relay ohne unterstuetzten Kanal fuer %s", nodeId);
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
    mqttClient.setBufferSize(384);
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

    char willTopic[96] = {0};
    char willPayload[192] = {0};
    baueMasterTopic("status", willTopic, sizeof(willTopic));
    baueMasterStatusJson(willPayload, sizeof(willPayload), false);

    bool verbunden = mqttClient.connect(
        DEVICE_ID,
        MQTT_USER,
        MQTT_PASSWORD,
        willTopic,
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

    if (!mqttClient.subscribe(MQTT_TOPIC_CMD_SET_SUB)) {
        logf("WARN", "MQTT Subscribe fehlgeschlagen: %s", MQTT_TOPIC_CMD_SET_SUB);
    }
    if (!mqttClient.subscribe(MQTT_TOPIC_CMD_GET_SUB)) {
        logf("WARN", "MQTT Subscribe fehlgeschlagen: %s", MQTT_TOPIC_CMD_GET_SUB);
    }

    publishBekannteNodesNachReconnect();
}

void pruefeCommandAckTimeout() {
    pruefeCommandAckCacheAblauf();

    if (!pendingCommand.aktiv) return;

    if ((millis() - pendingCommand.letztes_senden_ms) < COMMAND_ACK_TIMEOUT_MS) {
        return;
    }

    if ((pendingCommand.versuche - 1U) < COMMAND_MAX_RETRIES) {
        if (wiederholePendingCommand()) {
            return;
        }
    }

    sendeCommandAckMqtt(
        pendingCommand.node_index,
        pendingCommand.request_id,
        "timeout",
        (int)SH_ERROR_ACK_TIMEOUT,
        SH_MSG_CMD,
        pendingCommand.seq,
        pendingCommand.versuche,
        "master_retry_timeout",
        true,
        true);

    logf(
        "WARN",
        "ACK Timeout fuer %s request_id=%s seq=%u nach %u Versuchen",
        NODE_DEFINITIONS[pendingCommand.node_index].node_id,
        pendingCommand.request_id,
        (unsigned)pendingCommand.seq,
        (unsigned)pendingCommand.versuche);

    pendingCommand = {};
}

void pruefeOfflineTimeout() {
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        if (!nodeStates[i].online) continue;

        if ((millis() - nodeStates[i].letzter_kontakt_ms) > NODE_DEFINITIONS[i].offline_timeout_ms) {
            nodeStates[i].online = false;
            publishNodeStatus(i, false);
            logf("WARN", "Node %s nach Timeout offline", NODE_DEFINITIONS[i].node_id);
        }
    }
}

void gibStartmeldungAus() {
    if (!DEBUG_LOKAL_AKTIV) return;

    Serial.println("================================");
    Serial.println(PROJECT_NAME);
    Serial.print(DATEI_GERAET);
    Serial.print(" v");
    Serial.println(DATEI_VERSION);
    Serial.print("FW: ");
    Serial.println(PROJECT_VERSION);
    Serial.println("Bekannte Basisnodes:");
    for (size_t i = 0; i < NODE_COUNT; ++i) {
        Serial.print(" - ");
        Serial.print(NODE_DEFINITIONS[i].node_id);
        Serial.print(" (");
        Serial.print(NODE_DEFINITIONS[i].device_type);
        Serial.println(")");
    }
    Serial.println("MQTT: master/<id>/(status|event) und node/<id>/(meta|status|state|event)");
    Serial.println("================================");
}

void setup() {
    if (DEBUG_LOKAL_AKTIV) {
        Serial.begin(115200);
        delay(150);
    }

    masterStatus = {};
    initialisiereNodeStates();
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
    pruefeCommandAckTimeout();
    pruefeOfflineTimeout();
    delay(LOOP_INTERVAL_MS);
}
