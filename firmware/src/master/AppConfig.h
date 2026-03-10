#pragma once

// ============================================================
// Master – Gerätekonfiguration vor dem Upload
// ============================================================
// Diese Datei enthält alle Einstellungen, die vor dem Flashen
// geprüft und ggf. angepasst werden müssen.
//
// Private Zugangsdaten (WLAN, MQTT) stehen in Secrets.h,
// die nicht ins Repository gehört (liegt in .gitignore).
// Vorlage: firmware/include/Secrets.example.h
// ============================================================

// Geräte-ID des Masters.
// Format: MASTER-001 (keine Sensorbestückung in der ID)
constexpr char DEVICE_ID[]    = "MASTER-001";

// Anzeigename für MQTT-meta und Node-RED-Dashboard.
// Darf nach Inbetriebnahme über CFG geändert werden.
constexpr char DEVICE_NAME[]  = "Master";

// Firmware-Variante (für BuildInfo und MQTT-meta).
constexpr char FW_VARIANT[]   = "master";

// Debug-Stand: in frühen Testständen aktiv lassen.
// Für Produktionseinsatz auf false setzen.
constexpr bool DEVICE_DEBUG_AKTIV = true;

// Fester WLAN-/ESP-NOW-Kanal.
// Muss mit dem Heimnetz-Kanal übereinstimmen.
// Kanal 1, 6 oder 11 bevorzugen (keine Kanalüberlappung).
// Dieser Wert wird beim Verbindungsaufbau erzwungen.
constexpr int  WLAN_KANAL = 6;

// Reconnect-Takt für WLAN und MQTT.
constexpr unsigned long WIFI_RECONNECT_INTERVAL_MS = 10000UL;
constexpr unsigned long MQTT_RECONNECT_INTERVAL_MS = 5000UL;

// Node gilt nach ausbleibenden HEARTBEATs als offline.
constexpr unsigned long NODE_OFFLINE_TIMEOUT_MS = 15000UL;

// Takt der Hauptschleife.
constexpr unsigned long LOOP_INTERVAL_MS = 10UL;
