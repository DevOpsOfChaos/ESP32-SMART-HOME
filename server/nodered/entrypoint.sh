#!/bin/sh
set -eu

# Ensure the SQLite schema exists before Node-RED starts.
SQLITE_TARGET="${SQLITE_PATH:-/data/sqlite/smarthome.sqlite}"
SQLITE_DIR="$(dirname "$SQLITE_TARGET")"

mkdir -p "$SQLITE_DIR"
sqlite3 "$SQLITE_TARGET" < /opt/smarthome/schema.sql

if [ -n "${MQTT_USERNAME:-}" ] && [ -n "${MQTT_PASSWORD:-}" ]; then
  node <<'EOF'
const crypto = require("crypto");
const fs = require("fs");

const credentialPath = "/data/flows_cred.json";
const credentials = {
  cfg_mqtt_broker: {
    user: process.env.MQTT_USERNAME,
    password: process.env.MQTT_PASSWORD
  }
};

if (process.env.NODERED_CREDENTIAL_SECRET) {
  const key = crypto.createHash("sha256").update(process.env.NODERED_CREDENTIAL_SECRET).digest();
  const initVector = crypto.randomBytes(16);
  const cipher = crypto.createCipheriv("aes-256-ctr", key, initVector);
  const payload =
    initVector.toString("hex") +
    cipher.update(JSON.stringify(credentials), "utf8", "base64") +
    cipher.final("base64");

  fs.writeFileSync(credentialPath, JSON.stringify({ $: payload }));
} else {
  fs.writeFileSync(credentialPath, JSON.stringify(credentials));
}
EOF
else
  rm -f /data/flows_cred.json
fi

exec /usr/src/node-red/entrypoint.sh "$@"
