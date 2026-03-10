# Server-Basisarchitektur

## 1. Zweck

Dieses Dokument zieht die fachliche und technische Server-Basis fuer das Smart-Home-Projekt fest.
Es beschreibt die Zielarchitektur fuer die Serverseite, nicht die fertige Umsetzung.

Verbindlich in diesem Scope:
- Docker Compose als Zielplattform
- lokal auf dem PC testbar, spaeter auf Raspberry Pi uebertragbar
- nur Heimnetz
- MQTT-Praefix `smarthome/`
- getrennte Topics fuer `status` und `state`
- physische Geraete werden technisch nur durch den Master bekannt gemacht
- FlowFuse Dashboard statt deprecated `node-red-dashboard`
- InfluxDB fuer Sensor-Zeitreihen
- SQLite fuer Geraete-, Config-, Automations- und Auditdaten
- echte serverseitige Automationen in Node-RED
- keine Home-Assistant-Abhaengigkeit
- keine Secrets im Repo, nur Example- und Template-Dateien

Nicht Ziel dieses Dokuments:
- fertige Node-RED-Flows vortaeuschen
- ungeklaerte Fachentscheidungen als umgesetzt ausgeben
- Zusatzsysteme wie Grafana, Redis, Nginx oder externe App-Backends einfuehren

## 2. Zielplattform

### 2.1 Technische Basis

| Baustein | Verbindliche Technik | Rolle |
|---|---|---|
| Orchestrierung | Docker Compose | lokale und spaetere Pi-Bereitstellung |
| MQTT Core | Eclipse Mosquitto | Broker, ACL, Retained Messages, Last Will |
| Automation / Integration / UI | Node-RED + FlowFuse Dashboard | MQTT-Verarbeitung, Automationen, Dashboard, Wetterintegration |
| Time Series | InfluxDB | Historisierung von Sensor- und Wetterwerten |
| Betriebsdaten | SQLite | Registry, Config, Automationen, Audit, aktuelle Zustandsbilder |

SQLite ist absichtlich kein eigener Container.
Die Datenbank ist eine Datei im persistenten Server-Datenpfad und wird von Node-RED genutzt.

### 2.2 Netzwerk- und Betriebsgrenzen

- Der Server laeuft nur im Heimnetz.
- Exponiert werden nur die benoetigten LAN-Ports fuer MQTT, Node-RED / FlowFuse Dashboard und optional InfluxDB-Administration.
- Keine WAN-Freigaben als Basisannahme.
- Lokale Tests auf dem PC muessen dieselbe Compose-Struktur nutzen wie spaeter der Raspberry Pi.
- Secrets bleiben lokal in `.env`, `passwd` oder vergleichbaren nicht versionierten Dateien.

## 3. Logische Module und Verantwortlichkeiten

| Modul | Hauptverantwortung | Primartechnik | Persistenz |
|---|---|---|---|
| MQTT Core | Broker, Retain, Sessions, ACL, Last Will | Mosquitto | Broker-Volume |
| Device Registry | bekannte physische Geraete fuehren, Metadaten uebernehmen, Master als Quelle absichern | Node-RED + SQLite | SQLite |
| State Store | letzten bekannten `status` und `state` pro Geraet halten | Node-RED + SQLite | SQLite |
| Time Series | numerische Sensor- und Wetterdaten historisieren | Node-RED + InfluxDB | InfluxDB |
| Automation Engine | Regeln serverseitig ausfuehren und MQTT-Commands erzeugen | Node-RED + SQLite | SQLite |
| Dashboard / UI | Visualisierung, Bedienung, Historie, Automation-Verwaltung | FlowFuse Dashboard in Node-RED | keine eigene DB |
| Config / Provisioning | Soll-Konfigurationen und Zuordnungen pflegen, Revisionsstand verfolgen | Node-RED + SQLite | SQLite |
| Logging / Audit | Bedienaktionen, Config-Aenderungen, Automation-Laeufe, Fehler und Ereignisse protokollieren | Node-RED + SQLite | SQLite |

Klare Grenze:
- Mosquitto ist nur Broker, keine Business-Logik.
- InfluxDB ist nur Zeitreihenspeicher, kein Registry-Ersatz.
- SQLite ist die operative Server-Datenbasis, aber kein Historienersatz fuer Messwerte.

## 4. Serversicht auf die Basisgeraetetypen

| Typ | Serverrolle | Besondere Behandlung |
|---|---|---|
| `master` | Health, Bridge-Status, Registry-Quelle, Provisioning-Endpunkt | kein normales Feldgeraet; eigene Topics unter `smarthome/master/...` |
| `net_erl` | Aktor mit moeglichen Zusatzsensoren | Dashboard braucht Schaltkarte plus optionale Sensoranzeige |
| `net_zrl` | Zwei-Relais-Aktor oder Cover-Geraet | Interlock- und Cover-Zustaende serverseitig mitdenken |
| `net_sen` | Sensor-Geraet | Fokus auf Messwerte, Trends und Alarme |
| `bat_sen` | Event- und Sensor-Geraet mit Schlafphasen | Offline-Regeln duerfen nicht wie bei Netzgeraeten aggressiv sein |

## 5. Nachrichten- und Datenfluss

### 5.1 Discovery und Registry

1. Der Node meldet sich technisch beim Master an.
2. Der Master publiziert `meta` und `status` fuer das Geraet auf MQTT.
3. Die Device Registry schreibt oder aktualisiert den Datensatz in SQLite.
4. Erst danach darf die Serverseite Konfigurationen, Raumzuordnung oder Automationsbeziehungen hinterlegen.

Verbindliche Regel:
Der Server erfindet keine physischen Geraete.
Ein Eintrag unter `devices` entsteht nur nach Master-bestaetigter Bekanntmachung.

### 5.2 Laufzeitdaten

1. Der Master publiziert `status`, `state` und `event`.
2. Der State Store schreibt den letzten bekannten `status` und `state` je Geraet in SQLite.
3. Die Time-Series-Schicht extrahiert nur normalisierte numerische Felder und schreibt sie nach InfluxDB.
4. `event` und technische Fehler werden fuer Nachvollziehbarkeit in SQLite protokolliert.

### 5.3 Commands und Config

1. Dashboard oder Automation Engine publizieren `command` oder `config`.
2. Der Master prueft, ob das Zielgeraet bekannt und online erreichbar ist.
3. Der Master uebertraegt den Befehl per ESP-NOW und publiziert anschliessend `ack`.
4. Der fachlich gueltige Endzustand kommt spaeter wieder ueber `state` zurueck.

Verbindliche Regel:
`ack` bestaetigt Verarbeitung oder Ablehnung.
Die fachliche Wahrheit ueber den Zielzustand kommt aus `state`, nicht aus dem Dashboard.

### 5.4 Automationen

Triggerquellen:
- MQTT `event`
- MQTT `state` oder `status`
- Zeitplaene
- Wetterkontext

Verarbeitung:
- Regeldefinitionen und Aktivierungsstatus liegen in SQLite.
- Die Ausfuehrung laeuft serverseitig in Node-RED.
- Aktionen werden ausschliesslich als MQTT-`command` oder MQTT-`config` ausgeloest.

Nicht erlaubt:
- direkte Datenbankschreiberei als "Automation"
- nur clientseitige Dashboard-Regeln ohne serverseitige Ausfuehrung

## 6. Compose-Zielbild

### 6.1 Verbindliche Dienste

| Dienst | Pflicht | Zweck |
|---|---|---|
| `mosquitto` | ja | MQTT Broker im Heimnetz |
| `nodered` | ja | Flows, Automationen, FlowFuse Dashboard, Wetterintegration, SQLite-Zugriff |
| `influxdb` | ja | Sensor- und Wetter-Zeitreihen |

Kein eigener `sqlite`-Dienst.
Keine Home-Assistant-, Grafana- oder API-Gateway-Container in der Basisarchitektur.

### 6.2 Persistenz

- `mosquitto_data`, `mosquitto_log`
- `nodered_data`
- `influxdb_data`
- SQLite-Datei innerhalb des persistenten Node-RED-Datenpfads oder eines separaten persistenten Host-Verzeichnisses

### 6.3 Betriebsregel

Die Compose-Struktur soll auf PC und Raspberry Pi gleich bleiben.
Unterschiede zwischen Test-PC und Pi duerfen nur aus:
- Architektur des Docker-Images
- Port- oder Hostnamen-Konfiguration
- lokalen Secrets

entstehen, nicht aus einer zweiten Serverarchitektur.

## 7. Wetterintegration

Wetter ist Teil des Serverkonzepts, aber kein physisches Geraet.

Verbindliches Modell:
- Wetter ist eine servereigene Kontextquelle.
- Topics liegen unter `smarthome/server/weather/...`.
- Die Wetterintegration laeuft in Node-RED, nicht im Master und nicht in einem Zusatzsystem.
- Aktuelle Wetterwerte werden retained als `state` veroeffentlicht.
- Integrationsgesundheit und letzte erfolgreiche Aktualisierung werden als `status` veroeffentlicht.
- Numerische Wetterwerte werden in InfluxDB historisiert.
- Quellkonfiguration und Fetch-Status werden in SQLite verwaltet.

Offen, aber ausserhalb dieser Architekturentscheidung:
- konkreter Wetterprovider
- API-Key-Pflicht oder providerfreie Quelle
- Forecast-Tiefe

Fallback-Regel:
Wenn keine frischen Wetterdaten verfuegbar sind, muessen Automationen deterministisch degradieren, zum Beispiel Wetterbedingungen ignorieren oder in einen sicheren Default wechseln.

## 8. Empfohlene Ordnerstruktur

```text
server/
├─ config/
│  ├─ .env.example
│  ├─ docker-compose.example.yml
│  ├─ mosquitto/
│  ├─ nodered/
│  └─ influxdb/
├─ docs/
│  ├─ 00_server_ueberblick.md
│  ├─ 01_server_basisarchitektur.md
│  └─ 02_dashboard_konzept.md
├─ flows/
│  ├─ README.md
│  ├─ 10_ingest/
│  ├─ 20_registry/
│  ├─ 30_state_store/
│  ├─ 40_time_series/
│  ├─ 50_automation/
│  ├─ 60_dashboard/
│  ├─ 70_weather/
│  └─ 80_logging/
├─ db/
│  ├─ README.md
│  ├─ migrations/
│  └─ seeds/
└─ tools/
   ├─ backup/
   └─ validate/
```

Diese Struktur ist ein Zielbild fuer die Serverseite.
Sie definiert Ablage und Verantwortungsgrenzen, aber noch keine fertigen Flows.

## 9. Offene Annahmen und Grenzen

Bereits fest:
- ein aktiver Master als technische Quelle der physischen Geraete
- Heimnetz statt Cloud
- Compose statt Spezialinstallation
- FlowFuse Dashboard statt deprecated Dashboard-Altbestand
- keine Home-Assistant-Abhaengigkeit

Noch offen:
- exakte Backup- und Restore-Strategie fuer SQLite und InfluxDB
- konkrete Wetterquelle
- Benachrichtigungskanal fuer Alarme
- ob InfluxDB-Administration im Heimnetz dauerhaft offen bleibt oder nur lokal genutzt wird

Bewusst nicht Teil der Basisarchitektur:
- Multi-Master-Betrieb
- Mandantenfaehigkeit
- externer Fernzugriff
- vollwertiges Benutzer- und Rollenmodell
- OTA-Management als Serverpflicht
