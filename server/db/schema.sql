PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS rooms (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    slug TEXT NOT NULL UNIQUE,
    name TEXT NOT NULL,
    sort_order INTEGER NOT NULL DEFAULT 0,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS devices (
    device_id TEXT PRIMARY KEY,
    device_role TEXT NOT NULL CHECK (device_role IN ('node', 'master')),
    display_name TEXT NOT NULL,
    default_name TEXT NOT NULL,
    room_id INTEGER,
    origin_master_id TEXT,
    device_class TEXT,
    hardware_type TEXT,
    firmware_version TEXT,
    protocol_version TEXT,
    power_source TEXT,
    manufacturer TEXT,
    model TEXT,
    status TEXT,
    meta_json TEXT,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL,
    first_seen_at TEXT NOT NULL,
    last_seen_at TEXT NOT NULL,
    last_meta_at TEXT,
    last_status_at TEXT,
    last_state_at TEXT,
    last_event_at TEXT,
    last_ack_at TEXT,
    FOREIGN KEY (room_id) REFERENCES rooms(id) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS device_capabilities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id TEXT NOT NULL,
    capability_key TEXT NOT NULL,
    capability_role TEXT NOT NULL DEFAULT 'unknown',
    data_type TEXT,
    unit TEXT,
    writable INTEGER NOT NULL DEFAULT 0,
    readable INTEGER NOT NULL DEFAULT 1,
    channel TEXT,
    meta_json TEXT,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL,
    UNIQUE (device_id, capability_key),
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS device_config (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    device_id TEXT NOT NULL,
    config_key TEXT NOT NULL,
    config_value TEXT,
    config_type TEXT,
    source_channel TEXT NOT NULL,
    last_reported_at TEXT,
    last_requested_at TEXT,
    updated_at TEXT NOT NULL,
    UNIQUE (device_id, config_key),
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS device_last_state (
    device_id TEXT PRIMARY KEY,
    status_json TEXT,
    status_at TEXT,
    state_json TEXT,
    state_at TEXT,
    ack_json TEXT,
    ack_at TEXT,
    event_json TEXT,
    event_at TEXT,
    updated_at TEXT NOT NULL,
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS automations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    is_enabled INTEGER NOT NULL DEFAULT 1,
    trigger_type TEXT,
    trigger_definition_json TEXT,
    action_definition_json TEXT,
    notes TEXT,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL,
    last_executed_at TEXT
);

CREATE TABLE IF NOT EXISTS audit_log (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    topic TEXT NOT NULL,
    device_id TEXT,
    master_id TEXT,
    scope TEXT NOT NULL,
    channel TEXT NOT NULL,
    direction TEXT NOT NULL,
    retain INTEGER NOT NULL DEFAULT 0,
    payload_json TEXT,
    occurred_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS weather_settings (
    id INTEGER PRIMARY KEY CHECK (id = 1),
    provider TEXT NOT NULL,
    location_label TEXT,
    latitude REAL,
    longitude REAL,
    api_base_url TEXT,
    api_key_ref TEXT,
    poll_interval_minutes INTEGER NOT NULL DEFAULT 15,
    enabled INTEGER NOT NULL DEFAULT 0,
    units TEXT NOT NULL DEFAULT 'metric',
    config_json TEXT,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL
);

CREATE INDEX IF NOT EXISTS idx_devices_room_id ON devices(room_id);
CREATE INDEX IF NOT EXISTS idx_devices_role ON devices(device_role);
CREATE INDEX IF NOT EXISTS idx_devices_last_seen ON devices(last_seen_at);
CREATE INDEX IF NOT EXISTS idx_device_capabilities_role ON device_capabilities(device_id, capability_role);
CREATE INDEX IF NOT EXISTS idx_device_config_device ON device_config(device_id);
CREATE INDEX IF NOT EXISTS idx_audit_log_occurred_at ON audit_log(occurred_at);
CREATE INDEX IF NOT EXISTS idx_audit_log_device ON audit_log(device_id);

