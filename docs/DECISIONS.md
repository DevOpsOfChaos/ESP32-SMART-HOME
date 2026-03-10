# DECISIONS

## D-001 Neubasis statt Altprojekt-Fortsetzung
Entscheidung:
Das Projekt wird als Neubasis aufgebaut und nicht als direkte Fortsetzung des Altprojekts.

Grund:
Der Altbestand enthaelt zu viele vermischte Altannahmen, Projektreste und technische Altlasten. Eine saubere, veroeffentlichbare Basis ist wichtiger als scheinbar schneller Uebernahmegewinn.

## D-002 Lokales Repo ausserhalb OneDrive
Entscheidung:
Das aktive Git-Repository liegt lokal unter `Documents\Playground`, nicht in OneDrive.

Grund:
OneDrive ist fuer Begleitdateien brauchbar, aber als aktives Git-Arbeitsverzeichnis stoeranfaellig. Das zentrale Versionssystem ist GitHub.

## D-003 Nodes ohne MQTT
Entscheidung:
Nodes sprechen kein MQTT.

Grund:
Die Trennung zwischen Funkebene und Serverebene bleibt nur dann stabil, wenn MQTT ausschliesslich am Master terminiert. Sonst wird jede Node zum Server-Sonderfall.

## D-004 net_zrl als allgemeine Basis, Cover-Logik nur im Cover-Modus
Entscheidung:
`net_zrl` bleibt ein allgemeiner Zwei-Relais-Node. Rolladen- oder Cover-Logik wird nur ueber einen klaren Cover-Modus aktiviert.

Grund:
Die Basisklasse muss wiederverwendbar bleiben. Sonderlogik in der Basis wuerde den Lampenmodus und andere Einsaetze unnoetig verkomplizieren.

## D-005 Kleine Arbeitspakete fuer Codex statt Grossauftraege
Entscheidung:
Codex bekommt kleine, klar abgegrenzte Arbeitspakete statt breiter Sammelauftraege.

Grund:
Kleine Pakete halten Architektur, Review und Protokollierung kontrollierbar. Grosse Pakete erzeugen Kontextverlust, halbfertige Umbauten und schlechte Rueckverfolgbarkeit.

## D-006 Server-Zielplattform ist Docker Compose im Heimnetz
Entscheidung:
Die Serverseite wird fuer Docker Compose im Heimnetz ausgelegt, lokal auf dem PC testbar und spaeter auf Raspberry Pi uebertragbar.

Grund:
Eine Compose-Basis haelt Setup, Portabilitaet und spaetere Betriebsuebernahme kontrollierbar, ohne eine zweite Serverarchitektur einzufuehren.

## D-007 Server-Stack bleibt schlank
Entscheidung:
Die verbindliche Server-Basis besteht aus Mosquitto, Node-RED mit FlowFuse Dashboard, InfluxDB und SQLite. Home Assistant und unnoetige Zusatzsysteme gehoeren nicht zur Basis.

Grund:
Das Projekt braucht eine belastbare, ueberschaubare Serverbasis statt wieder eine Sammlung halbintegrierter Plattformen.

## D-008 MQTT trennt status und state; Geraete kommen nur ueber den Master
Entscheidung:
Unter `smarthome/` bleiben `status` und `state` getrennt. Physische Geraete werden technisch nur vom Master unter MQTT bekannt gemacht.

Grund:
Damit bleiben Betriebszustand, Fachzustand und Registry-Verantwortung klar getrennt und spaetere Serverlogik laeuft nicht in widerspruechliche Quellen.

## D-009 Datenhaltung wird zwischen SQLite und InfluxDB geteilt
Entscheidung:
Sensor-Zeitreihen gehen nach InfluxDB. Geraete-, Config-, Automations- und Auditdaten bleiben in SQLite.

Grund:
Messwert-Historisierung und operative Serverdaten haben unterschiedliche Anforderungen. Ein System fuer alles waere hier der falsche Kompromiss.

## D-010 Automationen laufen serverseitig in Node-RED
Entscheidung:
Automationen werden serverseitig in Node-RED ausgefuehrt. Das Dashboard ist Bedien- und Verwaltungsoberflaeche, nicht die eigentliche Regel-Engine.

Grund:
Nur serverseitige Automationen bleiben reproduzierbar, auditierbar und unabhaengig vom geoeffneten UI-Client.

## D-011 Wetter ist eine servereigene Kontextquelle
Entscheidung:
Wetter wird als servereigene Kontextquelle unter `smarthome/server/weather/...` gefuehrt und nicht als physisches Geraet modelliert.

Grund:
Wetterdaten kommen nicht ueber den Master und duerfen die Geraete-Registry nicht verwaessern.

## Pflegehinweis
Neue Architektur- oder Arbeitsentscheidungen hier kurz mit Entscheidung und Grund ergaenzen. Keine langen Rechtfertigungen, nur belastbare Regeln.
