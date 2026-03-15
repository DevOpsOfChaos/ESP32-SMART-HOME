# Changelog

## 0.2.1 – 2026-03-10 – NET-ZRL-Basis korrigiert

### Korrektur der ZRL-Basis
- harte Relaisverriegelung aus der allgemeinen NET-ZRL-Basis entfernt
- Rolladen-Schutzlogik jetzt sauber an `COVER_MODUS_AKTIV` gebunden
- `SH_CMD_COVER` wird bei deaktiviertem Cover-Modus bewusst ignoriert
- Zwei-Relais-Basis ist damit wieder für Doppelschaltaktor und zwei Lichtkreise nutzbar

### Versionskonsistenz bereinigt
- `firmware/include/ProjectVersion.h` auf `0.2.1` angehoben
- `firmware/include/BuildConfig.h` auf `0.2.1` angehoben
- `firmware/src/net_zrl/main.cpp` auf `0.2.1` angehoben

## 0.2.0 – 2026-03-10 – Protokoll und Firmware-Basis

### Neues Protokoll (ShProtocol)
- `firmware/lib/ShProtocol/src/Protocol.h` vollständig neu geschrieben
- 10-Byte-Header: Magic, Protokollversion, msg_type, seq, flags,
  payload_len, CRC16
- Acht Nachrichtentypen: HELLO, HELLO_ACK, STATE, EVENT, CMD, CFG, ACK, TIME
- Feste Payload-Strukturen für Verwaltungsnachrichten
- TLV-Feld-IDs für STATE (>20 Felder) und EVENT (>10 Felder)
- EVENT-Typen vollständig definiert
- CFG-Parameter nach Gruppen strukturiert
- Hilfsfunktionen: fillHeader, isValidHeader, calcCrc16, isValidDeviceId,
  safeCopyDeviceId, isValidMac, macToString

### Neue Gerätetyp-Definitionen (ShProtocol)
- `firmware/lib/ShProtocol/src/DeviceTypes.h` erstellt
- Geräteklassen SH_CLASS_*, Fähigkeits-Bitmasks SH_CAP_*, Power-Typen

### Firmware-Gerüste verbessert
- Master `main.cpp`: RegistryEntry-Struktur, vollständiger ESP-NOW-Dispatch,
  Availability-Check mit Power-Typ-Timeout, TIME-Verteilung
- NET-ERL: Relaissteuerung mit Schaltgrund, Tasterentprellung, EVENT-Dispatch
- NET-ZRL: Doppelrelais-Verriegelung, Reversier-Sperrzeit, Cover-Dispatch
- NET-SEN: Differenzmeldeschwellen, PIR-Flankenauswertung
- BAT-SEN: WakeReason-Enum, Reed-Kontakt, Schlafsystem-Placeholder

### AppConfig.h und PinConfig.h für alle Geräte verbessert
- Alle Zeitwerte mit Begründungen kommentiert
- Sicherheitsrelevante Hinweise (Rolladen-Verriegelung) explizit

### Server-Infrastruktur erweitert
- `server/config/mosquitto/mosquitto.example.conf` erstellt
- `server/config/docker-compose.example.yml` mit Volumes verbessert
- `server/config/.env.example` mit TZ-Variable und Kommentaren
- `server/flows/README.md` mit Flow-Tabellen und offenen Entscheidungen
- `server/db/README.md` mit schematischen Tabellenentwürfen
- `server/tools/README.md` mit Skript-Übersicht

### Dokumentation
- `PROTOKOLL/beta01_protokoll_und_firmware_basis.txt` angelegt

---

## 0.1.0 – 2026-03-10 – Neubasis angelegt
- öffentliches Repo-Gerüst für die neue Smart-Home-Basis aufgesetzt
- Architektur- und Codevorgaben eingebunden
- Altbestand sauber in Referenz vs. Verwerfen zerlegt
- Server-Struktur als neue, leere Basis ergänzt
- PROTOKOLL-Ordner und Git-/Secrets-Regeln ergänzt
- lokale Arbeitsgrundlagen vorbereitet
