# Altbestand zerlegt: behalten, nur referenzieren, verwerfen

Stand der Bewertung:
- Altprojekt aus `TechnikerProjektDaten_Git.zip`
- neue Dokumentbasis aus den Architektur- und Codevorgabedateien
- bestehendes neues Repo-Gerüst aus `Techniker_Arbeitsordner.zip`

## Harte Gesamtbewertung

Das Altprojekt ist **nicht** als neue Basis geeignet.

Nicht weil alles schlecht wäre, sondern weil die Struktur falsch gewachsen ist:
- konkrete Geräte statt sauberer Basisklassen
- viel Logik in großen Sketches
- serverseitige Logik an Altstrukturen gekoppelt
- Build-Artefakte und Altlasten im Bestand
- private Konfigurationsreste im Quellbaum
- Geräte-ID-Schema zu stark an Sensorbestückung gekoppelt

Ein Neuaufbau ist deshalb nicht nur sinnvoll, sondern notwendig.

---

## A. Direkt in die Neubasis übernehmbar

Diese Punkte dürfen in die Neubasis eingehen, aber **nicht blind per Copy/Paste als Altcode**:

### 1. Architekturidee Node -> Master -> MQTT -> Server
Bleibt gültig.
Die Rollen sind fachlich richtig getrennt.

### 2. Trennung zwischen Netz- und Batterie-Geräten
Bleibt gültig.
Das Verhalten ist unterschiedlich genug, dass es in der Architektur berücksichtigt werden muss.

### 3. Bekannte reale Geräte und ihre fachliche Logik
Bleiben gültig als Zielgeräte:
- Küchenlampe
- Flurlicht
- Rolladen
- Außenlicht
- Fensterkontakt
- Regensensor
- Wandschalter

### 4. Praktische Erkenntnisse aus dem Altprojekt
Bleiben wertvoll:
- ACK/Retry ist notwendig
- Duplikaterkennung ist notwendig
- lokales Weiterlaufen bei Masterverlust ist notwendig
- Setup-Portal pro Node ist sinnvoll
- feste Kanalstrategie ist sinnvoll
- Rolladen braucht harte Relaisverriegelung
- BAT-Geräte brauchen kurzes RX-Fenster nach TX

### 5. Server-Ideen als Referenz
Nur konzeptionell übernehmbar:
- Docker-basierter Serveraufbau
- MQTT + Node-RED + Datenhaltung
- Git-Sync-Skripte als Grundidee
- Trennung zwischen aktueller Zustandsanzeige und Historie

---

## B. Nur als technische Referenz behalten

Diese Dinge sind nützlich, aber dürfen **nicht** die neue Struktur diktieren:

### 1. Pinbelegungen und Hardware-Know-how
Nützlich für:
- funktionierende Sensoranschlüsse
- Relaisansteuerung
- Tastereingänge
- LED-/WS2812-Ansteuerung
- Rolladen-Pins

### 2. Sensoransteuerung einzelner Geräte
Nützlich als Referenz:
- AHT/ENS/VEML
- DHT22
- PIR/Radar
- Reed-/Regensensor
- WS2812-Ring

### 3. Einzelne Hilfskonzepte aus `SmartHomeCore`
Nützlich als Ideen:
- BuildInfo
- Debug-Wrapper
- Watchdog-Hilfen
- ConfigStore/NVS-Idee
- Board-/LED-/Button-Abstraktion

### 4. Server-Tools aus dem Altprojekt
Nützlich als Idee:
- Push/Pull-Skripte
- Config-Contract-Denke
- Smoke-Test- und MQTT-Testskripte

### 5. Node-RED-Flows und SQLite-Schema
Nur als Referenz für spätere Umsetzung.
Nicht 1:1 übernehmen, weil sie an das alte MQTT- und Gerätebild gekoppelt sind.

---

## C. Bewusst verwerfen

Diese Dinge sollen **nicht** in die Neubasis hineinragen:

### 1. Alte Device-ID-Logik
Verwerfen:
- Sensorbestückung in der ID
- zu viele Bedeutungen in einem Bezeichner
- IDs als heimliche Konfigurationsquelle

Neu gilt:
- einfache Basisklassen-ID
- Fähigkeiten über Metadaten und Capability-Masken

### 2. Große Ein-Datei-Sketches
Verwerfen:
- monolithische `main.cpp`
- vermischte Zuständigkeiten
- Gerätecode + Kommunikationscode + Speziallogik in einem Block

### 3. Altprotokoll als bindende Vorgabe
Verwerfen:
- alte gewachsene Spezialpakete
- Altlasten aus konkreten Gerätefällen

Neu gilt:
- kleine gemeinsame Headerstruktur
- klare Pakettypen
- TLV für `STATE` und `EVENT`

### 4. Server-Altlogik als Soll-Zustand
Verwerfen:
- Flow-Struktur als Architekturvorgabe
- Alt-Dashboard als Ausgangspunkt
- an alte IDs oder alte JSON-Felder gebundene API-Denke

### 5. Build- und Editor-Müll
Sofort verwerfen:
- `.pio/`
- `.vscode/`
- `__pycache__/`
- temporäre Deploy-Dateien
- Build-Binaries

### 6. Private Dateien im Quellbaum
Sofort verwerfen:
- `secrets_local.h`
- lokale Umgebungsdateien mit Realwerten
- private Logs
- reale MAC-/Netzlisten

---

## Konkrete Funde im Altbestand

### Problematische Altlasten
Im alten ZIP wurden unter anderem gefunden:
- ein `secrets_local.h` im Master-Quellordner
- ein kompletter `.pio`-Buildbestand
- `.vscode`-Projektdateien
- Python-Cache-Dateien
- temporäre Deploy-Artefakte aus dem Serverbereich

Das bestätigt:
Der Altstand ist keine saubere veröffentlichbare Basis, sondern ein Arbeitsstand mit Ballast.

### Was daraus folgt
Die Neubasis darf **keinen** Altcode ungeprüft übernehmen.
Alles muss entweder:
- neu geschrieben,
- bewusst als Referenz gelesen,
- oder vollständig aussortiert werden.

---

## Ergebnis der Zerlegung

### Für die Neubasis freigegeben
- Architekturprinzipien
- Geräteklassen
- Sondergeräte-Ziele
- bekannte Hardware-/Pin-Erkenntnisse
- Debug-/Storage-/Watchdog-Konzepte
- Git-/Server-Organisationsideen

### Für die Neubasis gesperrt
- konkrete Alt-Sketches als Startpunkt
- alte Geräte-ID-Systematik
- alte Serverstruktur als Sollzustand
- private Dateien
- Build-Reste
- konkrete Altflows als neue API-Grundlage

Das ist die einzige saubere Trennlinie.
Alles andere wäre wieder ein halb versteckter Neustart auf fauler Basis.
