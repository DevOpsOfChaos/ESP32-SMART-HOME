# Server Flows V1

Die technische Wahrheit fuer die laufende Node-RED-Basis liegt in:

- `server/nodered/build-flows.js`

Der Generator erzeugt die produktive `flows.json` beim Image-Build.
Wenn lokale Broker-Authentifizierung aktiviert ist, erzeugt `server/nodered/entrypoint.sh` beim Containerstart aus `MQTT_USERNAME`, `MQTT_PASSWORD` und `NODERED_CREDENTIAL_SECRET` die laufzeitseitige `flows_cred.json`.

Nicht Teil des versionierten Repo-Bestands:

- `server/nodered/_generated_v1_flows.tmp.json`

## V1-Bloecke

- MQTT-Ingest fuer `server/status`, `master/*` und `node/*`
- automatische Registry in SQLite
- getrennte Speicherung von `status`, `state`, `event` und `ack`
- `cfg/report` nach SQLite
- schmaler `cmd/set`-MQTT-Egress fuer `net_erl_01` ueber versionierte Node-RED-Trigger (`inject` und technische Dashboard-Buttons)
- Sensorpunkte nach InfluxDB
- Dashboard-Queries fuer Geraete, Raeume, Wetter, Automationen und Audit

## Ingest-Semantik

- `smarthome/server/status` schreibt nach `server_settings`
- unbekannte Master und Nodes werden bei gueltigen MQTT-Nachrichten automatisch angelegt
- `display_name` startet mit der MQTT-ID und wird nur durch echte Metadaten sinnvoll ersetzt
- retained `event` und retained `ack` werden nur auditiert und nicht in Registry oder Last-State uebernommen
- Influx bekommt nur numerische Sensorwerte aus `node/.../state`, bevorzugt aus `state.sensors`
- reale Root-Felder wie `temp_01c`, `hum_01pct`, `battery_pct`, `battery_mv` und `rain_raw` werden ebenfalls als Zeitreihe erkannt
- pro Messwert entsteht genau ein `smarthome_sensor`-Punkt mit `metric`-Tag und `value_num`
- boolesche Aktorflags, Statusfelder und textuelle Werte werden verworfen

## Grenzen

- `cmd/get` und `cfg/set` sind im aktuellen Flow-Generator noch nicht als versionierter Egress verdrahtet
- keine Home-Assistant-Abhaengigkeit
- keine Fake-Command-Semantik pro Geraet
- keine komplexen Automationsmonster in V1
