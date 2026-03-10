# Server-Überblick

Der Server ist in dieser Architektur **nicht** die Intelligenz der Feldgeräte.
Er ist zuständig für:
- MQTT
- Node-RED
- Visualisierung
- Automationen
- Logging

Direkte Kommunikation mit Nodes findet nicht statt.
Alle Wege laufen über den Master.
