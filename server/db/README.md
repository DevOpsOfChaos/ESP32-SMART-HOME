# Server-Datenmodell

## 1. Speichergrenzen

Die Datenhaltung ist bewusst auf zwei Systeme getrennt:

- SQLite
  operative Serverdaten: Geraete, aktuelle Zustaende, Config, Automationen, Audit
- InfluxDB
  Sensor- und Wetter-Zeitreihen

Klare Regel:
InfluxDB ist kein Ersatz fuer Registry, Config oder Audit.
SQLite ist kein Ersatz fuer Messwert-Historisierung.

## 2. SQLite-Modell

### 2.1 Ziel

SQLite ist die verbindliche Quelle fuer:
- bekannte Geraete
- Raum- und UI-Zuordnung
- aktuellen `status` und `state`
- Config-Profile und Soll-Konfigurationen
- serverseitige Automationen
- Audit und Ablaufprotokolle
- Wetterquellen-Konfiguration

### 2.2 Kernobjekte

| Tabelle | Zweck |
|---|---|
| `masters` | bekannte Master-Instanzen und ihr technischer Zustand |
| `areas` | Raeume oder logische Bereiche fuer Dashboard und Automationen |
| `devices` | physische Geraete, die der Master bekannt gemacht hat |
| `device_status_current` | letzter technischer `status` je Geraet |
| `device_state_current` | letzter fachlicher `state` je Geraet |
| `device_events` | persistierte Ereignisse aus MQTT `event` |
| `config_profiles` | wiederverwendbare Soll-Konfigurationen pro Geraeteklasse |
| `device_config_assignments` | Profilzuordnung und Sync-Status je Geraet |
| `automation_rules` | serverseitige Regeldefinitionen |
| `automation_run_log` | Ausfuehrungsprotokolle der Regeln |
| `audit_log` | Bedienungen, Commands, Config-Aenderungen, Systemaktionen |
| `external_context_sources` | servereigene Quellen wie Wetter |

### 2.3 Tabellenentwurf

```sql
CREATE TABLE masters (
    master_id            TEXT PRIMARY KEY,
    display_name         TEXT NOT NULL,
    fw_version           TEXT,
    last_status_at       TEXT,
    status_json          TEXT NOT NULL,
    updated_at           TEXT NOT NULL
);

CREATE TABLE areas (
    area_id              TEXT PRIMARY KEY,
    name                 TEXT NOT NULL,
    sort_order           INTEGER NOT NULL DEFAULT 0,
    active               INTEGER NOT NULL DEFAULT 1
);

CREATE TABLE devices (
    device_id            TEXT PRIMARY KEY,
    master_id            TEXT NOT NULL REFERENCES masters(master_id),
    area_id              TEXT REFERENCES areas(area_id),
    device_class         TEXT NOT NULL,
    display_name         TEXT NOT NULL,
    power_class          TEXT NOT NULL,
    hw_variant           TEXT,
    fw_version           TEXT,
    config_schema        TEXT,
    active               INTEGER NOT NULL DEFAULT 1,
    discovered_at        TEXT NOT NULL,
    last_meta_at         TEXT NOT NULL,
    meta_json            TEXT NOT NULL
);

CREATE TABLE device_status_current (
    device_id            TEXT PRIMARY KEY REFERENCES devices(device_id),
    online               INTEGER NOT NULL,
    fault                INTEGER NOT NULL DEFAULT 0,
    fault_code           TEXT,
    rssi_dbm             INTEGER,
    battery_pct          REAL,
    config_revision_applied INTEGER,
    last_seen_at         TEXT,
    status_json          TEXT NOT NULL,
    updated_at           TEXT NOT NULL
);

CREATE TABLE device_state_current (
    device_id            TEXT PRIMARY KEY REFERENCES devices(device_id),
    state_json           TEXT NOT NULL,
    updated_at           TEXT NOT NULL
);

CREATE TABLE device_events (
    id                   INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id            TEXT NOT NULL REFERENCES devices(device_id),
    ts                   TEXT NOT NULL,
    event_type           TEXT NOT NULL,
    severity             TEXT NOT NULL,
    event_json           TEXT NOT NULL
);

CREATE TABLE config_profiles (
    profile_id           TEXT PRIMARY KEY,
    device_class         TEXT NOT NULL,
    name                 TEXT NOT NULL,
    revision             INTEGER NOT NULL,
    values_json          TEXT NOT NULL,
    active               INTEGER NOT NULL DEFAULT 1,
    created_at           TEXT NOT NULL,
    updated_at           TEXT NOT NULL
);

CREATE TABLE device_config_assignments (
    device_id            TEXT PRIMARY KEY REFERENCES devices(device_id),
    profile_id           TEXT REFERENCES config_profiles(profile_id),
    override_json        TEXT,
    desired_revision     INTEGER NOT NULL,
    applied_revision     INTEGER,
    sync_state           TEXT NOT NULL,
    last_request_id      TEXT,
    updated_at           TEXT NOT NULL
);

CREATE TABLE automation_rules (
    automation_id        TEXT PRIMARY KEY,
    name                 TEXT NOT NULL,
    enabled              INTEGER NOT NULL DEFAULT 1,
    trigger_kind         TEXT NOT NULL,
    trigger_json         TEXT NOT NULL,
    condition_json       TEXT,
    action_json          TEXT NOT NULL,
    cooldown_s           INTEGER,
    area_id              TEXT REFERENCES areas(area_id),
    updated_at           TEXT NOT NULL
);

CREATE TABLE automation_run_log (
    id                   INTEGER PRIMARY KEY AUTOINCREMENT,
    automation_id        TEXT NOT NULL REFERENCES automation_rules(automation_id),
    started_at           TEXT NOT NULL,
    finished_at          TEXT,
    result               TEXT NOT NULL,
    trigger_context_json TEXT,
    action_count         INTEGER NOT NULL DEFAULT 0,
    message              TEXT
);

CREATE TABLE audit_log (
    id                   INTEGER PRIMARY KEY AUTOINCREMENT,
    ts                   TEXT NOT NULL,
    actor_type           TEXT NOT NULL,
    actor_id             TEXT,
    subject_type         TEXT NOT NULL,
    subject_id           TEXT,
    action               TEXT NOT NULL,
    result               TEXT NOT NULL,
    payload_json         TEXT
);

CREATE TABLE external_context_sources (
    source_id            TEXT PRIMARY KEY,
    source_type          TEXT NOT NULL,
    enabled              INTEGER NOT NULL DEFAULT 1,
    config_json          TEXT NOT NULL,
    status_json          TEXT,
    updated_at           TEXT NOT NULL
);
```

### 2.4 Modellierungsregeln

- `devices` entsteht nur aus Master-bestaetigtem `meta`.
- `device_status_current` und `device_state_current` halten nur den letzten Stand.
- Roh-JSON bleibt erhalten, damit heterogene Geraeteklassen ohne EAV-Horror abbildbar bleiben.
- Wichtige Abfragefelder wie `online`, `battery_pct` oder `config_revision_applied` liegen zusaetzlich als echte Spalten vor.
- Automationen werden bewusst als JSON-definierte Trigger, Bedingungen und Aktionen gespeichert, weil das fuer Node-RED praktikabler ist als ein uebereifrig normalisiertes DSL-Schema.

### 2.5 Empfohlene Indizes

```sql
CREATE INDEX idx_devices_area ON devices(area_id);
CREATE INDEX idx_devices_class ON devices(device_class);
CREATE INDEX idx_device_events_device_ts ON device_events(device_id, ts DESC);
CREATE INDEX idx_automation_run_log_automation_started ON automation_run_log(automation_id, started_at DESC);
CREATE INDEX idx_audit_log_subject_ts ON audit_log(subject_type, subject_id, ts DESC);
```

## 3. InfluxDB-Modell

### 3.1 Ziel

InfluxDB speichert nur normalisierte Zeitreihen:
- numerische Sensorwerte
- trendfaehige boolsche Werte als `0` oder `1`, wenn sie fuer Charts oder Auswertungen relevant sind
- Wetterwerte

Keine JSON-Blobs in InfluxDB.
Keine Registry- oder Config-Daten in InfluxDB.

### 3.2 Bucket und Measurements

Empfohlener Start:
- Bucket `smarthome`
- Measurement `device_state`
- Measurement `weather_state`

### 3.3 Tags und Felder

`device_state`
- Tags: `device_id`, `device_class`, `area_id`, `power_class`
- Felder: `temperature_c`, `humidity_pct`, `lux`, `co2_ppm`, `voc_index`, `battery_pct`, `rssi_dbm`, `position_pct`, `relay_1`, `relay_2`, `motion_active`, `contact_open`

`weather_state`
- Tags: `source_id`, `provider`
- Felder: `temperature_c`, `humidity_pct`, `wind_ms`, `wind_gust_ms`, `rain_mm_h`, `cloud_pct`, `pressure_hpa`

### 3.4 Schreibregeln

- Nur die Time-Series-Schicht schreibt nach InfluxDB.
- Es werden nur normalisierte Felder mit stabilen Namen geschrieben.
- Hohe Kardinalitaet wird vermieden; `request_id`, `fw_version` oder Freitext gehoeren nicht in Tags.
- Der Server schreibt Wetter und physische Sensorwerte nach demselben Namensschema fuer Einheiten.

Beispiel:

```text
device_state,device_id=net_sen_01,device_class=net_sen,area_id=wohnzimmer,power_class=mains temperature_c=21.4,humidity_pct=48.1,lux=132 1773165603000000000
weather_state,source_id=weather_main,provider=openmeteo temperature_c=6.4,wind_ms=3.8,cloud_pct=84 1773165603000000000
```

## 4. Basistypen im Datenmodell

| Typ | SQLite-Schwerpunkt | Influx-Schwerpunkt |
|---|---|---|
| `master` | `masters`, Audit | keiner ausser optionalen Betriebsmetriken |
| `net_erl` | Device, Status, State, Config, Audit | optionale Sensorwerte, optional Relaistrend |
| `net_zrl` | Device, Status, State, Config, Audit | Position, Zusatzsensoren, optional Relaistrend |
| `net_sen` | Device, Status, State, Audit | Hauptquelle fuer klassische Sensorzeitreihen |
| `bat_sen` | Device, Status, State, Event, Audit | Batterie und optionale Sensorwerte; Event-Historie bleibt in SQLite |

## 5. Noch offene Punkte

- konkrete Retention fuer InfluxDB
- Backup-Frequenz fuer SQLite und InfluxDB
- ob alte Audit- und Eventeintraege zyklisch archiviert oder geloescht werden
