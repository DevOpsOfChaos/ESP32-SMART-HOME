# Server Basisarchitektur V1

## Dienste

- Mosquitto als MQTT-Broker
- Node-RED als Integrations- und Dashboard-Schicht
- InfluxDB fuer Sensor-Zeitreihen
- SQLite fuer operative Serverdaten

## Broker-Betrieb V1

- `server/config/mosquitto/mosquitto.conf` bleibt die einzige versionierte Broker-Vorlage
- der Repo-Default bleibt anonym startbar, damit keine lokalen Zugangsdaten ins Repo muessen
- lokale Passwortpflicht wird nur ueber ignorierte Dateien unter `server/config/mosquitto/config/` aktiviert
- MQTT-Vertrag, Topic-Baum und Payload-Semantik bleiben dabei unveraendert

## Datenfluss

1. Master publiziert unter `smarthome/master/<master_id>/...` und `smarthome/node/<node_id>/...`
2. der Server publiziert seinen Betriebszustand unter `smarthome/server/status`
3. Node-RED normalisiert genau diese Topics in `scope`, `entityId`, `channel` und Payload
4. unbekannte Master und Nodes werden sofort in SQLite angelegt
5. `status`, `state`, `event` und `ack` bleiben in SQLite getrennt
6. `cfg/report` wird als bestaetigte Geraetekonfiguration in `device_config` abgelegt
7. numerische Sensorwerte aus `node/.../state.sensors` werden als einzelne `smarthome_sensor`-Punkte nach InfluxDB geschrieben
8. Dashboard liest aus SQLite und Live-Datenstroemen

## Realer Nachweisstand dieser Architektur

- im Repo real belegt: MQTT-Ingest, SQLite-Ablage, Influx-Schreibpfad, offizieller Node-RED-Rueckweg fuer `net_erl_01`, SQLite-Audit fuer ausgehende `cmd/set`-Publishes und kombinierter Live-Lauf im aktuellen Repo-Stand
- der belegte Command-/Rueckweg ist bewusst kein generischer Multi-Device-Baukasten, sondern nur der enge `net_erl_01`-Pilotpfad
- offen bleiben ACK/Retry, Offline-Timeout, Langzeitstabilitaet, weitere Basisgeraete und ein vollstaendiger Gesamtprojektnachweis

## V1-Grenzen

- keine Zusatzdienste ausser Mosquitto, Node-RED und InfluxDB
- keine Home-Assistant-Abhaengigkeit
- keine produktive Wetterlogik
- keine komplexen Automationen
- keine konkurrierende Server-Timeout-Logik fuer Node-Onlinezustand; der Master bleibt autoritativ
- kein Legacy-Support fuer `smarthome/master/status` ohne `<master_id>`
