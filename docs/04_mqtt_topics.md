# MQTT-Vertrag

## Grundregeln

- MQTT-Praefix bleibt `smarthome/`
- Nodes sprechen weiterhin kein MQTT
- physische Geraete werden technisch nur durch den Master bekannt gemacht
- `status` und `state` bleiben strikt getrennt
- Retain nur fuer `meta`, `status`, `state`, `cfg/report`
- kein Retain fuer `event`, `ack`, `cmd/*`, `cfg/set`

## Verbindliche Topics

- `smarthome/server/status`
- `smarthome/master/<master_id>/status`
- `smarthome/master/<master_id>/event`
- `smarthome/node/<node_id>/meta`
- `smarthome/node/<node_id>/status`
- `smarthome/node/<node_id>/state`
- `smarthome/node/<node_id>/event`
- `smarthome/node/<node_id>/ack`
- `smarthome/node/<node_id>/cmd/set`
- `smarthome/node/<node_id>/cmd/get`
- `smarthome/node/<node_id>/cfg/set`
- `smarthome/node/<node_id>/cfg/report`

## Rollen

- Master publiziert `master/*`, `node/*/meta`, `node/*/status`, `node/*/state`, `node/*/event`, `node/*/ack`, `node/*/cfg/report`
- Server publiziert `server/status`, `node/*/cmd/set`, `node/*/cmd/get`, `node/*/cfg/set`
- Dashboard und Automationen gehen immer ueber den Server und damit ueber MQTT

## V1-Hinweise

- `status` transportiert technische Betriebsqualitaet
- `state` transportiert fachlichen Zustand
- `cfg/report` ist die bestaetigte Geraetekonfiguration
- `ack` transportiert die transiente Bestaetigung eines konkreten Kommandopfads; fuer `cmd/set` darf der Payload im aktuellen V1-Pilotpfad `request_id`, `ack_seq`, `ack_msg_type`, `status`, `attempts` und `retry_count` enthalten
- `smarthome/server/status` wird in SQLite unter `server_settings` gespiegelt
- unbekannte `master_id` und `node_id` werden serverseitig automatisch als Geraet angelegt
- `display_name` startet immer mit der MQTT-ID und wird erst durch echte Metadaten ueberschrieben
- `event` und `ack` werden nicht als retained-artiger Dauerzustand behandelt
- retained `event` oder retained `ack` sind ausserhalb des Vertrags und werden nur auditiert
- Influx bekommt nur numerische Zeitreihen aus `smarthome/node/<node_id>/state`
- ein altes `smarthome/master/status` ohne `<master_id>` gehoert nicht zum V1-Vertrag und wird nicht als gueltiger Master-Status behandelt
- Wetter bleibt in V1 nur eine serverseitige Vorbereitung und hat noch keinen produktiven Topic-Baum
