# Protokoll

## Ziel
Ein kleines, einheitliches und erweiterbares ESP-NOW-Protokoll für alle Basisgeräte.

## Implementierungsstatus
- Definitionen in `firmware/lib/ShProtocol/src/Protocol.h`
- Geräteklassen und Fähigkeiten in `firmware/lib/ShProtocol/src/DeviceTypes.h`
- Minimalstrecke `net_erl_01 <-> master` ist seit `beta03` real implementiert
- Breitere Laufzeitimplementierung für weitere Geräte bleibt offen

## Feste Nachrichtentypen
- `HELLO`     (0x01) – Node-Anmeldung bei Boot / periodisch
- `HELLO_ACK` (0x02) – Master-Bestätigung mit Kanalinfo
- `STATE`     (0x03) – Aktueller Gerätezustand (TLV)
- `EVENT`     (0x04) – Eingetretenes Ereignis (TLV)
- `CMD`       (0x05) – Befehl Master -> Node
- `CFG`       (0x06) – Konfigurationsänderung Master -> Node
- `ACK`       (0x07) – Quittung (beide Richtungen)
- `TIME`      (0x08) – Zeitabgleich Master -> Nodes
- `HEARTBEAT` (0x09) – zyklischer Lebensnachweis Node -> Master

## Header (10 Bytes, alle Pakete)
| Feld       | Bytes | Inhalt |
|---|---|---|
| magic      | 1 | 0xA5 (Paketkennzeichnung) |
| proto_ver  | 1 | 1 (aktuell) |
| msg_type   | 1 | SH_MSG_* |
| seq        | 1 | 0..255 (Duplikaterkennung) |
| flags      | 1 | SH_FLAG_* Bitmask |
| _reserved  | 1 | 0 |
| payload_len| 2 | Payload-Länge in Bytes |
| crc16      | 2 | CRC16 über Header + Payload |

Verfügbare Payload-Bytes: 250 – 10 = **240 Bytes max.**

## Feste Payload-Größen
| Typ      | Payload-Bytes |
|---|---|
| HELLO    | 58 |
| HELLO_ACK| 4 |
| HEARTBEAT| 20 |
| CMD      | 4 |
| CFG      | 4 |
| ACK      | 4 |
| TIME     | 8 |
| STATE    | 20 in der Minimalstrecke, später variabel/TLV |
| EVENT    | variabel, TLV |

## TLV-Regel
`STATE` und `EVENT` sind TLV-basiert:
- `type`   (1 Byte) – SH_TLV_STATE_* oder SH_TLV_EVENT_*
- `length` (1 Byte) – Länge des Wertefelds
- `value`  (n Bytes) – Rohdaten (little-endian)

Vorteile:
- neue Felder ohne neues Paketlayout ergänzbar
- Sondergeräte brauchen keine eigene Paketfamilie
- Protokoll bleibt kontrolliert erweiterbar

## ACK-Strategie
- Jede relevante Nachricht trägt SH_FLAG_ACK_REQUEST im flags-Feld
- ACK referenziert die Sequenznummer der Originalnachricht
- Retries begrenzt auf MAX_RETRIES (AppConfig.h je Gerät, default 3)
- Duplikate werden erkannt: eingehende seq mit letztem bekannten seq vergleichen
- Retry-Timeout: ACK_TIMEOUT_MS (AppConfig.h je Gerät, default 250 ms)

## Geräte-ID-Format
Erlaubte Formate:
- `NET-ERL-001` (11 Zeichen)
- `NET-ZRL-001`
- `NET-SEN-001`
- `BAT-SEN-001`
- `MASTER-001` (10 Zeichen)

Regeln:
- Nur Großbuchstaben, Ziffern, Bindestrich
- Suffix: genau drei Ziffern
- Keine Sensorbestückung in der ID

## Offene Entscheidungen (vor Phase 2 festlegen)
- String-Übertragung für CFG_DEVICE_NAME (Sonderprotokoll)
- Verschlüsselung: Schlüsselslots vorgesehen, Aktivierung in Phase 6
- Konkrete Retry-Limits je Geräteklasse abschließend justieren
