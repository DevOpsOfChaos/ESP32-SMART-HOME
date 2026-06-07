# Tests

This folder contains the repository's smoke checks and contract checks.

## What is covered

- server startup checks
- MQTT / SQLite contract validation
- device-flow regression checks
- ESPHome configuration validation helpers

## Where to start

- `tests\server\server_contract_smoke.ps1` for the main server contract check
- `tests\server\phase1_ingest_checkliste.md` for the manual ingest path
- `tests\server\minimalserver_smoke_checkliste_2026-04-24.md` for the minimal server checklist

## Scope

These tests are meant to catch broken setup states early. They are not a full hardware validation suite.
