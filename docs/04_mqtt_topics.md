# MQTT-Vertrag

## 1. Zweck

Dieses Dokument definiert den verbindlichen MQTT-Vertrag zwischen Master und Server.
Nodes kennen diesen Vertrag nicht direkt; sie sprechen nur ESP-NOW mit dem Master.

## 2. Grundregeln

- MQTT-Praefix bleibt `smarthome/`.
- Alle Vertragsnachrichten sind UTF-8-JSON-Objekte.
- Physische Geraete werden technisch nur durch den Master bekannt gemacht.
- `status` und `state` bleiben getrennt.
- `status` beschreibt technische Erreichbarkeit und Betriebsqualitaet.
- `state` beschreibt den fachlichen Zustand eines Geraets oder einer Kontextquelle.
- `event` beschreibt ein eingetretenes Ereignis, nicht den aktuellen Dauerzustand.
- `command` und `config` werden vom Server publiziert und vom Master konsumiert.
- `ack` wird vom Master publiziert und bestaetigt Verarbeitung oder Ablehnung.
- Retained Messages werden nur fuer aktuelle Stammdaten und Dauerzustaende genutzt.
- QoS 1 ist der Standard fuer alle Vertragsnachrichten.

## 3. Topic-Baum

### 3.1 Master

- `smarthome/master/status`
- `smarthome/master/event`
- `smarthome/master/command`
- `smarthome/master/ack`

### 3.2 Physische Nodes

- `smarthome/node/<device_id>/meta`
- `smarthome/node/<device_id>/status`
- `smarthome/node/<device_id>/state`
- `smarthome/node/<device_id>/event`
- `smarthome/node/<device_id>/command`
- `smarthome/node/<device_id>/config`
- `smarthome/node/<device_id>/ack`

### 3.3 Servereigene Kontextquellen

Aktuell verbindlich:
- `smarthome/server/weather/status`
- `smarthome/server/weather/state`
- `smarthome/server/weather/event`

Regel:
Servereigene Quellen duerfen nicht unter `smarthome/node/...` publiziert werden.

## 4. Topic-Matrix

| Topic | Publisher | Konsumenten | Retain | Zweck |
|---|---|---|---|---|
| `smarthome/master/status` | Master | Dashboard, Logging, Automationen | ja | technische Gesundheit des Masters |
| `smarthome/master/event` | Master | Logging, Dashboard | nein | Neustarts, Fehler, Registry-Ereignisse |
| `smarthome/master/command` | Server | Master | nein | Infrastrukturkommandos an den Master |
| `smarthome/master/ack` | Master | Server | nein | Antwort auf `master/command` |
| `smarthome/node/<device_id>/meta` | Master | Registry, Dashboard | ja | Stammdaten, Capabilities, Klassifikation |
| `smarthome/node/<device_id>/status` | Master | Registry, Dashboard, Automationen | ja | Online/Fault/Batterie/RSSI/Config-Revision |
| `smarthome/node/<device_id>/state` | Master | State Store, Dashboard, Historisierung | ja | fachlicher Ist-Zustand |
| `smarthome/node/<device_id>/event` | Master | Logging, Automationen, Dashboard | nein | diskrete Ereignisse |
| `smarthome/node/<device_id>/command` | Server | Master | nein | Aktorbefehle und Zustandsanfragen |
| `smarthome/node/<device_id>/config` | Server | Master | nein | Soll-Konfigurationen und Provisioning |
| `smarthome/node/<device_id>/ack` | Master | Server | nein | Antwort auf `command` oder `config` |
| `smarthome/server/weather/status` | Server | Dashboard, Automationen | ja | Gesundheitszustand der Wetterquelle |
| `smarthome/server/weather/state` | Server | Dashboard, Automationen, Historisierung | ja | aktueller Wetterkontext |
| `smarthome/server/weather/event` | Server | Logging | nein | Fetch-Fehler, Providerwechsel, Grenzwert-Events |

## 5. Payload-Grundregeln

### 5.1 Gemeinsame Felder

Wo sinnvoll, gelten diese Felder:
- `schema`
  Versionskennung des Payload-Typs, zum Beispiel `sh.node.state.v1`
- `ts`
  Zeitstempel in UTC als ISO-8601-String
- `device_id`
  nur bei `smarthome/node/<device_id>/...`; muss zum Topic passen
- `request_id`
  Pflicht bei `command`, `config` und `ack`
- `source`
  Quelle innerhalb des Systems, zum Beispiel `master`, `dashboard`, `automation`, `weather`

### 5.2 Feldregeln

- Bool-Werte sind echte JSON-Booleans, keine Strings.
- Numerische Felder tragen ihre Einheit im Namen, zum Beispiel `_c`, `_pct`, `_ppm`, `_dbm`.
- Freitextfelder bleiben kurz und maschinenlesbar.
- MQTT-Topics enthalten keine semantischen Sonderfaelle pro Einzelgeraet.
- Unbekannte Felder duerfen ignoriert, aber nicht umgedeutet werden.

## 6. Payload-Typen

### 6.1 `meta`

Verantwortung:
Nur der Master publiziert `meta`.

Pflichtinhalt:
- `device_id`
- `device_class`
- `display_name`
- `power_class`
- `fw_version`
- `hw_variant`
- `capabilities`
- `master_id`

Beispiel:

```json
{
  "schema": "sh.node.meta.v1",
  "ts": "2026-03-10T18:00:00Z",
  "device_id": "net_erl_01",
  "device_class": "net_erl",
  "display_name": "Flurlicht",
  "power_class": "mains",
  "fw_version": "0.3.0",
  "hw_variant": "rev_a",
  "capabilities": ["relay_1", "button_1", "temperature", "humidity", "lux"],
  "master_id": "master_01"
}
```

### 6.2 `status`

Verantwortung:
`status` beschreibt technische Gesundheit und Erreichbarkeit.
`status` ist nicht der fachliche Schalt- oder Sensorzustand.

Typische Felder fuer Nodes:
- `online`
- `fault`
- `fault_code`
- `rssi_dbm`
- `battery_pct`
- `last_seen`
- `config_revision_applied`

Typische Felder fuer den Master:
- `online`
- `wifi_connected`
- `mqtt_connected`
- `espnow_ready`
- `fw_version`

### 6.3 `state`

Verantwortung:
`state` beschreibt den fachlichen Ist-Zustand.

Empfohlene Struktur:
- `actuators`
- `sensors`
- `derived`

Beispiel:

```json
{
  "schema": "sh.node.state.v1",
  "ts": "2026-03-10T18:00:03Z",
  "device_id": "net_erl_01",
  "actuators": {
    "relay_1": true
  },
  "sensors": {
    "temperature_c": 21.4,
    "humidity_pct": 48.1,
    "lux": 12
  },
  "derived": {
    "last_trigger": "manual_button"
  }
}
```

### 6.4 `event`

Verantwortung:
`event` beschreibt punktuelle Ereignisse.

Pflichtinhalt:
- `event_type`
- `severity`
- `data`

Beispiele fuer `event_type`:
- `button_pressed`
- `motion_detected`
- `contact_opened`
- `relay_changed`
- `cover_started`
- `cover_stopped`
- `sensor_fault`
- `node_restarted`

### 6.5 `command`

Verantwortung:
Commands werden vom Server publiziert.
Nur der Master konsumiert sie und uebersetzt sie fuer das Zielgeraet.

Pflichtinhalt:
- `request_id`
- `command`
- `args`
- `source`

Beispiel:

```json
{
  "schema": "sh.node.command.v1",
  "ts": "2026-03-10T18:00:10Z",
  "request_id": "cmd-20260310-0001",
  "device_id": "net_erl_01",
  "command": "set_relay",
  "args": {
    "channel": "relay_1",
    "value": true
  },
  "source": "dashboard"
}
```

### 6.6 `config`

Verantwortung:
Config wird serverseitig als Soll-Zustand gepflegt und ueber MQTT zum Master gebracht.

Pflichtinhalt:
- `request_id`
- `profile_id`
- `revision`
- `values`

Beispiel:

```json
{
  "schema": "sh.node.config.v1",
  "ts": "2026-03-10T18:01:00Z",
  "request_id": "cfg-20260310-0007",
  "device_id": "net_erl_01",
  "profile_id": "hall_light_v1",
  "revision": 3,
  "values": {
    "auto_mode_enabled": true,
    "light_threshold_on_lux": 40,
    "auto_off_delay_s": 90
  },
  "source": "automation"
}
```

### 6.7 `ack`

Verantwortung:
Nur der Master publiziert `ack`.

Pflichtinhalt:
- `request_id`
- `kind`
- `status`
- `message`

Zulaessige `kind`-Werte:
- `command`
- `config`
- `master_command`

Zulaessige `status`-Werte:
- `accepted`
- `applied`
- `rejected`
- `timeout`
- `error`

Beispiel:

```json
{
  "schema": "sh.node.ack.v1",
  "ts": "2026-03-10T18:00:11Z",
  "request_id": "cmd-20260310-0001",
  "device_id": "net_erl_01",
  "kind": "command",
  "status": "accepted",
  "message": "queued_for_espnow",
  "source": "master"
}
```

## 7. Verbindliche Kommandos nach Basistyp

| Typ | Erwartete Commands |
|---|---|
| `net_erl` | `set_relay`, `toggle_relay`, `request_state`, `identify` |
| `net_zrl` | `set_relay`, `cover_move`, `cover_stop`, `cover_set_position`, `request_state`, `identify` |
| `net_sen` | `request_state`, `identify` |
| `bat_sen` | `request_state`, `identify` |
| `master` | `republish_registry`, `republish_state`, `sync_time`, `reload_config` |

Nicht jeder Command muss in der ersten Umsetzung schon verfuegbar sein.
Der Vertrag legt nur den serverseitigen Zielraum fest.

## 8. Verbindliche Zustandsregeln

- `meta`, `status` und `state` sind retained.
- `event`, `command`, `config` und `ack` sind nicht retained.
- Wenn ein Geraet offline geht, wird `status.online=false` retained aktualisiert.
- Der letzte bekannte `state` bleibt erhalten, bis ein neuer fachlicher Zustand vorliegt oder das Geraet bewusst entfernt wird.
- Eine bewusste Geraeteentfernung darf Retained Topics leeren, aber nicht ein normales Offline verwechseln.

## 9. Wetter als servereigene Kontextquelle

Wetter ist kein `node`.
Deshalb gelten hier eigene serverseitige Topics.

`smarthome/server/weather/status`:
- Provider erreichbar
- letzte erfolgreiche Aktualisierung
- Datenalter

`smarthome/server/weather/state`:
- Temperatur
- Luftfeuchte
- Wind
- Regen
- Cloud-Cover
- optional Forecast-Kurzsicht

## 10. Verantwortlichkeiten

- Nur der Master publiziert unter `smarthome/node/<device_id>/meta`.
- Nur der Master publiziert `status`, `state`, `event` und `ack` fuer physische Geraete.
- Der Server publiziert unter Node-Topics nur `command` und `config`.
- Der Server publiziert eigene Kontextdaten nur unter `smarthome/server/...`.
- Nodes publizieren niemals direkt auf MQTT.
