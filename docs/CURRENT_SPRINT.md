# CURRENT_SPRINT

## Sprintziel
Dokumentations- und Nachweisbasis nach den belegten Pilotpfaden `net_erl_01` und `net_zrl_01` repo-weit widerspruchsfrei halten, bevor weitere Features gebaut werden.

## Im Fokus
- offizielles Projektgedaechtnis fuer neue Chats eindeutig machen
- belegten `net_erl_01`-Live-, ACK/Retry-, Offline-Timeout- und Langzeitnachweis sowie den belegten `net_zrl_01`-Live-/ACK-/Retry-Nachweis eng und ehrlich von offenen Punkten trennen
- reale Doku-Luecken fuer weitere Basisgeraete, weitere Pfade und Bereichsdoku sichtbar halten

## Aktuell nicht im Fokus
- neue Firmware-Features
- neue Server-Features
- MQTT-Vertragsaenderungen
- Architekturumbauten

## Realer Projektstand, auf dem dieser Sprint aufsetzt
- `master`, `net_erl`, `net_zrl`, `net_sen` und `bat_sen` liegen als Code-/Buildbasis im Repo
- realer Hardware- und MQTT-Minimalpfad ist fuer `master <-> net_erl_01` dokumentiert
- die Server-V1 hat getrennte Nachweise fuer MQTT-Ingest, Influx-Schreibpfad, die offiziellen `net_erl_01`- und `net_zrl_01`-Rueckwege und `cmd/set`-Egress-Audit
- der kombinierte Live-Nachweis `Node-RED -> cmd/set -> audit_log egress -> Master -> net_erl -> MQTT state -> device_last_state` ist fuer den Pilotpfad `net_erl_01` belegt
- der kombinierte Live-Nachweis `Node-RED -> cmd/set -> audit_log egress -> Master -> net_zrl -> MQTT state -> device_last_state` ist fuer den Pilotpfad `net_zrl_01` belegt
- der ACK-/Retry-Nachweis fuer die offiziellen `net_erl_01`- und `net_zrl_01`-`cmd/set`-Pfade ist belegt
- der Offline-Timeout fuer `master <-> net_erl_01` ist inklusive MQTT-/SQLite-Sichtbarkeit und Wieder-online-Pfad ebenfalls belegt
- die Langzeitstabilitaet fuer `master <-> net_erl_01` ist ueber 45 Minuten, 6 offizielle serverseitige Schaltvorgaenge und ruhige Laufphasen ebenfalls belegt
- offen bleiben weitere Basisgeraete, Offline-Timeout und Langzeitstabilitaet fuer weitere Pfade und ein vollstaendiger Gesamtprojektnachweis

## Sprint-Abnahme
1. Ein neuer Chat findet Einstieg und Projektgedaechtnis ohne Rueckfragen.
2. Hauptdoku, Bereichsdoku und `docs/14_test_und_nachweisstand.md` widersprechen sich nicht.
3. Die engen belegten Pilotpfade `net_erl_01` und `net_zrl_01` und ihre offenen Grenzen sind explizit benannt statt versteckt.
