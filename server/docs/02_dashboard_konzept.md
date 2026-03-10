# Dashboard-Konzept

## 1. Ziel

Das Dashboard wird mit FlowFuse Dashboard in Node-RED umgesetzt.
Es ist die einzige verbindliche Dashboard-Basis fuer dieses Projekt.

Ziele:
- modern und responsive
- lokal im Heimnetz bedienbar
- auf Registry- und State-Daten aufgebaut, nicht auf hardcodierten Geraete-IDs
- klare Trennung zwischen fachlichem `state` und technischem `status`
- serverseitige Automationen steuerbar, aber nicht nur clientseitig ausfuehrbar

Nicht Ziel:
- deprecated `node-red-dashboard`
- eine zweite externe Web-App als Parallelwelt

## 2. UI-Grundregeln

- `state` ist die primaere Nutzsicht.
- `status` ist die technische Sicht und wird als Badges, Hinweise oder Detailbereich gezeigt.
- Aktionen gehen immer ueber MQTT-`command` oder MQTT-`config`.
- Der sichtbare Endzustand wird erst nach Ruecklauf von `state` als bestaetigt betrachtet.
- Layouts muessen auf Mobilgeraeten einspaltig und auf Desktop mehrspaltig funktionieren.
- Keine fest verdrahteten Seiten pro Einzelgeraet; Gruppierung ueber SQLite-Registry, Raumzuordnung und Geraeteklasse.

## 3. Seiten und Kernfunktionen

| Seite | Zweck | Datenquellen | Kernfunktionen |
|---|---|---|---|
| Start | Gesamtueberblick | MQTT current state, SQLite, Influx Kurztrends | Master-Health, Server-Health, aktive Warnungen, Schnellaktionen |
| Raeume | Bedienung nach Raum | SQLite Registry + aktueller `state`/`status` | Schalten, Cover fahren, Sensoren im Kontext anzeigen |
| Geraete | technische und fachliche Detailansicht | SQLite Registry, MQTT, Audit | Detailansicht pro Geraet, letzte Events, Config-Revision, Diagnose |
| Automationen | Regeln verwalten | SQLite Automationen + Laufprotokoll | aktivieren/deaktivieren, letzte Ausfuehrungen, Testlauf nur serverseitig |
| Historie | Trends und Vergleiche | InfluxDB | Zeitreihen fuer Temperatur, Luftfeuchte, Licht, Luftqualitaet, Batterie |
| Wetter | externer Kontext | MQTT `smarthome/server/weather/*`, InfluxDB | aktuelles Wetter, Verlauf, Einfluss auf Automationen |
| System | Betriebs- und Auditansicht | Master-Status, SQLite Audit, Node-RED Betriebsdaten | Registry-Sync, Fehlerliste, letzte Commands, letzte Config-Deploys |

## 4. Geraetekarten nach Basistyp

| Typ | Primaere Karte | Sekundaere Informationen |
|---|---|---|
| `master` | Systemkarte mit WLAN-, MQTT- und ESP-NOW-Status | Firmware, letzter Kontakt, Registry-Zustand |
| `net_erl` | Schaltkarte mit Relaiszustand und Hauptaktion | letzter Ausloeser, Zusatzsensoren, Fault-Status |
| `net_zrl` | Cover- oder Zwei-Relais-Karte mit Interlock-sicherer Bedienung | Position, Fahrtrichtung, Kalibrierstatus, Zusatzsensoren |
| `net_sen` | Sensorcluster-Karte | Trends, Grenzwerte, letzte Aktualisierung |
| `bat_sen` | Event- und Batteriekarte | letzter Kontakt, Batteriestand, Schlaf-/Wake-Kontext |

## 5. Responsive Verhalten

- Mobil: eine Spalte, grosse Touch-Ziele, feste Kopfzeile mit Raum- oder Seitenwechsel.
- Tablet: zwei Spalten, priorisierte Karten zuerst.
- Desktop: Grid mit zwei bis vier Spalten, Historie und Details parallel moeglich.
- Charts duerfen auf Mobilgeraeten reduziert sein, aber nicht unbedienbar.
- Tabellen mit langen Auditlisten brauchen Filter und vernuenftige Standardansichten, keine Full-Dump-Seiten.

## 6. Dashboard und Automationen

Das Dashboard ist Verwaltungs- und Bedienoberflaeche, nicht die Laufzeit der Regeln.

Verbindlich:
- Aktivieren oder Deaktivieren einer Automation schreibt nach SQLite.
- Die Node-RED Automation Engine liest diesen Zustand serverseitig und fuehrt Regeln dort aus.
- Manuelle Aktionen aus dem Dashboard werden in der Audit-Tabelle erfasst.
- Eine Automation darf ueber das Dashboard deaktiviert werden, ohne den zugehoerigen Flow manuell umzubauen.

## 7. Visuale Leitlinien fuer FlowFuse Dashboard

- Kartenbasierte Navigation statt alter Formularseiten.
- Klare Statusfarben fuer `online`, `offline`, `fault`, `warning`, aber keine visuelle Ueberladung.
- Charts nur dort, wo Verlauf relevant ist; kein Blindflug mit Diagrammen fuer alles.
- Konfigurations- und Diagnoseansichten getrennt von den schnellen Bedienseiten.
- Eigene UI-Templates nur dort, wo Standard-Widgets von FlowFuse Dashboard fachlich nicht reichen.

## 8. Grenzen und offene Punkte

Bereits fest:
- FlowFuse Dashboard
- responsive Layoutpflicht
- Bedienung und Historie in einer UI
- Automationen serverseitig

Noch offen:
- exakte Theme-Definition
- ob Schnellaktionen zentral als "Szenen" benoetigt werden
- ob Forecast-Ansichten fuer Wetter in der ersten Dashboard-Version enthalten sind
