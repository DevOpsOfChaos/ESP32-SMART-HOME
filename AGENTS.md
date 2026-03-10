# AGENTS.md

## Rolle dieser Datei
Zentrale Arbeitsanweisung fuer Codex im Repo `C:\Users\mries\Documents\Playground\smarthome-esp32`.
Ziel ist, dass neue Tasks meist nur noch aus der konkreten Einzelaufgabe bestehen.

## Projektzweck
- Neubasis fuer das Smart-Home-Technikerprojekt.
- Oeffentliches, GitHub-taugliches Repository.
- Aktive Entwicklung nur lokal im Repo unter `Documents\Playground`; keine privaten Daten im Repo.

## Pflichtlektuere pro Task
1. `AGENTS.md`
2. `README.md`
3. `docs/README.md`
4. `docs/PROJECT_CONTEXT.md`
5. `docs/CURRENT_SPRINT.md`
6. `docs/TASK_QUEUE.md`
7. `docs/DECISIONS.md`
8. Falls Status, Tests oder reale Nachweise relevant sind: `docs/14_test_und_nachweisstand.md`
9. Falls im Scope: Bereichs-`AGENTS.md` in `firmware/`, `server/` oder `docs/`

## Verbindliche Doku-Rollen
- `AGENTS.md`, `docs/PROJECT_CONTEXT.md`, `docs/CURRENT_SPRINT.md`, `docs/TASK_QUEUE.md` und `docs/DECISIONS.md` bilden das offizielle Projektgedaechtnis fuer neue Chats.
- `docs/README.md` ist der Doku-Index, nicht die Quelle fuer Projektentscheidungen.
- `docs/06_testplan.md` beschreibt nur das Soll.
- `docs/14_test_und_nachweisstand.md` beschreibt nur den belegten Ist-Stand.
- `PROTOKOLL/` bleibt die historische Chronik einzelner Stande und wird nicht rueckwirkend geglaettet.
- `CHANGELOG.md` ist die verdichtete Repo-Historie, kein Nachweisdokument.

## Architekturgrenzen
- Nodes sprechen nur ESP-NOW.
- Master ist die einzige Bridge zwischen ESP-NOW und MQTT.
- Server bleibt getrennt; kein direkter Node-Zugriff.
- Kein MQTT in Nodes.
- Altbestand nur als Referenz nutzen, nie blind uebernehmen.
- `net_zrl` bleibt allgemeine Zwei-Relais-Basis; Cover-Logik nur im Cover-Modus.
- Keine Speziallogik anderer Geraete in die `net_erl`-Minimalstrecke ziehen.

## Arbeitsweise fuer Codex
- Erst lesen, dann aendern.
- Kleine, klar abgegrenzte Aufgaben bevorzugen.
- Nur die fuer die Aufgabe noetigen Dateien anfassen.
- Keine unnoetigen Refactorings.
- Keine Sammelumbauten ohne klaren Nutzen.
- Konservativ aendern statt Architektur neu zu erfinden.
- Keine stillen Architekturwechsel und keine grossen Umbenennungen ohne echten Grund.
- Den Worktree sauber hinterlassen: keine eigenen Debug-Artefakte, keine halbfertigen Nebenbaustellen, keine zufaelligen Formatierwellen.

## Dokumentationspflicht
- Relevante Aenderungen knapp dokumentieren.
- Neue Beta-Staende in `PROTOKOLL/` nach bestehendem Schema pflegen.
- `docs/CURRENT_SPRINT.md` und `docs/TASK_QUEUE.md` nur aendern, wenn sich realer Fokus oder Prioritaeten verschieben.
- `docs/DECISIONS.md` nur fuer dauerhafte Architektur- oder Arbeitsentscheidungen erweitern.
- Doku nicht aufblasen.
- Keine Fake-Tests, keinen Fake-Fortschritt und nichts als erledigt markieren, was nicht real erledigt ist.

## Repo- und Security-Regeln
- Keine Secrets, Tokens, Passwoerter, WLAN-Daten, Mail-Zugangsdaten oder sonstige private Konfigurationsdaten committen.
- Fuer sensible Konfigurationen nur Example-Dateien versionieren, zum Beispiel `Secrets.example.h`, `.env.example` oder `docker-compose.example.yml`.
- OneDrive ist nicht das aktive Repo.
- Das aktive Repo liegt lokal unter `C:\Users\mries\Documents\Playground\smarthome-esp32`.
- GitHub ist das zentrale Remote.

## Standardvorgehen pro Task
1. Pflichtdateien und ggf. Bereichs-`AGENTS.md` lesen.
2. Aufgabe mit minimalem Eingriff umsetzen.
3. Relevante dokumentierte Checks ausfuehren, soweit real moeglich.
4. Ergebnis knapp zusammenfassen: Was geaendert wurde; welche Dateien geaendert wurden; was bewusst nicht umgesetzt wurde; welche Tests gelaufen sind; welche naechsten Schritte sinnvoll sind.

## Builds und Tests
- Wenn fuer den betroffenen Bereich dokumentierte Checks existieren, diese nach Aenderungen ausfuehren.
- Repo-Check bei Repo-, Struktur- oder Doku-Arbeiten: `powershell -ExecutionPolicy Bypass -File .\tools\check_public_repo.ps1`
- Firmware-Builds ueber `firmware/platformio.ini` mit dem betroffenen Environment ausfuehren.
- Keine Tests erfinden, die nicht wirklich gelaufen sind.
- Wenn etwas nicht getestet werden konnte, klar den Grund nennen.

## Verbote
- Keine neue Architektur ohne klaren Auftrag.
- Keine privaten Daten committen.
- Keine grossen Umbenennungen ohne echten Grund.
- Keine Altordner heimlich als aktive Basis reaktivieren.
- Keine Speziallogik anderer Geraete in die `net_erl`-Minimalstrecke hineinziehen.
