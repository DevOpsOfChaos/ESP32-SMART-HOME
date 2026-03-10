# MQTT Topics

## Ziel
Neutrales MQTT-Schema ohne Altlasten aus alten Geräte- oder Dashboard-Strukturen.

## Basis-Themen
- `smarthome/master/status`
- `smarthome/<device_id>/meta`
- `smarthome/<device_id>/availability`
- `smarthome/<device_id>/state`
- `smarthome/<device_id>/event`
- `smarthome/<device_id>/ack`
- `smarthome/<device_id>/cmd`
- `smarthome/<device_id>/cfg`

## Bedeutung

### `meta`
Gerätebeschreibung und feste Eigenschaften.
Typisch retained.

### `availability`
Online-/Offline-Zustand.
Typisch retained.

### `state`
Aktueller Zustand des Geräts.
Typisch retained.

### `event`
Ereignisorientierte Meldungen.
Nicht retained.

### `ack`
Quittierungen auf Anwendungsseite.
Nicht retained.

### `cmd`
Befehle an das Gerät.
Der Master abonniert diese Themen.

### `cfg`
Konfigurationsänderungen an das Gerät.
Der Master abonniert diese Themen.

## Grundsatz
MQTT ist die Server-Schnittstelle.
Nodes kennen dieses Schema nicht direkt.
Nur der Master übersetzt zwischen ESP-NOW und MQTT.
