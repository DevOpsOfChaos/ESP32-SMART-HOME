#pragma once

/*
====================================================================
 Projekt   : SmartHome ESP32
 Datei     : Protocol.h
 Modul     : ShProtocol
 Version   : 0.2.2
 Stand     : 2026-03-10

 Funktion:
 Vollständige Protokolldefinition für die ESP-NOW-Kommunikation
 zwischen Nodes und Master.

 Dieses Protokoll ist eine Neuentwicklung.
 Es basiert NICHT auf dem alten Protokoll aus dem Altprojekt.

 Entscheidungen:
 - Kleiner gemeinsamer Header mit Magic, Protokollversion,
   Nachrichtentyp, Sequenznummer, Flags, Payload-Länge und CRC16.
 - Feste Payloads für stabile Verwaltungsnachrichten
   (HELLO, HELLO_ACK, CMD, CFG, ACK, TIME).
 - Kompakte feste STATE-/EVENT-Payloads für die vier Basisgeräte.
   Die reservierten TLV-Feld-IDs bleiben für spätere Ausbaustufen
   definiert, sind in diesem Stand aber noch nicht die aktive
   Laufzeitdarstellung.
 - Keine JSON-Payloads über ESP-NOW.
 - Einfaches Geräte-ID-Schema: NET-ERL-001, NET-ZRL-001 usw.
   Keine Sensorbestückung in der ID.

 Wichtige Einschränkung:
 - ESP-NOW-Pakete sind auf 250 Bytes begrenzt.
 - Der Header belegt 10 Bytes.
 - Für den Payload bleiben max. 240 Bytes.

 Abhängigkeiten:
 - <Arduino.h> oder <stdint.h> / <string.h>

 Offene Entscheidungen:
 - konkrete ACK-Timeouts und Retry-Limits je Geräteklasse
   (geplant in AppConfig.h je Gerät)
 - Verschlüsselungsrahmen (Schlüsselslots vorgesehen, noch nicht aktiv)
====================================================================
*/

#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

// ============================================================
// Protokollkonstanten
// ============================================================

// Magic-Wert zur schnellen Paketkennzeichnung.
// Pakete ohne diesen Wert werden am Empfänger sofort verworfen.
#define SH_PROTO_MAGIC         0xA5U

// Aktuelle Protokollversion.
// Inkompatible Änderungen erfordern eine neue Version.
// Wird im Header mitgeführt, damit zukünftige Implementierungen
// alte Pakete erkennen und kontrolliert ablehnen können.
#define SH_PROTO_VERSION       1U

// Maximale ESP-NOW-Payload-Größe laut ESP-IDF-Spezifikation.
#define SH_ESPNOW_MAX_BYTES    250U

// Größe des gemeinsamen Headers (alle Pakete).
#define SH_HEADER_SIZE         10U

// Maximal nutzbare Payload-Größe nach Abzug des Headers.
#define SH_MAX_PAYLOAD_BYTES   (SH_ESPNOW_MAX_BYTES - SH_HEADER_SIZE)

// Maximale Geräte-ID-Länge inkl. Nullterminator.
// Format: NET-ERL-001  -> 11 Zeichen + Nullterminator = 12
// Kürzer als im Altprojekt (32), weil der Name aus ID + Metadaten kommt.
#define SH_DEVICE_ID_LEN       16U

// Maximaler Anzeigename (retained MQTT meta), inkl. Nullterminator.
#define SH_DEVICE_NAME_LEN     32U

// Maximale Anzahl registrierter Geräte im Master-RAM.
#define SH_MAX_DEVICES         32U

// ============================================================
// Nachrichtentypen (msg_type im Header)
// ============================================================
//
// Reihenfolge entspricht dem typischen Kommunikationsfluss.

// Node -> Master: Anmeldung nach Boot oder periodisch.
// Enthält: Geräte-ID, Anzeigename, Fähigkeiten.
#define SH_MSG_HELLO           0x01U

// Master -> Node: Annahmebestätigung nach HELLO.
// Enthält: bestätigten WLAN-/ESP-NOW-Kanal.
#define SH_MSG_HELLO_ACK       0x02U

// Node -> Master: Aktueller Gerätezustand (TLV-Payload).
// Beantwortet: Wie ist das Gerät gerade?
#define SH_MSG_STATE           0x03U

// Node -> Master: Eingetretenes Ereignis (TLV-Payload).
// Beantwortet: Was ist gerade passiert?
#define SH_MSG_EVENT           0x04U

// Master -> Node: Steuerbefehl (z. B. Relais schalten).
// Feste Payload-Struktur.
#define SH_MSG_CMD             0x05U

// Master -> Node: Konfigurationsänderung.
// Feste Payload-Struktur.
#define SH_MSG_CFG             0x06U

// Beide Richtungen: Quittung auf empfangene Nachricht.
// Referenziert immer eine konkrete Sequenznummer.
#define SH_MSG_ACK             0x07U

// Master -> Node: Zeitabgleich.
// Ermöglicht lokale Zeitstempel auf Nodes ohne eigene RTC.
#define SH_MSG_TIME            0x08U

// Node -> Master: Periodischer Lebensnachweis.
// Hält die Minimalstrecke online und treibt den Offline-Timeout.
#define SH_MSG_HEARTBEAT       0x09U

// ============================================================
// Flags im Header (flags-Byte)
// ============================================================
//
// Bit 0: ACK erwünscht.
//   Sender erwartet ein ACK für diese Nachricht.
#define SH_FLAG_ACK_REQUEST    0x01U

// Bit 1: Retransmission.
//   Diese Nachricht ist eine Wiederholung (Duplikaterkennung).
#define SH_FLAG_RETRANSMIT     0x02U

// Bit 2: Verschlüsselung aktiv.
//   Noch nicht genutzt. Platzhalter für späteren Ausbau.
#define SH_FLAG_ENCRYPTED      0x04U

// ============================================================
// ACK-Status
// ============================================================

// Verarbeitung erfolgreich.
#define SH_ACK_OK              0x00U

// Verarbeitung fehlgeschlagen (unbekannter Typ, ungültiger Wert,
// Ressource nicht verfügbar).
#define SH_ACK_ERROR           0x01U

// Nachricht abgelehnt (unbekanntes Gerät, Sicherheitsproblem).
#define SH_ACK_REJECTED        0x02U

// ============================================================
// CMD-Subtypen (cmd_type im CMD-Payload)
// ============================================================

// Relais schalten (0 = aus, 1 = an, 2 = toggle).
#define SH_CMD_RELAY           0x01U

// Rolladen steuern (0 = stop, 1 = auf, 2 = ab, 3 = position).
#define SH_CMD_COVER           0x02U

// Sofortigen STATE anfordern (Polling).
#define SH_CMD_STATE_REQUEST   0x03U

// Neustart des Nodes auslösen (nur im Wartungskontext).
#define SH_CMD_REBOOT          0x04U

// Lesbarer Alias für die Minimalstrecke.
#define SH_CMD_SET_RELAY       SH_CMD_RELAY

// ============================================================
// CFG-Parameter-IDs (param_id im CFG-Payload)
// ============================================================
//
// Gruppen:
//   0x01..0x0F  Basisparameter
//   0x10..0x1F  Sensor- und Meldeintervalle
//   0x20..0x2F  Relais- und Lichtlogik
//   0x30..0x3F  Rolladen
//   0x40..0x4F  Batterie und Schlaf
//   0x50..0x5F  LED / WS2812
//   0xF0..0xFF  Diagnose / Wartung

// Anzeigename des Geräts (String-Parameter, Sonderbehandlung).
#define SH_CFG_DEVICE_NAME            0x01U

// Intervall für periodische STATE-Meldungen (Sekunden).
#define SH_CFG_REPORT_INTERVAL_S      0x02U

// ACK-Timeout für gesendete Nachrichten (Millisekunden).
#define SH_CFG_ACK_TIMEOUT_MS         0x03U

// Maximale Anzahl Wiederholungen vor Aufgabe.
#define SH_CFG_MAX_RETRIES            0x04U

// Entprellzeit für Events (Millisekunden).
#define SH_CFG_EVENT_DEBOUNCE_MS      0x05U

// LED aktiviert (0 = aus, 1 = an).
#define SH_CFG_LED_ENABLED            0x06U

// Temperaturdifferenz für sofortige Meldung (0.1 °C).
#define SH_CFG_TEMP_DELTA_01C         0x10U

// Feuchtigkeitsdifferenz für sofortige Meldung (0.1 %).
#define SH_CFG_HUM_DELTA_01PCT        0x11U

// Luxdifferenz für sofortige Meldung.
#define SH_CFG_LUX_DELTA              0x12U

// Präsenz-Haltezeit (Sekunden).
#define SH_CFG_PRESENCE_HOLD_S        0x13U

// Relais-Modus (0 = manuell+auto, 1 = nur auto, 2 = nur manuell).
#define SH_CFG_RELAY_MODE             0x20U

// Automatischer Abschalt-Delay (Sekunden, 0 = deaktiviert).
#define SH_CFG_AUTO_OFF_DELAY_S       0x21U

// Lichtschwellwert für Einschalt-Automatik (Lux).
#define SH_CFG_LIGHT_THRESHOLD_ON     0x22U

// Lichtschwellwert für Ausschalt-Automatik (Lux).
#define SH_CFG_LIGHT_THRESHOLD_OFF    0x23U

// Relay-Standardzustand bei Boot (0 = aus, 1 = an).
#define SH_CFG_RELAY_DEFAULT_ON_BOOT  0x24U

// Rolladen: Fahrzeit aufwärts (Millisekunden).
#define SH_CFG_COVER_RUN_UP_MS        0x30U

// Rolladen: Fahrzeit abwärts (Millisekunden).
#define SH_CFG_COVER_RUN_DOWN_MS      0x31U

// Rolladen: Sperrzeit nach Richtungswechsel (Millisekunden).
// Verhindert Richtungsumkehr ohne Pause (Motorschutz).
#define SH_CFG_COVER_REVERSE_LOCK_MS  0x32U

// Rolladen: Kalibrierung gültig (0/1).
#define SH_CFG_COVER_CALIBRATED       0x33U

// Batterie: Aufwachintervall (Sekunden).
#define SH_CFG_WAKE_INTERVAL_S        0x40U

// Batterie: Empfangsfenster nach TX (Millisekunden).
#define SH_CFG_RX_WINDOW_MS           0x41U

// Batterie: Warnung unter diesem Prozentsatz.
#define SH_CFG_LOW_BATTERY_PCT        0x42U

// WS2812-Ring: aktiviert (0/1).
#define SH_CFG_RING_ENABLED           0x50U

// WS2812-Ring: Helligkeit (0..255).
#define SH_CFG_RING_BRIGHTNESS        0x51U

// WS2812-Ring: Modus (0 = aus, 1 = AQI, 2 = Status).
#define SH_CFG_RING_MODE              0x52U

// ============================================================
// TLV-Feld-IDs für STATE
// ============================================================
//
// Gruppen:
//   0x01..0x1F  Gerätebasis
//   0x20..0x3F  Sensormesswerte
//   0x40..0x5F  Aktorstatus
//   0x60..0x7F  Diagnose

// Gerätezeitpunkt in Sekunden seit Epoche (uint32).
// Wird vom Master via TIME gesetzt.
#define SH_TLV_STATE_TIMESTAMP        0x01U

// Letzter Auslöser / Schaltgrund (uint8, SH_TRIGGER_*).
#define SH_TLV_STATE_LAST_TRIGGER     0x02U

// Fehlerstatus (uint8, SH_ERROR_*).
#define SH_TLV_STATE_ERROR            0x03U

// Temperatur in 0.1 °C, INT16_MIN = ungültig (int16).
#define SH_TLV_STATE_TEMP_01C         0x20U

// Luftfeuchtigkeit in 0.1 %, 0xFFFF = ungültig (uint16).
#define SH_TLV_STATE_HUM_01PCT        0x21U

// Lichtwert in Lux (uint16).
#define SH_TLV_STATE_LUX              0x22U

// Luftqualitäts-Index AQI 0..500 (uint16).
#define SH_TLV_STATE_AQI              0x23U

// TVOC in ppb (uint16).
#define SH_TLV_STATE_TVOC_PPB         0x24U

// eCO2 in ppm (uint16).
#define SH_TLV_STATE_ECO2_PPM         0x25U

// Bewegungserkennung aktiv (uint8, 0/1).
#define SH_TLV_STATE_MOTION           0x26U

// Fensterstatus (uint8, 0 = geschlossen, 1 = offen, 0xFF = n/a).
#define SH_TLV_STATE_WINDOW           0x27U

// Regenrohdaten ADC 0..4095, 0xFFFF = n/a (uint16).
#define SH_TLV_STATE_RAIN_RAW         0x28U

// Batteriestand in Prozent, 0xFF = n/a (uint8).
#define SH_TLV_STATE_BATTERY_PCT      0x29U

// Batterieladung in Millivolt (uint16).
#define SH_TLV_STATE_BATTERY_MV       0x2AU

// Relais 0 Zustand (uint8, 0 = aus, 1 = an).
#define SH_TLV_STATE_RELAY0           0x40U

// Relais 1 Zustand (uint8, 0 = aus, 1 = an).
#define SH_TLV_STATE_RELAY1           0x41U

// Rolladen aktuell: Position 0..100 (uint8).
#define SH_TLV_STATE_COVER_POSITION   0x42U

// Rolladen aktuell: Fahrzustand (uint8, SH_COVER_STATE_*).
#define SH_TLV_STATE_COVER_STATE      0x43U

// Rolladen: Kalibrierungsstatus (uint8, 0/1).
#define SH_TLV_STATE_COVER_CALIBRATED 0x44U

// RSSI des letzten empfangenen Pakets in dBm (int8).
#define SH_TLV_STATE_RSSI_DBM         0x60U

// Boot-Zähler aus NVS (uint32).
#define SH_TLV_STATE_BOOT_COUNTER     0x61U

// ============================================================
// TLV-Feld-IDs für EVENT
// ============================================================

// Ereignistyp (uint8, SH_EVENT_*). Pflichtfeld in jedem EVENT-Paket.
#define SH_TLV_EVENT_TYPE             0x01U

// Zeitstempel des Ereignisses (uint32).
#define SH_TLV_EVENT_TIMESTAMP        0x02U

// Betroffenes Relais (uint8, 0 oder 1).
#define SH_TLV_EVENT_RELAY_INDEX      0x03U

// Schaltgrund für Relais (uint8, SH_TRIGGER_*).
#define SH_TLV_EVENT_RELAY_TRIGGER    0x04U

// Neuer Relaiszustand (uint8, 0 = aus, 1 = an).
#define SH_TLV_EVENT_RELAY_STATE      0x05U

// Taster-Index (uint8, 0..3 für Mehrfachtaster).
#define SH_TLV_EVENT_BUTTON_INDEX     0x06U

// Tastentyp (uint8, 0 = kurz, 1 = lang).
#define SH_TLV_EVENT_BUTTON_TYPE      0x07U

// Rolladen-Zielposition 0..100 (uint8).
#define SH_TLV_EVENT_COVER_TARGET     0x08U

// Rolladen-Zielzustand (uint8, SH_COVER_STATE_*).
#define SH_TLV_EVENT_COVER_STATE      0x09U

// Sensorfehlernummer (uint8, SH_ERROR_*).
#define SH_TLV_EVENT_SENSOR_ERROR     0x0AU

// ============================================================
// EVENT-Typen (SH_TLV_EVENT_TYPE Wert)
// ============================================================

// Manueller Taster betätigt.
#define SH_EVENT_BUTTON_PRESS         0x01U

// PIR oder Radar hat Bewegung erkannt.
#define SH_EVENT_MOTION_DETECTED      0x02U

// Fenster oder Tür geöffnet.
#define SH_EVENT_WINDOW_OPENED        0x03U

// Fenster oder Tür geschlossen.
#define SH_EVENT_WINDOW_CLOSED        0x04U

// Regensensor hat Regen erkannt.
#define SH_EVENT_RAIN_DETECTED        0x05U

// Relais wurde geschaltet (manuell oder automatisch).
#define SH_EVENT_RELAY_CHANGED        0x06U

// Lampe automatisch eingeschaltet.
#define SH_EVENT_LIGHT_AUTO_ON        0x07U

// Lampe automatisch ausgeschaltet.
#define SH_EVENT_LIGHT_AUTO_OFF       0x08U

// Rolladen fährt aufwärts.
#define SH_EVENT_COVER_UP             0x09U

// Rolladen fährt abwärts.
#define SH_EVENT_COVER_DOWN           0x0AU

// Rolladen gestoppt.
#define SH_EVENT_COVER_STOP           0x0BU

// Rolladen-Kalibrierung gestartet.
#define SH_EVENT_COVER_CALIB_START    0x0CU

// Rolladen-Kalibrierung abgeschlossen.
#define SH_EVENT_COVER_CALIB_DONE     0x0DU

// Node neu gestartet.
#define SH_EVENT_NODE_BOOT            0x0EU

// Sensorfehler erkannt.
#define SH_EVENT_SENSOR_FAULT         0x0FU

// Verbindungsfehler (ACK-Ausfall nach Retry-Limit).
#define SH_EVENT_COMM_FAULT           0x10U

// Manueller Taster stabil losgelassen.
#define SH_EVENT_BUTTON_RELEASE       0x11U

// Manueller Taster hat die Long-Press-Schwelle erreicht.
#define SH_EVENT_BUTTON_LONG_PRESS    0x12U

// ============================================================
// Auslöserquellen / Schaltgründe (SH_TRIGGER_*)
// ============================================================

// Unbekannt oder nicht gesetzt.
#define SH_TRIGGER_UNKNOWN            0x00U

// Manueller lokaler Taster.
#define SH_TRIGGER_MANUAL_BUTTON      0x01U

// Befehl vom Master.
#define SH_TRIGGER_MASTER_CMD         0x02U

// Automatik (Bewegung oder Lichtschwellwert).
#define SH_TRIGGER_AUTO               0x03U

// Automatischer Abschalt-Timer abgelaufen.
#define SH_TRIGGER_AUTO_OFF_TIMER     0x04U

// Konfigurationsänderung vom Master.
#define SH_TRIGGER_CONFIG             0x05U

// ============================================================
// Rolladen-Fahrzustände (SH_COVER_STATE_*)
// ============================================================

#define SH_COVER_STATE_STOPPED        0x00U
#define SH_COVER_STATE_MOVING_UP      0x01U
#define SH_COVER_STATE_MOVING_DOWN    0x02U

// ============================================================
// Fehlercodes (SH_ERROR_*)
// ============================================================

#define SH_ERROR_NONE                 0x00U
#define SH_ERROR_SENSOR_INIT          0x01U  // Sensor konnte nicht initialisiert werden
#define SH_ERROR_SENSOR_READ          0x02U  // Sensormessung fehlgeschlagen
#define SH_ERROR_ACK_TIMEOUT          0x03U  // Keine ACK nach Retry-Limit
#define SH_ERROR_COVER_CALIB          0x04U  // Rolladen-Kalibrierungsfehler

// ============================================================
// Protokoll-Datenstrukturen (Namespace SmartHome)
// ============================================================

namespace SmartHome {

// ───────────────────────────────────────────────────────────────
// Gemeinsamer Header (10 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Aufbau:
//   magic       : 0xA5 (schnelle Erkennung, Verwurf fremder Pakete)
//   proto_ver   : Protokollversion (aktuell 1)
//   msg_type    : Nachrichtentyp (SH_MSG_*)
//   seq         : Sequenznummer 0..255 (Duplikaterkennung)
//   flags       : Steuer-Bits (SH_FLAG_*)
//   payload_len : Länge des Payloads in Bytes
//   crc16       : CRC16 über Header (Bytes 0..7) + Payload
//
// Wichtig:
//   Die CRC16 muss über den gesamten Paketinhalt gerechnet werden
//   (Header mit crc16=0 + Payload), damit spätere Implementierungen
//   einfach prüfen können.

typedef struct __attribute__((packed)) {
    uint8_t  magic;        // immer SH_PROTO_MAGIC
    uint8_t  proto_ver;    // aktuell SH_PROTO_VERSION
    uint8_t  msg_type;     // SH_MSG_*
    uint8_t  seq;          // 0..255, Duplikaterkennung
    uint8_t  flags;        // SH_FLAG_* Bitmask
    uint8_t  _reserved;    // immer 0, für spätere Nutzung
    uint16_t payload_len;  // Payload-Länge in Bytes (max. 240)
    uint16_t crc16;        // CRC16 über Header (ohne crc16-Feld) + Payload
} MsgHeader;

static_assert(sizeof(MsgHeader) == SH_HEADER_SIZE,
    "MsgHeader muss exakt SH_HEADER_SIZE Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// HELLO-Payload (feste Struktur, 58 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Einziger Pakettyp, der Strings trägt.
// Wird bei jedem Boot und periodisch gesendet.
//
// device_id   : Technische ID, z. B. "NET-ERL-001"
// device_name : Anzeigename für MQTT-meta und Dashboard
// device_class: Geräteklasse (SH_CLASS_*)
// caps        : Fähigkeiten-Bitmask (SH_CAP_*)
// fw_version  : Firmware-Version, z. B. 0x0100 = 1.0.0
// boot_counter: monoton steigend aus NVS

typedef struct __attribute__((packed)) {
    char     device_id[SH_DEVICE_ID_LEN];    // max. 15 + Nullterminator
    char     device_name[SH_DEVICE_NAME_LEN]; // max. 31 + Nullterminator
    uint8_t  device_class;  // SH_CLASS_*
    uint8_t  caps_hi;       // obere 8 Bit der Fähigkeits-Bitmask
    uint8_t  caps_lo;       // untere 8 Bit der Fähigkeits-Bitmask
    uint8_t  power_type;    // 0 = Netz, 1 = Batterie
    uint16_t fw_version;    // z. B. 0x0200 = Version 2.0
    uint32_t boot_counter;  // Boot-Zähler aus NVS
} HelloPayload;

static_assert(sizeof(HelloPayload) == 58,
    "HelloPayload muss 58 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// HELLO_ACK-Payload (feste Struktur, 4 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Bestätigt einen erfolgreichen HELLO und teilt dem Node mit,
// auf welchem WLAN-Kanal der Master dauerhaft lauscht.

typedef struct __attribute__((packed)) {
    uint8_t channel;    // 1..13, bestätigter ESP-NOW/WLAN-Kanal
    uint8_t ack_status; // SH_ACK_OK oder SH_ACK_REJECTED
    uint8_t _pad[2];    // immer 0
} HelloAckPayload;

static_assert(sizeof(HelloAckPayload) == 4,
    "HelloAckPayload muss 4 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// HEARTBEAT-Payload (feste Struktur, 20 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Minimaler Lebensnachweis des Nodes an den Master.

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    uint32_t uptime_s;
} HeartbeatPayload;

static_assert(sizeof(HeartbeatPayload) == 20,
    "HeartbeatPayload muss 20 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// CMD-Payload (feste Struktur, 4 Bytes)
// ───────────────────────────────────────────────────────────────
//
// cmd_type  : Untertyp (SH_CMD_*)
// param1/2  : Typabhängige Parameter
//
// Beispiele:
//   SH_CMD_RELAY: param1 = relay_index (0/1), param2 = 0/1/2
//   SH_CMD_COVER: param1 = SH_COVER_STATE_*, param2 = Zielposition

typedef struct __attribute__((packed)) {
    uint8_t cmd_type;  // SH_CMD_*
    uint8_t param1;    // Primärparameter
    uint8_t param2;    // Sekundärparameter (je nach cmd_type)
    uint8_t _pad;      // immer 0
} CmdPayload;

static_assert(sizeof(CmdPayload) == 4,
    "CmdPayload muss 4 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// CFG-Payload (feste Struktur, 4 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Überträgt genau einen Konfigurationswert an den Node.
// Für String-Parameter (SH_CFG_DEVICE_NAME) wird ein
// Sonderprotokoll noch definiert (offene Entscheidung).

typedef struct __attribute__((packed)) {
    uint8_t  param_id;   // SH_CFG_*
    uint8_t  _pad;       // immer 0
    uint16_t value;      // Parameterwert (0..65535)
} CfgPayload;

static_assert(sizeof(CfgPayload) == 4,
    "CfgPayload muss 4 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// ACK-Payload (feste Struktur, 4 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Quittiert eine konkrete empfangene Nachricht anhand ihrer
// Sequenznummer.
//
// ack_seq     : Sequenznummer der quittierten Nachricht
// ack_msg_type: Nachrichtentyp der quittierten Nachricht
// status      : SH_ACK_OK / SH_ACK_ERROR / SH_ACK_REJECTED

typedef struct __attribute__((packed)) {
    uint8_t ack_seq;       // Sequenznummer der Originalnachricht
    uint8_t ack_msg_type;  // msg_type der Originalnachricht
    uint8_t status;        // SH_ACK_*
    uint8_t _pad;          // immer 0
} AckPayload;

static_assert(sizeof(AckPayload) == 4,
    "AckPayload muss 4 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// TIME-Payload (feste Struktur, 8 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Verteilt die aktuelle Zeit des Masters an alle Nodes.
// Nodes nutzen diese Zeit für interne Zeitstempel in STATE/EVENT.

typedef struct __attribute__((packed)) {
    uint32_t unix_time;       // Aktuelle Zeit in Sekunden seit 1970-01-01
    int16_t  tz_offset_min;   // Zeitzone-Offset in Minuten (z. B. 60 für UTC+1)
    uint8_t  is_dst;          // 0 = kein Sommerzeit, 1 = Sommerzeit aktiv
    uint8_t  _pad;            // immer 0
} TimePayload;

static_assert(sizeof(TimePayload) == 8,
    "TimePayload muss 8 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// STATE_REPORT-Payload (feste Struktur, 20 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Minimale Zustandsmeldung für die erste Vertikalstrecke.

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    uint8_t  relay_1;
    uint8_t  fault;
    uint8_t  _pad[2];
} StateReportPayload;

static_assert(sizeof(StateReportPayload) == 20,
    "StateReportPayload muss 20 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// NET-ZRL STATE_REPORT-Payload (feste Struktur, 22 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Minimale Zwei-Relais-Zustandsmeldung für die allgemeine NET-ZRL-Basis.
// Cover-Felder bleiben nur dann fachlich relevant, wenn
// COVER_MODUS_AKTIV in der Firmware gesetzt ist.

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    uint8_t  relay_1;
    uint8_t  relay_2;
    uint8_t  cover_mode;
    uint8_t  cover_state;
    uint8_t  cover_position;
    uint8_t  fault;
} ZrlStateReportPayload;

static_assert(sizeof(ZrlStateReportPayload) == 22,
    "ZrlStateReportPayload muss 22 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// NET-SEN STATE_REPORT-Payload (feste Struktur, 24 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Generische Sensormeldung für die aktuelle NET-SEN-Basis.
// Nicht bestückte Sensoren verwenden die im Protokoll kommentierten
// Sentinel-Werte (INT16_MIN / 0xFFFF).

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    int16_t  temp_01c;
    uint16_t hum_01pct;
    uint16_t lux;
    uint8_t  motion;
    uint8_t  fault;
} SensorStateReportPayload;

static_assert(sizeof(SensorStateReportPayload) == 24,
    "SensorStateReportPayload muss 24 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// Erweiterter NET-SEN STATE_REPORT-Payload (feste Struktur, 34 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Additive Erweiterung für optionale Umgebungs- und Luftqualitätsmodule.
// Der Basispfad mit DHT22 bleibt auf dem kleineren Payload gültig.
// Nicht bestückte Werte bleiben auf ihren Sentinel-Werten:
//   pressure_pa = 0xFFFFFFFF
//   aqi/tvoc_ppb/eco2_ppm = 0xFFFF

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    int16_t  temp_01c;
    uint16_t hum_01pct;
    uint16_t lux;
    uint32_t pressure_pa;
    uint16_t aqi;
    uint16_t tvoc_ppb;
    uint16_t eco2_ppm;
    uint8_t  motion;
    uint8_t  fault;
} ExtendedSensorStateReportPayload;

static_assert(sizeof(ExtendedSensorStateReportPayload) == 34,
    "ExtendedSensorStateReportPayload muss 34 Bytes gross sein");

// ───────────────────────────────────────────────────────────────
// BAT-SEN STATE_REPORT-Payload (feste Struktur, 24 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Minimaler Batteriestatus und Event-nahe Zustände der BAT-SEN-Basis.
// Nicht vorhandene Eingänge verwenden 0xFF / 0xFFFF als "n/a".

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    uint8_t  battery_pct;
    uint16_t battery_mv;
    uint8_t  window_open;
    uint16_t rain_raw;
    uint8_t  button_flags;
    uint8_t  fault;
} BatteryStateReportPayload;

static_assert(sizeof(BatteryStateReportPayload) == 24,
    "BatteryStateReportPayload muss 24 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// EVENT_REPORT-Payload (feste Struktur, 22 Bytes)
// ───────────────────────────────────────────────────────────────
//
// Allgemeine Event-Meldung für die Basisgeräte.
// param1/param2 sind vom event_type abhängig und bleiben bewusst
// klein, damit keine neue Spezialarchitektur entsteht.

typedef struct __attribute__((packed)) {
    char     node_id[SH_DEVICE_ID_LEN];
    uint8_t  event_type;
    uint8_t  trigger;
    uint8_t  param1;
    uint16_t param2;
    uint8_t  _pad;
} EventReportPayload;

static_assert(sizeof(EventReportPayload) == 22,
    "EventReportPayload muss 22 Bytes groß sein");

// ───────────────────────────────────────────────────────────────
// TLV-Eintrag (reserviert für spätere STATE/EVENT-Ausbaustufen)
// ───────────────────────────────────────────────────────────────
//
// TLV-Felddefinitionen bleiben im Protokoll erhalten, werden in diesem
// Firmware-Stand aber noch nicht als aktive Laufzeitnutzlast kodiert.

typedef struct {
    uint8_t  type;
    uint8_t  length;
    uint8_t  value[4]; // max 4 Bytes für einfache skalare Werte
} TlvEntry;

// ============================================================
// Hilfsfunktionen
// ============================================================

/**
 * Füllt den gemeinsamen Header mit den gegebenen Werten.
 * CRC16 wird auf 0 gesetzt und muss nachträglich berechnet werden.
 *
 * Parameter:
 *   h          - Ziel-Header
 *   msg_type   - Nachrichtentyp (SH_MSG_*)
 *   seq        - Sequenznummer
 *   flags      - Steuer-Bits (SH_FLAG_*)
 *   payload_len- Payload-Länge in Bytes
 */
static inline void fillHeader(
    MsgHeader& h,
    uint8_t msg_type,
    uint8_t seq,
    uint8_t flags,
    uint16_t payload_len)
{
    h.magic       = SH_PROTO_MAGIC;
    h.proto_ver   = SH_PROTO_VERSION;
    h.msg_type    = msg_type;
    h.seq         = seq;
    h.flags       = flags;
    h._reserved   = 0;
    h.payload_len = payload_len;
    h.crc16       = 0; // Muss nach fillHeader mit calcCrc16 gesetzt werden
}

/**
 * Prüft, ob der Header formal gültig ist.
 * Prüft Magic, Protokollversion und Payload-Größe.
 *
 * Rückgabewert:
 *   true  -> Header sieht formal korrekt aus
 *   false -> Paket verwerfen
 */
static inline bool isValidHeader(const MsgHeader& h) {
    if (h.magic != SH_PROTO_MAGIC)      return false;
    if (h.proto_ver != SH_PROTO_VERSION) return false;
    if (h.payload_len > SH_MAX_PAYLOAD_BYTES) return false;
    return true;
}

/**
 * Sicheres Kopieren und Säubern einer Geräte-ID.
 * Kürzt auf SH_DEVICE_ID_LEN, erzwingt Nullterminator,
 * entfernt Sonderzeichen außerhalb des erlaubten Bereichs.
 *
 * Parameter:
 *   src - Quell-String
 *   dst - Ziel-Buffer (mindestens SH_DEVICE_ID_LEN Bytes)
 */
static inline void safeCopyDeviceId(const char* src, char* dst) {
    if (!src || !dst) return;
    strncpy(dst, src, SH_DEVICE_ID_LEN - 1);
    dst[SH_DEVICE_ID_LEN - 1] = '\0';
    // Unerlaubte Zeichen entfernen (nur A-Z, 0-9, - erlaubt)
    for (int i = 0; dst[i] != '\0'; i++) {
        char c = dst[i];
        if (!isupper((unsigned char)c) &&
            !isdigit((unsigned char)c) &&
            c != '-') {
            dst[i] = '\0';
            break;
        }
    }
}

/**
 * Prüft, ob eine Geräte-ID dem Projektstandard entspricht.
 * Erlaubte Formate:
 *   NET-ERL-001  (11 Zeichen)
 *   NET-ZRL-001
 *   NET-SEN-001
 *   BAT-SEN-001
 *   MASTER-001   (10 Zeichen)
 *
 * Regeln:
 *   - Nur Großbuchstaben, Ziffern und '-'
 *   - Suffix: genau drei Ziffern
 *   - Präfix: NET oder BAT oder MASTER
 *
 * Rückgabewert:
 *   true  -> ID ist gültig
 *   false -> ID darf nicht verwendet werden
 */
static inline bool isValidDeviceId(const char* id) {
    if (!id) return false;
    size_t len = strlen(id);

    // Längenprüfung: 10 (MASTER-001) oder 11 (NET-ERL-001)
    if (len != 10 && len != 11) return false;

    // Suffix: letzte drei Zeichen müssen Ziffern sein
    for (int i = (int)len - 3; i < (int)len; i++) {
        if (!isdigit((unsigned char)id[i])) return false;
    }

    // Trennstrich vor dem Suffix
    if (id[len - 4] != '-') return false;

    // Gesamtzeichen: nur erlaubte Zeichen
    for (size_t i = 0; i < len; i++) {
        char c = id[i];
        if (c == '-') continue;
        if (!isdigit((unsigned char)c) && !isupper((unsigned char)c)) return false;
    }

    return true;
}

/**
 * Prüft, ob eine MAC-Adresse gültig ist (weder all-zero noch all-FF).
 *
 * Parameter:
 *   mac - 6-Byte-Array
 *
 * Rückgabewert:
 *   true  -> MAC ist verwendbar
 *   false -> MAC ungültig
 */
static inline bool isValidMac(const uint8_t* mac) {
    if (!mac) return false;
    bool allZero = true;
    bool allFf   = true;
    for (int i = 0; i < 6; i++) {
        if (mac[i] != 0x00) allZero = false;
        if (mac[i] != 0xFF) allFf   = false;
    }
    return !(allZero || allFf);
}

/**
 * Formatiert eine MAC-Adresse als lesbaren String "AA:BB:CC:DD:EE:FF".
 *
 * Parameter:
 *   mac    - 6-Byte-Array
 *   buffer - Ziel-Buffer (mindestens 18 Bytes)
 */
static inline void macToString(const uint8_t* mac, char* buffer) {
    if (!mac || !buffer) return;
    snprintf(buffer, 18,
        "%02X:%02X:%02X:%02X:%02X:%02X",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/**
 * Einfache CRC16/CCITT-False-Implementierung.
 * Verwendung: Header (mit crc16=0) + Payload zusammen hashen.
 *
 * Parameter:
 *   data - Eingabedaten
 *   len  - Länge in Bytes
 *
 * Rückgabewert:
 *   CRC16-Prüfsumme
 */
static inline uint16_t calcCrc16(const uint8_t* data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    while (len--) {
        crc ^= (uint16_t)(*data++) << 8;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (uint16_t)((crc << 1) ^ 0x1021);
            } else {
                crc = (uint16_t)(crc << 1);
            }
        }
    }
    return crc;
}

/**
 * Berechnet die CRC16 für ein komplettes Paket aus Header und Payload.
 * Das crc16-Feld des Headers wird dabei intern auf 0 gesetzt.
 */
static inline uint16_t calcPacketCrc(
    const MsgHeader& header,
    const uint8_t* payload)
{
    MsgHeader temp = header;
    temp.crc16 = 0;

    uint8_t buffer[SH_ESPNOW_MAX_BYTES] = {0};
    memcpy(buffer, &temp, sizeof(MsgHeader));

    if (temp.payload_len > 0 && payload != nullptr) {
        memcpy(buffer + sizeof(MsgHeader), payload, temp.payload_len);
    }

    return calcCrc16(buffer, (uint16_t)(sizeof(MsgHeader) + temp.payload_len));
}

/**
 * Finalisiert die CRC eines ausgehenden Pakets.
 */
static inline void finalizePacketCrc(
    MsgHeader& header,
    const uint8_t* payload)
{
    header.crc16 = calcPacketCrc(header, payload);
}

/**
 * Prüft ein komplettes Paket inklusive CRC16.
 */
static inline bool hasValidPacketCrc(const uint8_t* packet, size_t len) {
    if (!packet || len < sizeof(MsgHeader)) return false;

    MsgHeader header;
    memcpy(&header, packet, sizeof(MsgHeader));

    if (!isValidHeader(header)) return false;
    if (len != (sizeof(MsgHeader) + header.payload_len)) return false;

    const uint8_t* payload = packet + sizeof(MsgHeader);
    return calcPacketCrc(header, payload) == header.crc16;
}

} // namespace SmartHome
