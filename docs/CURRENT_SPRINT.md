# CURRENT_SPRINT

## Sprintziel
Dokumentations- und Nachweisbasis der bestehenden Repo-Basis stabilisieren, bevor weitere Features gebaut werden.

## Im Fokus
- offizielles Projektgedaechtnis fuer neue Chats eindeutig machen
- Testplan, Nachweisstand, Historie und Bereichsdoku sauber trennen
- reale Doku-Luecken fuer Build/Flash, End-to-End-Nachweis und Bereichsdoku sichtbar halten

## Aktuell nicht im Fokus
- neue Firmware-Features
- neue Server-Features
- MQTT-Vertragsaenderungen
- Architekturumbauten

## Realer Projektstand, auf dem dieser Sprint aufsetzt
- `master`, `net_erl`, `net_zrl`, `net_sen` und `bat_sen` liegen als Code-/Buildbasis im Repo
- realer Hardware-Basisnachweis ist aktuell fuer `master <-> net_erl` dokumentiert
- die Server-V1 hat getrennte Nachweise fuer MQTT-Ingest und Influx-Schreibpfad
- ein konsolidierter realer Gesamt-Nachweis `Node -> Master -> MQTT/Server -> Master -> Node` bleibt offen

## Sprint-Abnahme
1. Ein neuer Chat findet Einstieg und Projektgedaechtnis ohne Rueckfragen.
2. `docs/06_testplan.md` und `docs/14_test_und_nachweisstand.md` widersprechen sich nicht.
3. Offene Doku-Luecken sind explizit benannt statt versteckt.
