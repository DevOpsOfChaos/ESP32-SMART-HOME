# Node-RED

This folder contains the Node-RED runtime for the local server and dashboard path.

## Purpose

- `flows/active/` holds the production flow fragments
- `lib/` contains helper modules for routing, handlers, and SQL helpers
- `settings.js` is the base Node-RED settings file
- the real startup path is in `../docker-compose.yml`, where SQLite, schema preparation, `flows.json`, and runtime settings are assembled during container startup

## Active features

- MQTT ingestion
- device and master state storage
- overview and detail dashboard views
- automation persistence and runner logic

## Not part of this layer

- weather aggregation
- charts and historical analytics
- broad command-console features
- large generator or editor tooling
