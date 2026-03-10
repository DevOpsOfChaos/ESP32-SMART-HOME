# AGENTS.md

Gilt fuer `server/`. Root-`AGENTS.md` bleibt bindend.

## Server-Grenzen
- Server bleibt logisch getrennt von Nodes; keine direkte Node-Kommunikation.
- MQTT endet am Master; der Server arbeitet oberhalb davon.
- Keine Alt-Flows, Alt-Compose-Dateien oder Spezialstrukturen blind uebernehmen.
- MQTT-Themen und Payload-Annahmen mit `../docs/04_mqtt_topics.md` abgleichen.

## Konfigurationsregeln
- In `config/` nur Example-Dateien und oeffentliche Defaults committen.
- Keine echten `.env`, Passwoerter, Tokens, Mail-Zugangsdaten oder Broker-Secrets ins Repo.
- Lokale Pfade, Hosts und Volumes nicht als allgemeine Defaults tarnen.
- Docker- und Compose-Aenderungen reproduzierbar und generisch halten, nicht auf ein Einzelsetup zuschneiden.

## Checks
- Geaenderte YAML-, JSON- oder Env-Beispiele auf Syntax und Plausibilitaet pruefen, soweit mit Bordmitteln real moeglich.
- Nicht gestartete Container, Broker oder Flows nicht als getestet ausgeben.
