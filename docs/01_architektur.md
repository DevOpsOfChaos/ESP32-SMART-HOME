# Smart-Home-System – Architekturblatt

Stand: Arbeitsentwurf auf Basis der finalen Projektentscheidungen aus dem Chat.  
Ziel dieses Dokuments ist eine **klare, einheitliche und dauerhaft erweiterbare Grundarchitektur**, damit das Projekt nicht erneut durch zu viele Sonderfälle, unklare Zuständigkeiten und inkonsistente Kommunikation unwartbar wird.

---

## 1. Projektziel

Entwicklung eines modularen Smart-Home-Systems mit:

- dezentralen ESP32-basierten Sensor- und Aktormodulen,
- ESP-NOW als lokaler Funkstrecke zwischen Nodes und Master,
- MQTT als Schnittstelle zum Server,
- Node-RED als offizieller Dashboard- und Automatisierungsplattform,
- lokaler Verarbeitung ohne Cloud,
- späterer stiller Umstellbarkeit des Masters, ohne die Nodes neu denken zu müssen.

Wichtig: Home Assistant wird **nicht** erwähnt und **nicht** in die aktuelle Codebasis geschrieben.  
Die Architektur wird lediglich so entworfen, dass später im Wesentlichen nur der Master angepasst werden muss.

---

## 2. Grundprinzip

Das System besteht aus drei Ebenen:

### Ebene A – Nodes
Nodes sind die Feldgeräte im Haus.

Aufgaben der Nodes:
- Sensorwerte erfassen
- Relais schalten
- lokale Taster auswerten
- lokale Schutz- und Komfortlogik ausführen
- Zustände und Ereignisse an den Master melden
- Konfiguration vom Master annehmen

Nodes sprechen ausschließlich:
- **ESP-NOW**
- optional im Setup-Modus ein lokales Webportal

Nodes kennen **kein MQTT**, **kein Node-RED**, **kein Dashboard** und **keinen Serveraufbau**.

### Ebene B – Master
Der Master ist die zentrale Brücke.

Aufgaben des Masters:
- ESP-NOW-Kommunikation mit allen Nodes
- Geräteverwaltung im RAM
- Empfang, Quittierung, Weiterleitung und Befehlsrouting
- Umwandlung in MQTT
- Annahme von MQTT-Befehlen und Umwandlung in ESP-NOW-Kommandos
- Online-/Offline-Erkennung
- zentrale Zeitbasis
- zentrale Konfigurationsverteilung

Der Master ist **kein zweites Node-RED** und **keine Automationsplattform**.

### Ebene C – Server
Der Server besteht aus:
- MQTT-Broker
- Node-RED
- Visualisierung, Regeln und Protokollierung

Direkte Kommunikation Server ↔ Nodes findet nicht statt.

---

## 3. Feste Architekturregeln

Diese Regeln gelten verbindlich:

1. Kein MQTT in Nodes.
2. Kein Dashboard-Wissen in Nodes.
3. Kein direkter Serverzugriff auf Nodes.
4. Alle Node-Typen basieren auf derselben Kommunikationslogik.
5. Sondergeräte sind Erweiterungen auf Basis weniger Grundgeräte, keine komplett neue Architektur.
6. ESP-NOW ist die einzige Funkstrecke zwischen Node und Master.
7. Es wird ein fester WLAN-/ESP-NOW-Kanal im Haus verwendet.
8. Setup-Portal ist auf jedem Node Pflicht.
9. Jede relevante Nachricht wird quittiert.
10. Jeder Aktor meldet Zustandsänderungen und Auslöser aktiv an den Master.
11. Masterverlust darf keine Paniklogik auslösen; der letzte Zustand bleibt erhalten.
12. Lokale manuelle Bedienung hat bei Aktorgeräten weiterhin Funktion, auch wenn die Masterverbindung fehlt.

---

## 4. Master-Hardware

Festlegung:
- Der Master bleibt ein **ESP32-C3**.
- Zuhause wird ein **fester WLAN-Kanal** eingerichtet.

Begründung:
- gleiche Plattformfamilie wie andere Geräte
- mehrfach verfügbar
- geringeres Risiko beim Neuaufbau
- kein zusätzlicher Plattformwechsel mitten in der Projektkonsolidierung

Der WT32-ETH01 ist technisch interessant, wird aber für dieses Projekt aktuell **nicht** als Masterbasis verwendet.

---

## 5. Basisgeräteklassen

Es gibt genau vier Grundgeräteklassen.

### 5.1 NET-ERL
Netzbetriebener Node mit **einem Relais**.

Typische Einsätze:
- einfache Lampensteuerung
- Schaltaktor mit lokalem Taster

### 5.2 NET-ZRL
Netzbetriebener Node mit **zwei Relais**.

Typische Einsätze:
- Rolladensteuerung
- Außenlicht mit zwei getrennten Kreisen
- Doppelschaltaktor

### 5.3 NET-SEN
Netzbetriebener Sensor-Node.

Typische Einsätze:
- Temperatur
- Luftfeuchtigkeit
- Präsenz
- Lichtwert
- Luftqualität

### 5.4 BAT-SEN
Batteriebetriebener Sensor-/Event-Node.

Unter BAT-SEN fallen ebenfalls:
- 1–4-fach Wandschalter
- Fenster- und Türkontakte
- Regensensor
- batteriebetriebener Außen-Temperatur-/Feuchtesensor

Wichtig:
Die Benennung ist nicht perfekt, aber architektonisch ausreichend.  
Einfachheit hat Vorrang vor perfekter Semantik.

---

## 6. Sondergeräte

Sondergeräte bleiben in ihrer Geräte-ID ganz normal innerhalb der vier Basisklassen.

### 6.1 Rolladensteuerung
Basisklasse: **NET-ZRL**

Geplante Ausstattung:
- zwei Relais für Auf / Ab
- DHT22 zusätzlich verbaut
- drei Touch-Buttons: Hoch / Runter / Stop
- zwei LEDs für Fahrtrichtung
- LED-/Anzeigezustand für Kalibrierung
- rolladenspezifische Kalibrierlogik
- rolladenspezifische Konfigurationsparameter

### 6.2 Küchenlampen-Steuerung
Basisklasse: **NET-ERL**

Geplante Ausstattung:
- Luftfeuchtigkeit
- Luftqualität
- Temperatur
- Lichtwert
- Motion-Radar
- manueller Taster zum Toggeln
- 17x WS2812 Pixel-Ring

Geplante Logik:
- LED-Ring an bei Bewegung und/oder Lampe an
- Lampe darf automatisch nur unterhalb einstellbarem Lichtschwellenwert einschalten
- per Weboberfläche jederzeit manuell schaltbar, unabhängig von Motion oder Lichtwert
- LED-Ring zeigt Luftqualität als laufende Kreis-Animation

### 6.3 Flurlicht-Steuerung
Basisklasse: **NET-ERL**

Geplante Ausstattung:
- manueller Taster
- PIR-Sensor (digital High bei Bewegung)
- Lichtsensor
- Temperatur
- Luftfeuchtigkeit

Geplante Logik:
- PIR startet einen konfigurierbaren Timer
- erneute Bewegung setzt den Timer zurück
- Websteuerung hat höchste Priorität
- Lichtschwellenwert konfigurierbar
- Standardwert so gewählt, dass das Licht im Alltag fast immer angehen darf

### 6.4 Außenlampen-Steuerung
Basisklasse: **NET-ZRL**

Geplante Ausstattung:
- zwei Relais
- Temperatur
- Luftfeuchtigkeit
- PIR oder Radar noch offen

---

## 7. Geräte-ID und Gerätemetadaten

Geräte-IDs bleiben bewusst einfach:

- `NET-ERL-001`
- `NET-ZRL-001`
- `NET-SEN-001`
- `BAT-SEN-001`

Die ID beschreibt:
- Geräteklasse
- laufende Nummer

Die ID beschreibt **nicht**:
- Sensorbestückung
- Raum
- Sonderrollen
- Firmware-Sonderfälle

Alles Weitere wird über Metadaten übertragen, z. B.:
- `device_class`
- `device_name`
- `capability_mask`
- `fw_variant`
- `fw_version`
- `power_type`

---

## 8. Setup-Portal

Das Setup-Portal bleibt Pflicht auf jedem Node.

Zweck:
- Master-MAC übernehmen
- optional Gerätename setzen
- Status anzeigen
- Werksreset auslösen

Bewusst nicht Teil des Portals:
- große Sensorkonfigurationen
- komplexe Logikeditoren
- MQTT-Daten
- Dashboard-Funktionen
- Gerätespezialmenüs ohne echten Mehrwert

Das Setup-Portal dient nur zur Inbetriebnahme und Grundwartung.

---

## 9. Kommunikationsmodell

Die Kommunikation wird strikt getrennt in:

- **STATE** → aktueller Zustand
- **EVENT** → gerade eingetretenes Ereignis
- **CMD** → Befehl vom Master
- **CFG** → Konfigurationsänderung
- **ACK** → Quittung
- **HELLO / HELLO_ACK** → Anmeldung / Annahme
- **TIME** → Zeitabgleich

### 9.1 STATE
STATE beschreibt den aktuellen Zustand eines Nodes.

Beispiele:
- Lampe an/aus
- Relaisstatus
- Temperaturwert
- Luftfeuchtigkeit
- Lichtwert
- Luftqualität
- Fenster offen/geschlossen
- Rolladen fährt / steht / Position unbekannt
- Batterieprozentsatz
- RSSI
- letzter Auslösegrund
- Zeitstempel

Frage, die STATE beantwortet:
**Wie ist das Gerät gerade?**

### 9.2 EVENT
EVENT beschreibt ein konkretes Ereignis.

Beispiele:
- manueller Taster gedrückt
- PIR ausgelöst
- Radar erkannt
- Fenster geöffnet
- Fenster geschlossen
- Lampe automatisch eingeschaltet
- Lampe automatisch ausgeschaltet
- Lampe manuell getoggelt
- Rolladen fährt hoch
- Rolladen fährt runter
- Rolladen stoppt
- Kalibrierung gestartet
- Kalibrierung abgeschlossen
- Sensorfehler erkannt
- Node neu gestartet

Frage, die EVENT beantwortet:
**Was ist gerade passiert?**

Diese Trennung ist Pflicht.  
STATE und EVENT werden nicht vermischt.

---

## 10. ACK-Strategie

Festlegung:
- Jede relevante Nachricht wird quittiert.
- Jede Nachricht besitzt eine Sequenznummer (`seq`).
- ACK referenziert genau diese Sequenznummer.
- Retries sind begrenzt.
- Duplikate müssen erkannt und ignoriert werden.

### 10.1 ACK-Pflicht besonders wichtig für
- alle `CMD`
- alle `CFG`
- alle `EVENT`
- alle Aktor-bezogenen `STATE`-Meldungen
- alle manuellen Bedienereingriffe

### 10.2 Begründung
Ohne ACK-System weißt du bei Funkverlust nie sicher:
- ob ein Befehl angekommen ist,
- ob ein Aktor geschaltet hat,
- ob ein Event verloren ging,
- ob du Mehrfachaktionen produzierst.

### 10.3 Harte Warnung
ACK für alles ist nur dann gut, wenn sauber gebaut.  
Ohne Sequenznummer, Retry-Limit und Duplikaterkennung ruinierst du dir das System selbst.

---

## 11. ESP-NOW-Pakettypen

Die Basis kennt nur diese Pakettypen:

- `HELLO`
- `HELLO_ACK`
- `STATE`
- `EVENT`
- `CMD`
- `CFG`
- `ACK`
- `TIME`

Mehr Pakettypen werden am Anfang bewusst nicht eingeführt.

---

## 12. Paketformat

### 12.1 Header
Jedes Paket besitzt einen kleinen gemeinsamen Header:

- `magic`
- `proto_ver`
- `msg_type`
- `seq`
- `flags`
- `payload_len`
- `crc16`

### 12.2 Payload-Regel
- `HELLO`, `HELLO_ACK`, `CMD`, `CFG`, `ACK`, `TIME` als feste strukturierte Payloads
- `STATE` und `EVENT` als **TLV-Felder**

Begründung:
- feste Pakete dort, wo Struktur klar und stabil ist
- TLV dort, wo viele Gerätekombinationen und spätere Erweiterungen zu erwarten sind

Diese Mischung ist robuster als ein komplett starres System und sauberer als frei zusammengeworfene Spezialpakete.

---

## 13. TLV-Konzept für STATE und EVENT

TLV bedeutet:
- `type`
- `length`
- `value`

Vorteile:
- neue Felder später ergänzbar
- Sondergeräte ohne neues Paketlayout möglich
- einheitlich für alle Klassen nutzbar
- Protokoll bleibt trotz Wachstum kontrollierbar

### 13.1 Mindestfelder für STATE
Folgende Datenfelder müssen grundsätzlich abbildbar sein:

- Gerätetyp / Device Class
- Zeitstempel
- Temperatur
- Luftfeuchtigkeit
- Präsenz / Bewegung
- Lichtwert
- Fenster-/Türstatus
- Luftqualität
- Batterie
- Relaisstatus
- Rolladenstatus
- RSSI
- Fehlerstatus
- letzter Auslöser / letzter Schaltgrund

### 13.2 Mindestfelder für EVENT
Folgende Event-Arten müssen mindestens abbildbar sein:

- Taster manuell gedrückt
- Mehrfachtaster Kanal 1–4 gedrückt
- Fenster geöffnet
- Fenster geschlossen
- Regensensor ausgelöst
- Bewegung erkannt
- Lampe automatisch eingeschaltet
- Lampe automatisch ausgeschaltet
- Lampe manuell geschaltet
- Relais geändert
- Rolladen hoch
- Rolladen runter
- Rolladen stop
- Kalibrierung gestartet
- Kalibrierung beendet
- Node gestartet
- Sensorfehler
- Kommunikationsfehler

### 13.3 Empfehlung zur TLV-Struktur
Nicht jedes Feld braucht seinen eigenen Datentyp-Zoo.  
Halte es klein:

- Bool
- UInt8
- UInt16
- Int16
- UInt32
- Float32 nur dort, wo wirklich nötig

Messwerte können oft skaliert als Integer übertragen werden, z. B.:
- Temperatur in 0.1 °C
- Feuchte in 0.1 %
- VOC/CO2 Index als Integer

Das ist sauberer als überall Float-Müll.

---

## 14. Melderegeln für Aktoren und Ereignisse

Festlegung:

### 14.1 Jeder Aktor meldet aktiv an den Master
Mindestens bei:
- manuellem Taster-Toggle
- automatischem Einschalten
- automatischem Ausschalten
- Relaisänderung
- Rolladen hoch
- Rolladen runter
- Rolladen stop
- Kalibrierstatusänderung

### 14.2 BAT-SEN meldet sofort
Insbesondere:
- Wandschalter 1–4
- Fenster-/Türkontakt
- Regensensor

Diese Meldungen müssen so schnell wie möglich gesendet werden.

### 14.3 Zusätzlich periodischer STATE
Neben den Ereignissen werden periodisch Zustände gesendet, damit der Master auch ohne frisches Event einen sauberen Ist-Zustand kennt.

---

## 15. MQTT-Schema

Das MQTT-Schema wird von Anfang an stabil und plattformneutral aufgebaut.

### 15.1 Topics
- `smarthome/master/status`
- `smarthome/<device_id>/meta`
- `smarthome/<device_id>/availability`
- `smarthome/<device_id>/state`
- `smarthome/<device_id>/event`
- `smarthome/<device_id>/ack`
- `smarthome/<device_id>/cmd`
- `smarthome/<device_id>/cfg`

### 15.2 Regeln
- `meta` retained
- `availability` retained
- `state` retained
- `event` nicht retained
- `ack` nicht retained
- `cmd` wird vom Master abonniert
- `cfg` wird vom Master abonniert

### 15.3 Grundsatz
Der MQTT-Baum wird so gebaut, dass Node-RED sauber damit arbeiten kann.  
Spätere Master-Anpassungen dürfen intern viel ändern, aber die Node-Grundlogik bleibt unberührt.

---

## 16. Konfigurationsstrategie

Hier ist dein Denkfehler bisher glasklar:  
Du willst „alles sofort drin haben“, weil du spätere Anpassungen hasst. Das ist verständlich, aber als Implementierungsprinzip brandgefährlich.

Die saubere Lösung ist deshalb:

### 16.1 Vollständige Architektur von Anfang an
Ja:
- Konfigurationspaket `CFG` existiert von Anfang an
- alle wichtigen Konfigurationsgruppen werden von Anfang an definiert
- Felder werden sauber reserviert und dokumentiert

### 16.2 Schrittweise Aktivierung im Code
Aber:
- nicht jedes Feld muss in der ersten funktionierenden Umsetzung bereits aktiv genutzt werden
- ungenutzte Felder dürfen vorhanden sein
- das Protokoll bleibt vollständig, die Erstimplementierung bleibt kontrollierbar

Das ist der saubere Mittelweg zwischen:
- „zu wenig vorbereitet“ und
- „wieder alles gleichzeitig verbaut“

---

## 17. Empfohlene Konfigurationsfelder

### 17.1 Gemeinsame Basisfelder
- `device_name`
- `report_interval_s`
- `ack_timeout_ms`
- `max_retries`
- `status_interval_s`
- `event_debounce_ms`
- `led_enabled`
- `timezone_offset_min` oder Zeitmodus-Basis

### 17.2 Sensorfelder
- `temp_report_delta`
- `hum_report_delta`
- `lux_report_delta`
- `air_report_delta`
- `presence_hold_s`
- `window_debounce_ms`
- `rain_debounce_ms`

### 17.3 Licht-/Relaisfelder
- `auto_mode_enabled`
- `light_threshold_on`
- `light_threshold_off`
- `auto_off_delay_s`
- `manual_override_enabled`
- `button_mode`
- `relay_default_on_boot`

### 17.4 Rolladenfelder
- `cover_run_time_up_ms`
- `cover_run_time_down_ms`
- `cover_reverse_lock_ms`
- `cover_calibrated`
- `touch_button_enabled`
- `touch_longpress_ms`
- `cover_position_estimation_enabled`

### 17.5 WS2812-/Anzeige-Felder
- `ring_enabled`
- `ring_brightness`
- `ring_mode`
- `ring_motion_mode`
- `ring_airquality_mode`
- `ring_timeout_s`

### 17.6 Batterie-Felder
- `wake_interval_s`
- `rx_window_ms`
- `low_battery_threshold`
- `sleep_after_event_ms`
- `multi_button_longpress_ms`

---

## 18. Online-/Offline-Strategie

### 18.1 Netzgeräte
- periodisches HELLO oder STATE
- gelten nach Timeout als offline

### 18.2 Batterie-Geräte
- eventgetrieben oder nach Schlafintervall
- nicht aggressiv offline markieren
- letzter Kontaktzeitpunkt zählt stärker als häufiger Ping

### 18.3 Master-Verlust
Bei Verbindungsverlust zum Master gilt:
- aktueller Aktorzustand bleibt erhalten
- lokale Bedienung bleibt möglich
- keine Panikabschaltung
- keine unkontrollierten Rücksetzlogiken

---

## 19. Geräte-Registry im Master

Der Master führt zunächst eine Registry **im RAM**.

Gespeicherte Basisinformationen:
- `device_id`
- `device_class`
- `device_name`
- `capability_mask`
- `fw_version`
- `last_seen`
- `online`
- `last_rssi`
- `last_battery`
- `last_state`
- `last_error`

Persistenz ist aktuell nicht Pflichtbestandteil.

---

## 20. Verschlüsselung

Festlegung:
- Verschlüsselung wird architektonisch vorbereitet.
- Die erste grob funktionierende Basis darf unverschlüsselt starten.
- Danach wird Verschlüsselung als nächster größerer Stabilitätsschritt integriert.

Das bedeutet konkret:
- Key-Slots und Konfigurationspfade jetzt schon vorsehen
- Kommunikationsmodell nicht so bauen, dass spätere Verschlüsselung weh tut
- aber nicht die Erstumsetzung an der Schlüsselverwaltung scheitern lassen

---

## 21. Softwarestruktur

Empfohlene Struktur:

```text
firmware/
├─ lib/
│  ├─ sh_common/
│  │  ├─ protocol/
│  │  ├─ crc/
│  │  ├─ types/
│  │  ├─ logging/
│  │  └─ buildinfo/
│  ├─ sh_node/
│  │  ├─ espnow_node/
│  │  ├─ setup_portal/
│  │  ├─ config_store/
│  │  ├─ sensors/
│  │  ├─ actuators/
│  │  ├─ events/
│  │  └─ state_builder/
│  └─ sh_master/
│     ├─ espnow_master/
│     ├─ mqtt_bridge/
│     ├─ device_registry/
│     ├─ command_router/
│     ├─ config_router/
│     ├─ availability/
│     └─ time_sync/
├─ src/
│  ├─ master/
│  │  └─ main.cpp
│  ├─ node_net_erl/
│  │  └─ main.cpp
│  ├─ node_net_zrl/
│  │  └─ main.cpp
│  ├─ node_net_sen/
│  │  └─ main.cpp
│  └─ node_bat_sen/
│     └─ main.cpp
└─ configs/
   ├─ devices/
   ├─ boards/
   └─ defaults/
```

Wichtig:
- genau vier Basis-Firmwares
- Sondergeräte entstehen aus Konfiguration + Capabilities + Hardwarebelegung
- keine Firmware-Kopierhölle mehr

---

## 22. Erstaufbau und Umsetzungsreihenfolge

Du willst alle Geräte mindestens einmal bauen. Das ist okay.  
Aber du darfst nicht wieder alles parallel als Erstimplementierung aufblasen.

Saubere Reihenfolge:

### Phase 1 – Grundfunktion End-to-End
Vier Beispielgeräte vollständig:
- `NET-ERL` einfache Lampe
- `NET-ZRL` Rolladen-Grundfunktion
- `NET-SEN` Standardsensor
- `BAT-SEN` Fensterkontakt oder Wandschalter

Ziel:
- Node ↔ Master ↔ MQTT ↔ Node-RED vollständig lauffähig

### Phase 2 – echte Sondergeräte
Danach:
- Flurlicht
- Küchenlicht
- Rolladen-Sonderlogik
- Außenlicht

### Phase 3 – restliche BAT-SEN-Varianten
- Mehrfachtaster
- Regensensor
- Außensensor
- weitere Kontakte

---

## 23. Klare Schlussfolgerung

Die neue Architektur folgt nicht dem Ziel, sofort alles maximal kompliziert abzubilden.  
Sie folgt dem Ziel, **ein vollständiges, sauberes und einheitliches Fundament** zu schaffen.

Der kritische Unterschied ist:
- **vollständig im Entwurf**
- **kontrolliert in der Erstimplementierung**

Genau das hat vorher gefehlt.

Wenn diese Regeln eingehalten werden, kann das Projekt später wachsen, ohne wieder strukturell zu zerfallen.
