# Smart-Home-Projekt – Verbindliche Code-Vorgabe

Stand: 2026-03-09  
Projekt: Dezentrales Smart-Home-System mit ESP32, ESP-NOW, MQTT und Node-RED  
Geltungsbereich: **alle** Firmware-Dateien, Libraries, Testprogramme und Master-Programme des Projekts

---

## 1. Ziel dieser Vorgabe

Diese Vorgabe definiert, **wie jeder Code im Projekt aufgebaut sein muss**, damit:

- alle Programme gleich aussehen,
- neue Geräte auf derselben Basis entstehen,
- der Code auch nach Monaten noch lesbar bleibt,
- die Dokumentation im Code bereits für GitHub und Projektdokumentation geeignet ist,
- Fehler schneller gefunden werden,
- private Daten sauber von veröffentlichbarem Code getrennt bleiben.

Diese Vorgabe ist **verbindlich**. Abweichungen sind nur erlaubt, wenn sie technisch notwendig sind und im Code begründet werden.

---

## 2. Grundprinzipien

### 2.1 Einheit vor Individualität
Jeder Code muss dieselbe Grundstruktur verwenden. Ein neues Gerät bekommt **keinen eigenen Stil**, sondern wird in die vorhandene Struktur eingepasst.

### 2.2 Einfachheit vor Cleverness
Der Code soll nachvollziehbar sein, nicht beeindruckend. Versteckte Kurzlösungen, unnötige Makro-Konstrukte, kryptische Abkürzungen und unnötig komplexe Vererbungen sind zu vermeiden.

### 2.3 Kommentarqualität vor Kommentarmenge
Der Code soll **deutsch** kommentiert sein. Kommentare müssen erklären:

- **was** gemacht wird,
- **warum** es gemacht wird,
- **welche Parameter** verwendet werden,
- **warum Zeitwerte oder Grenzwerte gewählt wurden**.

Kommentare dürfen nicht bloß denselben Code in Worte übersetzen.

### 2.4 Eine Funktion = eine klare Aufgabe
Funktionen sollen klein, eindeutig und thematisch sauber getrennt sein. Lange Sammelfunktionen mit mehreren Verantwortlichkeiten sind zu vermeiden.

### 2.5 Hardwareverhalten muss im Code erkennbar sein
Ein Leser mit wenig Programmiererfahrung soll anhand der Kommentare erkennen können:

- welcher Sensor verwendet wird,
- welche Pins belegt sind,
- was bei einem Eingangssignal passiert,
- wann Relais schalten,
- welche Zeitabläufe gelten,
- wie Fehler behandelt werden.

---

## 3. Verbindlicher Aufbau jeder Hauptdatei

Jede `main.cpp` sowie jede größere Moduldatei beginnt mit derselben Reihenfolge.

### 3.1 Dateikopf / Einleitung
Am Anfang jeder Datei steht ein gut sichtbarer Kommentarblock mit:

- Projektname
- Gerätetyp
- Dateiname
- Autor / Ersteller
- Version
- Datum der letzten Änderung
- Kurzbeschreibung
- Hardwarebasis
- verwendete Sensoren / Aktoren
- Besonderheiten
- Abhängigkeiten / Libraries
- Hinweis auf private Konfigurationsdateien, falls notwendig

### 3.2 Funktionsbeschreibung des Geräts
Direkt unter dem Dateikopf steht eine verständliche Beschreibung:

- Welche Aufgabe hat dieses Gerät?
- Welche Signale verarbeitet es?
- Welche Zustände meldet es?
- Welche Aktionen kann es ausführen?
- Welche lokalen Sicherheitsfunktionen gibt es?
- Welche manuellen Bedienelemente gibt es?

### 3.3 Sensor- und Aktorbeschreibung
Für jedes Gerät muss dokumentiert sein:

- Sensorname
- Messgröße
- Zweck im Gerät
- Besonderheiten
- Plausible Grenzen / Messintervalle
- Verhalten bei Ausfall

Für Aktoren zusätzlich:

- Was geschaltet wird
- Schaltlogik
- Sicherheitsverhalten
- Verhalten bei Master-Verlust

### 3.4 Pinlayout
Das Pinlayout muss sichtbar und sofort verständlich beschrieben werden.

Zu jedem Pin gehört:

- GPIO-Nummer
- Signalname
- Richtung
- Funktion
- elektrischer Hinweis, falls relevant

### 3.5 Bibliotheken
Alle verwendeten Bibliotheken werden gesammelt dokumentiert mit:

- Name
- Zweck
- Warum diese Bibliothek verwendet wird
- Ob sie projektweit Standard ist oder nur für dieses Gerät benötigt wird

### 3.6 Einstellbereich vor dem Upload
Vor dem eigentlichen Code kommt ein klar markierter Bereich für Einstellungen, die vor dem Flashen geprüft werden müssen.

Dazu gehören je nach Datei:

- Gerätename
- Geräte-ID
- Gerätetyp
- Hardware-Variante
- Debug aktiv / aus
- Messintervalle
- Timeout-Werte
- verwendete Sensoren
- Schwellwerte
- optionale Features

Dieser Bereich muss so aufgebaut sein, dass ein Leser schnell erkennt, **was man vor dem Upload prüfen oder ändern darf**.

---

## 4. Verbindliche Abschnittsreihenfolge im Code

Jede Hauptdatei hält sich an diese Reihenfolge:

1. Dateikopf und Einleitung  
2. Funktionsbeschreibung  
3. Sensor-/Aktorbeschreibung  
4. Pinlayout  
5. Bibliotheken / Includes  
6. Versions- und Geräteinformationen  
7. Einstellbare Projektparameter  
8. Debug-Konfiguration  
9. Konstanten und Zeitwerte  
10. Pin-Definitionen  
11. Datenstrukturen / Enums / Typen  
12. Globale Statusvariablen  
13. Vorwärtsdeklarationen  
14. Hilfsfunktionen  
15. Hardwarefunktionen  
16. Sensorfunktionen  
17. Kommunikationsfunktionen  
18. Logikfunktionen  
19. Debugfunktionen  
20. `setup()`  
21. `loop()`

Falls eine Datei einen Abschnitt nicht benötigt, bleibt die Reihenfolge trotzdem erhalten, der Abschnitt wird dann mit einem kurzen Kommentar als „nicht benötigt“ markiert oder weggelassen, wenn dadurch keine Unklarheit entsteht.

---

## 5. Kommentarstandard

### 5.1 Sprache
Alle Kommentare im Projekt werden **auf Deutsch** geschrieben.

### 5.2 Jede Funktion erhält einen Kopfkommentar
Jede Funktion bekommt einen Kommentarblock mit mindestens:

- Zweck der Funktion
- Parameter
- Rückgabewert
- Seiteneffekte
- Besonderheiten / Risiken

Beispiel:

```cpp
/**
 * Liest den PIR-Sensor aus und startet bzw. verlängert den Bewegungs-Timer.
 *
 * Zweck:
 * Diese Funktion erkennt eine neue Bewegung am Eingangspin und setzt den
 * Nachlauf-Timer zurück. Dadurch bleibt das Licht nach jeder erkannten
 * Bewegung für die konfigurierte Zeit aktiv.
 *
 * Parameter:
 * - keiner
 *
 * Rückgabewert:
 * - true  -> neue oder weiterhin erkannte Bewegung relevant verarbeitet
 * - false -> keine relevante Änderung
 *
 * Seiteneffekte:
 * - aktualisiert den Zeitstempel der letzten Bewegung
 * - kann Statusvariablen für die Automatik verändern
 */
```

### 5.3 Wichtige Variablen kommentieren
Nicht jede lokale Laufvariable braucht einen Roman. Aber folgende Dinge müssen erklärt werden:

- Zeitwerte
- Grenzwerte
- Flags
- Statusvariablen
- Zustandsautomaten
- Zähler
- Retry-Logik
- Debounce-Werte
- Schwellwerte

### 5.4 Zeitwerte immer begründen
Jeder fest definierte Zeitwert muss kommentiert werden.

Beispiele:

- warum 50 ms Entprellzeit?
- warum 30 s Nachlauf?
- warum 10 min Sendeintervall?
- warum 250 ms ACK-Timeout?

Nicht erlaubt:

```cpp
const uint32_t timeoutMs = 250;
```

Erwartet wird:

```cpp
// ACK-Timeout für eine gesendete Nachricht.
// 250 ms ist kurz genug für eine schnelle Wiederholung,
// aber lang genug, damit der Empfänger die Nachricht
// verarbeiten und das ACK zurücksenden kann.
const uint32_t ACK_TIMEOUT_MS = 250;
```

### 5.5 Blockkommentare verwenden
Längere Codebereiche werden in Funktionsblöcke eingeteilt und jeweils sichtbar getrennt.

Beispiel:

```cpp
// ============================================================
// Abschnitt: Bewegungslogik für automatisches Flurlicht
// ============================================================
```

---

## 6. Benennungskonventionen

### 6.1 Dateien
Dateinamen müssen klar und konsistent sein.

Beispiele:

- `main.cpp`
- `DeviceConfig.h`
- `EspNowHandler.cpp`
- `SensorReader.cpp`
- `LightLogic.h`

### 6.2 Konstanten
Konstanten immer in Großbuchstaben mit Unterstrichen:

- `ACK_TIMEOUT_MS`
- `PIR_HOLD_TIME_MS`
- `DEFAULT_REPORT_INTERVAL_S`

### 6.3 Variablen
Variablen sprechend benennen, keine kryptischen Kürzel:

- `letzteBewegungMs`
- `istLichtAn`
- `aktuellerLuxWert`
- `warteAufAck`

Englische Namen sind nur dort erlaubt, wo sie projektweit technisch sinnvoller sind, z. B. bei Protokollfeldern oder Library-Nähe. Innerhalb einer Datei ist ein Mischstil zu vermeiden.

### 6.4 Funktionen
Funktionen müssen als Verb oder Tätigkeitsbeschreibung erkennbar sein:

- `liesPirSensor()`
- `aktualisiereLichtAutomatik()`
- `sendeStatusAnMaster()`
- `verarbeiteEingehendenBefehl()`

### 6.5 Bool-Werte
Bool-Variablen mit klarer Zustandsaussage:

- `istVerbunden`
- `istLichtAn`
- `hatNeueMesswerte`
- `debugAktiv`

---

## 7. Strukturregeln für Funktionen

### 7.1 Maximale Verantwortung
Eine Funktion soll nach Möglichkeit nur **eine fachliche Aufgabe** übernehmen.

### 7.2 Länge
Sehr lange Funktionen sind zu zerlegen. Richtwert:

- kurze Hilfsfunktionen: bevorzugt
- mittlere Logikfunktionen: akzeptabel
- sehr lange Sammelfunktionen: vermeiden

### 7.3 Keine Logik-Mischung
Nicht in einer Funktion gleichzeitig:

- Sensor lesen,
- MQTT senden,
- Debug ausgeben,
- LED animieren,
- und einen Aktor schalten.

Das muss getrennt werden.

### 7.4 Rückgabewerte bewusst einsetzen
Funktionen sollen nicht unnötig globale Variablen verändern, wenn ein klarer Rückgabewert die bessere Lösung ist.

---

## 8. Debug-Strategie

### 8.1 Debug nur kontrolliert
In frühen Testversionen ist Debug erlaubt und erwünscht. Später wird Debug standardmäßig deaktiviert und nur bei Bedarf wieder aktiviert.

### 8.2 Debug zentral schaltbar
Debug muss über **eine zentrale Konstante oder ein zentrales Makro** ein- und ausschaltbar sein.

Beispiel:

```cpp
constexpr bool DEBUG_AKTIV = true;
```

oder

```cpp
#define DEBUG_AKTIV 1
```

### 8.3 Debug-Ausgaben gruppieren
Debug-Ausgaben werden nicht wahllos im Code verteilt, sondern thematisch gegliedert:

- Start / Initialisierung
- Sensoren
- Kommunikation
- ACK / Retry
- Aktoren
- Fehler

### 8.4 Keine Debug-Ausgabe in finalen Standardständen
Die Standardversion für den Alltagsbetrieb soll ohne serielle Spam-Ausgaben laufen.

### 8.5 Debug-Helfer verwenden
Statt überall `Serial.println()` zu streuen, sollen Wrapper verwendet werden, z. B.:

- `debugInfo()`
- `debugWarn()`
- `debugError()`

So bleibt Debug zentral anpassbar.

---

## 9. Umgang mit Konfiguration

### 9.1 Konfiguration von Anfang an vorgesehen
Konfiguration ist Teil der Architektur und wird nicht später angeklebt.

### 9.2 Trotzdem klare Trennung
Es gibt drei Arten von Konfiguration:

#### A – Feste Entwicklungsparameter im Code
Beispiele:
- Debounce-Zeiten
- Standard-Timeouts
- Standard-Sendeintervalle

#### B – Geräteparameter vor dem Flashen
Beispiele:
- Geräte-ID
- Rollen / Hardwarevariante
- aktivierte Sensoren
- Pinbelegung einer Variante

#### C – Laufzeitkonfiguration über Master
Beispiele:
- Lichtschwellwert
- Nachlaufzeit
- Meldeintervall
- LED-Helligkeit
- Kalibrierwerte

### 9.3 Verboten
Nicht erlaubt ist ein unkontrollierter Mischmasch aus:

- harter Konstante,
- Laufzeitwert,
- privaten Zugangsdaten,
- Testwerten,
- temporären Overrides.

---

## 10. GitHub- und Datenschutz-Regeln

Das Projekt soll veröffentlicht werden. Deshalb muss von Anfang an sauber getrennt werden.

### 10.1 Was auf GitHub landen darf
Öffentlich in das Repository dürfen:

- Quellcode
- Dokumentation
- Beispielkonfigurationen
- Hardwarebeschreibungen
- Standardwerte ohne private Daten
- `.example`-Dateien
- Schaltpläne und allgemeine Bilder

### 10.2 Was nicht auf GitHub landen darf
Nicht ins öffentliche Repository dürfen:

- WLAN-SSIDs
- WLAN-Passwörter
- MQTT-Benutzerdaten
- IP-Adressen des Heimnetzes
- MAC-Listen realer Geräte, wenn privat relevant
- API-Keys
- Tokens
- persönliche E-Mail-Adressen, falls nicht ausdrücklich gewollt
- private Debug-Logs mit sensiblen Daten

### 10.3 Verbindliches Dateikonzept für öffentliche/private Daten
Für jede private Konfiguration gilt dieses Muster:

- öffentliche Vorlage: `Secrets.example.h`
- private lokale Datei: `Secrets.h`
- `Secrets.h` kommt in `.gitignore`

Gleiches Prinzip für größere Konfigurationen:

- `config/public/`
- `config/private/`

### 10.4 Beispiel für `.gitignore`
Mindestens folgende Einträge sind vorzusehen:

```gitignore
.pio/
.vscode/
config/private/
**/Secrets.h
**/secrets.json
*.local.*
*.log
```

### 10.5 Beispielprinzip für Secrets-Dateien
Öffentliche Vorlage:

```cpp
#pragma once

#define WIFI_SSID     "DEIN_WLAN_NAME"
#define WIFI_PASSWORD "DEIN_WLAN_PASSWORT"
#define MQTT_HOST     "192.168.0.10"
#define MQTT_PORT     1883
#define MQTT_USER     "benutzer"
#define MQTT_PASSWORD "DEIN_MQTT_PASSWORT"
```

Private Datei lokal aus Vorlage erzeugen, aber niemals committen.

---

## 11. Git-Regeln

### 11.1 Repository-Struktur
Empfohlene Grundstruktur:

```text
firmware/
├─ docs/
├─ firmware/
│  ├─ lib/
│  ├─ src/
│  ├─ include/
│  └─ config/
├─ hardware/
├─ images/
├─ test/
├─ .gitignore
├─ README.md
└─ CHANGELOG.md
```

### 11.2 Commit-Regeln
Commits klein und thematisch sauber halten.

Gut:
- `Master: ACK-Handling für CMD und CFG ergänzt`
- `NET-SEN: DHT22-Auslesung getrennt und kommentiert`
- `Dokumentation: Pinlayout der Flurlicht-Platine ergänzt`

Schlecht:
- `Update`
- `Noch mehr geändert`
- `fix`

### 11.3 README-Pflicht
Jedes veröffentlichte Teilprojekt muss ein `README.md` haben mit:

- Ziel des Projekts
- Hardware
- Pinbelegung
- benötigte Bibliotheken
- Flash-Hinweise
- öffentliche/private Konfigurationsdateien
- aktueller Entwicklungsstand

### 11.4 Changelog führen
Wichtige Änderungen werden nachvollziehbar dokumentiert.

---

## 12. Verbindliche Dateikopf-Vorlage

Jede Hauptdatei beginnt sinngemäß mit diesem Muster:

```cpp
/*
====================================================================
 Projekt   : Smart-Home-System
 Gerät     : NET-ERL / Küchenlampe
 Datei     : main.cpp
 Autor     : Manuel Ries
 Version   : 0.1.0
 Stand     : 2026-03-09

 Funktion:
 Dieses Gerät ist ein netzbetriebenes Relaismodul zur Steuerung einer
 Küchenlampe. Es erfasst zusätzlich Temperatur, Luftfeuchtigkeit,
 Luftqualität, Lichtwert und Bewegung. Die Lampe kann automatisch oder
 manuell geschaltet werden. Ein WS2812-Ring zeigt den Luftqualitätsstatus.

 Hardware:
 - ESP32-C3 SuperMini
 - 1 Relais
 - DHT22 oder vergleichbarer Sensor
 - Luftqualitätssensor
 - Lichtsensor
 - Radar-Sensor
 - Taster
 - 17x WS2812

 Hinweise:
 - Vor dem Flashen Geräte-ID und Pinbelegung prüfen.
 - Private Zugangsdaten stehen nicht in dieser Datei.
 - Debug standardmäßig nur in Testständen aktiv.
====================================================================
*/
```

---

## 13. Verbindliche Abschnittsvorlage für `main.cpp`

```cpp
// ============================================================
// 1. Bibliotheken
// ============================================================

// ============================================================
// 2. Versions- und Geräteinformationen
// ============================================================

// ============================================================
// 3. Einstellungen vor dem Upload
// ============================================================

// ============================================================
// 4. Debug-Konfiguration
// ============================================================

// ============================================================
// 5. Konstanten, Zeitwerte und Grenzwerte
// ============================================================

// ============================================================
// 6. Pinbelegung
// ============================================================

// ============================================================
// 7. Datenstrukturen und Statusvariablen
// ============================================================

// ============================================================
// 8. Funktionsprototypen
// ============================================================

// ============================================================
// 9. Hilfsfunktionen
// ============================================================

// ============================================================
// 10. Hardware- und Sensorfunktionen
// ============================================================

// ============================================================
// 11. Kommunikationsfunktionen
// ============================================================

// ============================================================
// 12. Logikfunktionen
// ============================================================

// ============================================================
// 13. Debugfunktionen
// ============================================================

// ============================================================
// 14. setup()
// ============================================================

// ============================================================
// 15. loop()
// ============================================================
```

---

## 14. Anforderungen an `setup()`

`setup()` muss klar lesbar und immer ähnlich aufgebaut sein.

Reihenfolge:

1. Basisinitialisierung
2. serielle Schnittstelle optional starten
3. Versions- und Geräteinfos ausgeben
4. Pins initialisieren
5. lokale Statusvariablen initialisieren
6. Sensoren initialisieren
7. Kommunikationsschnittstellen initialisieren
8. letzte Plausibilitätsprüfungen
9. Startmeldung setzen

Keine komplexe Fachlogik in `setup()`, wenn sie besser in eigene Funktionen gehört.

---

## 15. Anforderungen an `loop()`

`loop()` darf nicht zum chaotischen Sammelplatz werden.

Sie soll nur die Hauptbausteine aufrufen, z. B.:

- Zeitbasis aktualisieren
- Kommunikation bedienen
- Sensoren zyklisch lesen
- lokale Logik aktualisieren
- Aktorzustände verwalten
- Events senden
- Debug-Hilfen ausführen

Die eigentliche Fachlogik gehört in eigene Funktionen.

---

## 16. Anforderungen an Master-Code

Der Master muss dieselben Strukturregeln einhalten wie Nodes.

Zusätzliche Pflichtabschnitte für den Master:

- Geräte-Registry
- MQTT-Bridge
- ACK-/Retry-Verwaltung
- Availability-Logik
- Zeitverteilung
- Konfigurationsweiterleitung
- Ereignisprotokollierung

Der Master darf kein Sonderchaos mit völlig anderem Stil werden.

---

## 17. Anforderungen an Node-Code

Jeder Node muss mindestens klar abbilden:

- eigene Geräteklasse
- verwendete Sensoren / Aktoren
- lokale Buttons
- Verhalten bei Master-Verlust
- Verhalten bei Sensorfehlern
- Verhalten bei ACK-Ausfall
- Meldeverhalten bei Zustandsänderung

---

## 18. Anforderungen an Ereignisse und Zustände

### 18.1 Zustandsmeldungen
Zustandsmeldungen beschreiben den **aktuellen Zustand** des Geräts.

Beispiele:
- Licht an/aus
- Lux-Wert
- Temperatur
- Luftfeuchtigkeit
- Batterieprozente
- Rolladen fährt / steht

### 18.2 Ereignismeldungen
Ereignisse melden, dass **etwas passiert ist**.

Beispiele:
- manueller Taster wurde gedrückt
- Bewegung erkannt
- Relais hat geschaltet
- Rolladen wurde gestoppt
- Fenster wurde geöffnet

### 18.3 Pflicht für Aktoren
Jede relevante Aktoraktion muss als Ereignis gemeldet werden.

Beispiele:
- Lampe manuell eingeschaltet
- Lampe automatisch eingeschaltet
- Lampe automatisch ausgeschaltet
- Rolladen fährt hoch
- Rolladen fährt runter
- Rolladen stoppt

### 18.4 Pflicht für schnelle Sensoren
Schnelle oder sicherheitsrelevante Zustandsänderungen sollen sofort gemeldet werden.

Beispiele:
- Wandtaster
- Fensterkontakt
- Türkontakt

---

## 19. Qualitätsregeln vor jeder Freigabe

Vor jeder neuen Testversion ist zu prüfen:

- Sind alle Kommentare aktuell?
- Sind alle Zeitwerte begründet?
- Sind private Daten ausgelagert?
- Ist Debug bewusst gesetzt?
- Ist die Pinbelegung dokumentiert?
- Ist die Versionsnummer angepasst?
- Ist das Gerät im Dateikopf korrekt beschrieben?
- Wurden temporäre Testreste entfernt?

---

## 20. Verbindliche Projektentscheidung

Für dieses Projekt gilt:

- **eine gemeinsame Codebasis**,
- **einheitlicher Aufbau**,
- **deutsche Kommentare**,
- **saubere GitHub-Trennung zwischen öffentlich und privat**,
- **Debug nur gezielt**,
- **alle Dateien dokumentationsfähig**,
- **kein individuell verwachsener Stil pro Gerät**.

Diese Vorgabe ist die Grundlage für alle weiteren Firmware-Dateien.
