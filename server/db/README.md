# Server – Datenbank

Dieser Ordner enthält Datenbankschemen und Migrationsskripte.

## Geplante Struktur

Zunächst SQLite als einfacher, wartungsarmer Ansatz.
InfluxDB optional für Zeitreihendaten (Temperaturverläufe etc.).

## Schematische Tabellen (Phase 3)

```sql
-- Gerätestammdaten (aus MQTT meta)
CREATE TABLE devices (
    device_id   TEXT PRIMARY KEY,
    device_name TEXT,
    device_class TEXT,
    caps        INTEGER,
    power_type  TEXT,
    fw_version  TEXT,
    first_seen  INTEGER,
    last_seen   INTEGER
);

-- Letzter bekannter STATE je Gerät
CREATE TABLE device_state (
    device_id   TEXT PRIMARY KEY,
    payload_json TEXT,
    updated_at  INTEGER
);

-- Ereignishistorie
CREATE TABLE events (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id   TEXT,
    event_type  TEXT,
    payload_json TEXT,
    ts          INTEGER
);

-- Availability-Log
CREATE TABLE availability (
    device_id   TEXT,
    online      INTEGER,
    ts          INTEGER
);
```

## Offene Entscheidungen

- Retention: Wie lange werden Events aufbewahrt?
- Indizierung: Welche Zeitreihen brauchen InfluxDB?
- Backup-Strategie: automatisch oder manuell?
