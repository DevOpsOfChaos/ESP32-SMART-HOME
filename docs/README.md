# Docs-Index

## Rolle dieser Datei
Zentraler Index fuer die Projektdoku. Diese Datei ordnet die Dokumente ein, ersetzt aber nicht das Projektgedaechtnis.

## Zuerst lesen
1. `../AGENTS.md`
2. `../README.md`
3. `PROJECT_CONTEXT.md`
4. `CURRENT_SPRINT.md`
5. `TASK_QUEUE.md`
6. `DECISIONS.md`
7. `14_test_und_nachweisstand.md`, wenn Status oder Testaussagen relevant sind

## Verbindliches Projektgedaechtnis
- `PROJECT_CONTEXT.md`: stabiler Projektrahmen, Repo-Grenzen, Dokumentrollen
- `CURRENT_SPRINT.md`: aktueller Fokus
- `TASK_QUEUE.md`: naechste reale Arbeitspakete
- `DECISIONS.md`: dauerhafte Entscheidungen mit Grund

## Fachdoku
- `00_projektueberblick.md`: kurzer Projektabriss
- `01_architektur.md`: Zielarchitektur und harte Grenzen
- `03_protokoll.md`: ESP-NOW-Protokoll und Paketregeln
- `04_mqtt_topics.md`: MQTT-Vertrag
- `05_hardware_geraete.md`: Basisgeraete und Sondergeraete
- `15_hardware_pinstandard.md`: verbindlicher Pinstandard

## Test und Nachweis
- `06_testplan.md`: Soll-Pruefplan, keine Ist-Aussagen
- `14_test_und_nachweisstand.md`: belegter Ist-Stand
- `14_minimalstrecke_net_erl_master.md`: technischer Ablauf und Detailblatt der Minimalstrecke, kein globales Nachweisdokument

## Historie
- `../PROTOKOLL/`: historische Einzelstaende, inklusive damaliger Testaussagen
- `../CHANGELOG.md`: verdichtete Versionshistorie des Repos

## Bereichsdoku
- `../server/README.md` und `../server/docs/`: Server-Bereichsdoku
- `../server/db/README.md`: Datenbasis des Servers
- `../server/flows/README.md`: Flow-Rolle und Ingest-Semantik
- `../hardware/*/README.md`: geraetespezifische Hardwareunterlagen
- `../tools/*/README.md`: Werkzeugrollen
- `../firmware/test/*/README.md`: Testordner-Rollen

## Offene Doku-Luecken
- reproduzierbarer Build-/Flash-/Seriell-Ablauf fuer `master` und `net_erl`
- konsolidierter realer End-to-End-Nachweis `Node -> Master -> MQTT/Server -> Master -> Node`
- Hardware-README-Dateien enthalten bisher nur Platzhalter
- Server-Start und lokaler Betriebsablauf sind nur als Kurzstart, nicht als belastbares Runbook dokumentiert
