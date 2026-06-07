# Release Notes

## Public English release preparation

This release converts the project into a cleaner public-facing repository.

### What changed

- root documentation rewritten in English
- hardware, firmware, server, and ESPHome entry points cleaned up
- device-specific readmes translated where they matter for reuse
- legacy German reference material moved into `docs/archive/`
- public repo structure clarified around independently usable modules
- runtime secrets, caches, and generated state excluded from the main tree

### What is included

- custom ESP32-C3 firmware
- ESPHome alternative line for Home Assistant users
- local MQTT, Node-RED, and SQLite server stack
- hardware designs, fabrication exports, and enclosures
- verification scripts and contract checks

### What is deliberately excluded

- private school hand-in documents
- runtime-only Node-RED state
- local secrets
- build caches
- older working backups and draft material

### Suggested first commit message

`chore: prepare public English release of ESP32 smart home project`

