# GITHUB_REPO_TEXT

## Kurze Repo-Beschreibung

Lokales ESP32-Smart-Home: ESP-NOW-Nodes, ESP32-C3-Master als einzige MQTT-Bridge, Server-Stack mit Mosquitto, Node-RED, InfluxDB und SQLite.

## Laengere Repo-Zusammenfassung

Dieses Repo enthaelt die aktive Firmware fuer `master`, `net_erl`, `net_zrl`, `net_sen` und `bat_sen` sowie die lokale Serverbasis unter Docker Compose. Nodes sprechen nur ESP-NOW; MQTT existiert nur zwischen `master` und `server`, wobei der Master die einzige Bridge ist. Die Doku trennt belegte Pilotpfade und reale Nachweise klar von offenen Punkten wie `bat_sen`-Hardwarebeleg und weiterem Mehrgeraetenachweis.

## Vorgeschlagene Feature-/Status-Bullets

- ESP-NOW nur auf den Nodes; MQTT nur zwischen `master` und `server`
- `master` auf ESP32-C3 als einzige Bridge zwischen Funknetz und Server
- Firmware-Basis fuer `master`, `net_erl`, `net_zrl`, `net_sen`, `bat_sen`
- Server-Stack mit Mosquitto, Node-RED, InfluxDB und SQLite
- Reale Pilotnachweise fuer `net_erl_01` und `net_zrl_01` ueber den offiziellen Serverpfad
- Reeller `net_sen_01`-Servernachweis mit DHT22 auf `GPIO6`
- `bat_sen` bisher nur als Buildbasis, ohne eigenen realen Hardware-Nachweis
- `migration/` und Night Pipeline nur fuer kontrollierte Legacy-Extraktion, nicht fuer den Produktpfad

## Optionale Repository-Topics

- `esp32`
- `esp-now`
- `mqtt`
- `platformio`
- `node-red`
- `mosquitto`
- `influxdb`
- `sqlite`
- `smarthome`
- `iot`
- `docker-compose`
