# PROJECT_CONTEXT

## Rolle dieser Datei
Stabiler Projektrahmen fuer neue Chats. Keine Taskliste, kein Arbeitsjournal und kein Nachweisdokument.

## Projektziel
Aufbau eines modularen, lokal betriebenen Smart-Home-Systems fuer das Technikerprojekt mit klarer Trennung zwischen Funkknoten, Master-Bridge und Server.

## Verbindliche Architekturgrenzen
- Nodes sind Feldgeraete auf ESP32-Basis.
- Nodes sprechen ausschliesslich ESP-NOW.
- Der Master ist die einzige Bridge zwischen ESP-NOW und MQTT.
- Der Server ist getrennt und uebernimmt Broker, Automatisierung, Visualisierung und spaetere Auswertung.
- MQTT bleibt ausserhalb der Nodes.

## Basisgeraete
- `master`
- `net_erl`
- `net_zrl`
- `net_sen`
- `bat_sen`

## Git-, OneDrive- und Remote-Regeln
- Das Git-Repo liegt lokal unter `C:\Users\mries\Documents\Playground\smarthome-esp32`.
- OneDrive dient nur fuer Doku, ZIP-Backups, Exporte und Begleitdateien.
- GitHub ist das zentrale Remote fuer Code und oeffentliche Projektdaten.
- Aktive Entwicklungsarbeit findet nicht in OneDrive statt.

## Public-Repo-Regeln
- Keine Passwoerter im Repo.
- Keine Tokens im Repo.
- Keine WLAN-Daten im Repo.
- Keine privaten Zugangsdaten im Repo.
- Fuer Secrets nur Example-Dateien versionieren, zum Beispiel `Secrets.example.h` oder `.env.example`.

## Verbindliche Dokumentrollen
- `AGENTS.md`: Arbeitsanweisung und Lesereihenfolge fuer Codex/ChatGPT
- `docs/README.md`: Doku-Index
- `docs/PROJECT_CONTEXT.md`: stabiler Projektrahmen
- `docs/CURRENT_SPRINT.md`: aktueller Fokus
- `docs/TASK_QUEUE.md`: naechste reale Arbeitspakete
- `docs/DECISIONS.md`: dauerhafte Entscheidungen
- `docs/06_testplan.md`: Soll-Pruefplan
- `docs/14_test_und_nachweisstand.md`: belegter Ist-Stand
- `PROTOKOLL/`: historische Einzelstaende

## Offizielles Projektgedaechtnis fuer neue Chats
- `AGENTS.md`
- `docs/PROJECT_CONTEXT.md`
- `docs/CURRENT_SPRINT.md`
- `docs/TASK_QUEUE.md`
- `docs/DECISIONS.md`

Aktuelle Test- und Statusaussagen gehoeren nicht hierher, sondern nach `docs/14_test_und_nachweisstand.md`.
