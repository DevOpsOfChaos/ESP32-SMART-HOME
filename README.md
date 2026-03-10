# SmartHome ESP32 – Neubasis

Öffentliche Neubasis für das Technikerprojekt.

## Worum es hier geht
Dieses Repository ist **nicht** die Fortsetzung des alten Projektstands.
Es ist die neue, veröffentlichbare Arbeitsbasis für:

- Firmware der vier Basisgeräte
- Master-Bridge auf ESP32-C3
- spätere Serverseite auf Raspberry Pi
- Dokumentation, Testplanung und Änderungsprotokolle
- saubere GitHub- und Secrets-Trennung

## Projektprinzip
Das System bleibt strikt in drei Ebenen getrennt:

1. **Nodes**
   - sprechen nur ESP-NOW
   - kennen weder MQTT noch Node-RED
   - führen lokale Grundlogik aus

2. **Master**
   - ESP32-C3
   - Bridge zwischen ESP-NOW und MQTT
   - Registry, ACK/Retry, Availability, Konfigurationsweitergabe

3. **Server**
   - MQTT, Node-RED, spätere Visualisierung und Automationen
   - kein direkter Zugriff auf Nodes

## Feste Basisgeräte
- `master`
- `net_erl`
- `net_zrl`
- `net_sen`
- `bat_sen`

Sondergeräte werden nicht als neue Grundarchitekturen gebaut, sondern aus diesen Basisklassen abgeleitet.

## Repository-Struktur
- `docs/` Architektur, Protokoll, Regeln, Testplan, Altbestand-Zerlegung
- `firmware/` PlatformIO-Firmware und gemeinsame Bibliotheken
- `server/` neue Server-Basis für Raspberry Pi
- `hardware/` Hardwareunterlagen und Varianten
- `tools/` Git-, Prüf- und Hilfsskripte
- `PROTOKOLL/` Versionen, Probleme, Entscheidungen und Tests
- `private/` lokale, nicht veröffentlichte Dateien

## Öffentliche / private Trennung
Dieses Repository darf **keine** echten Zugangsdaten enthalten.

Verwendetes Muster:
- `Secrets.example.h` -> öffentliche Vorlage
- `Secrets.h` -> lokal, nicht versioniert
- `.env.example` -> öffentliche Vorlage
- `.env` -> lokal, nicht versioniert

## Arbeitsregel
Altcode darf nur als Referenz für:
- Pinbelegungen
- Sensoransteuerung
- Sicherheitslogik
- bekannte Fehlerquellen
- organisatorische Ideen

verwendet werden.

Er darf **nicht** als neue Basis hineinkopiert werden.

## Einstieg
1. `docs/01_architektur.md`
2. `docs/02_codevorgabe.md`
3. `docs/03_protokoll.md`
4. `docs/04_mqtt_topics.md`
5. `docs/08_altbestand_zerlegung.md`
6. `docs/11_implementierungsphasen.md`

## Arbeitsregeln fuer dieses Repository
- Code-Arbeitsordner lokal: `C:\Users\mries\Documents\Playground\smarthome-esp32`
- OneDrive dient nur fuer Doku, Begleitdateien, Exporte und Backups
- GitHub ist das zentrale Remote fuer Code und oeffentliche Projektdaten
- Keine Secrets im Repo: keine Passwoerter, Tokens, WLAN-Daten oder lokalen Zugangsdaten committen
- Aenderungen sauber dokumentieren
- Jede relevante Optimierung als neue Version mit Protokolleintrag behandeln
