#pragma once

/*
====================================================================
 Projekt   : SmartHome ESP32
 Datei     : DeviceTypes.h
 Modul     : ShProtocol
 Version   : 0.2.0
 Stand     : 2026-03-10

 Funktion:
 Geräteklassen, Fähigkeits-Bitmasks und Power-Typen.
 Diese Definitionen sind Teil des Protokolls und werden sowohl
 in Firmware (HELLO-Paket) als auch auf der Serverseite genutzt.

 Entscheidung:
 Geräteklassen beschreiben die Basisarchitektur eines Nodes.
 Sondergeräte (z. B. Rolladensteuerung) sind NET-ZRL-Geräte
 mit entsprechenden Fähigkeits-Bits – keine neue Klasse.
====================================================================
*/

// ============================================================
// Geräteklassen (device_class im HELLO-Payload)
// ============================================================

// Netzbetriebener Node mit einem Relais.
// Typisch: einfache Lampe, Schaltaktor.
#define SH_CLASS_NET_ERL    0x01U

// Netzbetriebener Node mit zwei Relais.
// Typisch: Rolladen, Außenlicht, Doppelschalter.
#define SH_CLASS_NET_ZRL    0x02U

// Netzbetriebener Sensor-Node, kein Relais.
// Typisch: Klimasensor, Luftqualität, Präsenz.
#define SH_CLASS_NET_SEN    0x03U

// Batteriebetriebener Sensor- und Event-Node.
// Typisch: Fensterkontakt, Wandschalter, Regensensor.
#define SH_CLASS_BAT_SEN    0x04U

// Master-Gerät (Bridge ESP-NOW <-> MQTT).
#define SH_CLASS_MASTER     0xFEU

// Unbekannte oder noch nicht registrierte Klasse.
#define SH_CLASS_UNKNOWN    0xFFU

// ============================================================
// Power-Typen (power_type im HELLO-Payload)
// ============================================================

// Netzbetrieben (dauerhaft verfügbar).
#define SH_POWER_MAINS      0x00U

// Batteriebetrieben (schläft zwischen Ereignissen oder periodisch).
#define SH_POWER_BATTERY    0x01U

// ============================================================
// Fähigkeits-Bitmasks (caps_hi / caps_lo im HELLO-Payload)
// ============================================================
//
// Beide Bytes zusammen ergeben eine 16-Bit-Maske.
// caps_hi enthält Bits 15..8, caps_lo enthält Bits 7..0.
//
// Bits 7..0 (caps_lo):

// Mindestens ein Relais vorhanden.
#define SH_CAP_RELAY        0x0001U

// Zwei Relais vorhanden (Rolladen, Doppelschalter).
#define SH_CAP_RELAY2       0x0002U

// Temperatursensor vorhanden.
#define SH_CAP_TEMP         0x0004U

// Feuchtigkeitssensor vorhanden.
#define SH_CAP_HUM          0x0008U

// Lichtsensor vorhanden.
#define SH_CAP_LUX          0x0010U

// Luftqualitätssensor vorhanden.
#define SH_CAP_AQI          0x0020U

// Bewegungs- oder Präsenzsensor vorhanden (PIR oder Radar).
#define SH_CAP_MOTION       0x0040U

// Fensterkontakt vorhanden (Reed-Schalter).
#define SH_CAP_WINDOW       0x0080U

// Bits 15..8 (caps_hi):

// Regensensor vorhanden.
#define SH_CAP_RAIN         0x0100U

// Batteriemessung vorhanden.
#define SH_CAP_BATTERY      0x0200U

// Lokaler Taster vorhanden (1-fach).
#define SH_CAP_BUTTON       0x0400U

// Mehrfachtaster vorhanden (2- bis 4-fach).
#define SH_CAP_MULTIBUTTON  0x0800U

// WS2812-LED-Ring vorhanden.
#define SH_CAP_LED_RING     0x1000U

// Rolladenfähigkeit vorhanden (Fahrzeit, Kalibrierung, Verriegelung).
#define SH_CAP_COVER        0x2000U

// Setup-Portal aktiv (nur für Diagnosezwecke im HELLO mitgeteilt).
#define SH_CAP_SETUP_PORTAL 0x4000U

// Reserviert für spätere Nutzung.
#define SH_CAP_RESERVED     0x8000U
