# CURRENT_SPRINT

## Sprintziel
Dokumentations- und Nachweisbasis nach dem belegten `net_erl_01`-Pilotpfad repo-weit widerspruchsfrei halten, bevor weitere Features gebaut werden.

## Im Fokus
- offizielles Projektgedaechtnis fuer neue Chats eindeutig machen
- belegten `net_erl_01`-Live-Nachweis eng und ehrlich von offenen Punkten trennen
- reale Doku-Luecken fuer ACK/Retry, Offline-Timeout, Langzeitstabilitaet, weitere Basisgeraete und Bereichsdoku sichtbar halten

## Aktuell nicht im Fokus
- neue Firmware-Features
- neue Server-Features
- MQTT-Vertragsaenderungen
- Architekturumbauten

## Realer Projektstand, auf dem dieser Sprint aufsetzt
- `master`, `net_erl`, `net_zrl`, `net_sen` und `bat_sen` liegen als Code-/Buildbasis im Repo
- realer Hardware- und MQTT-Minimalpfad ist fuer `master <-> net_erl_01` dokumentiert
- die Server-V1 hat getrennte Nachweise fuer MQTT-Ingest, Influx-Schreibpfad, offiziellen `net_erl_01`-Rueckweg und `cmd/set`-Egress-Audit
- der kombinierte Live-Nachweis `Node-RED -> cmd/set -> audit_log egress -> Master -> net_erl -> MQTT state -> device_last_state` ist fuer den Pilotpfad `net_erl_01` belegt
- offen bleiben ACK/Retry, Offline-Timeout, Langzeitstabilitaet, weitere Basisgeraete und ein vollstaendiger Gesamtprojektnachweis

## Sprint-Abnahme
1. Ein neuer Chat findet Einstieg und Projektgedaechtnis ohne Rueckfragen.
2. Hauptdoku, Bereichsdoku und `docs/14_test_und_nachweisstand.md` widersprechen sich nicht.
3. Der enge belegte `net_erl_01`-Pilotpfad und seine offenen Grenzen sind explizit benannt statt versteckt.
