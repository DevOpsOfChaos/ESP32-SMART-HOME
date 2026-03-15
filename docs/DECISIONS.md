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

## D-005 Kleine Arbeitspakete statt Grossauftraege
Entscheidung:
Es werden kleine, klar abgegrenzte Arbeitspakete statt breiter Sammelauftraege bevorzugt.

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

## D-012 Server-Wetterprovider ist in V1 Open-Meteo
Entscheidung:
Der verbindliche Wetterprovider fuer die erste Server-Basis ist Open-Meteo.

Grund:
Open-Meteo erlaubt eine schlanke, servereigene Wetterintegration ohne zusaetzliche Plattformabhaengigkeit.

## D-013 Influx-Retention betraegt in V1 365 Tage
Entscheidung:
Die Start-Retention fuer den Influx-Bucket liegt bei 365 Tagen.

Grund:
Damit bleibt genug Verlauf fuer das Projekt erhalten, ohne fuer V1 schon eine komplexe Archivstrategie einzufuehren.

## D-014 Es gibt in V1 keinen Alert- oder Benachrichtigungskanal
Entscheidung:
Alarm- und Benachrichtigungskanaele werden in V1 bewusst nicht eingebaut.

Grund:
Die Basis muss zuerst Ingest, Registry, State-Store und Historisierung sauber liefern, bevor Folgekanal-Logik dazukommt.

## D-015 Forecast und Szenen sind nicht Teil der ersten UI-Stufe
Entscheidung:
Forecast-Ansichten und Szenen bekommen in V1 keine eigene UI-Stufe.

Grund:
Die erste UI-Stufe soll nur die notwendige Betriebsbasis zeigen und nicht wieder zu frueh in Komfortflaeche ausufern.

## D-016 Oeffentliche Fachdoku, Testplan, Nachweis und Historie bleiben getrennt
Entscheidung:
`docs/README.md` bleibt nur der oeffentliche Index. `docs/06_testplan.md` beschreibt nur den Soll-Pruefplan, `docs/14_test_und_nachweisstand.md` nur den belegten Ist-Stand und `PROTOKOLL/` bleibt die historische Chronik einzelner Stande. Interne Sprint-, Task- oder Arbeitsgedaechtnis-Dateien gehoeren nicht in den oeffentlichen GitHub-Stand.

Grund:
Ohne diese Trennung verwischen interne Arbeitsplanung, oeffentliche Technikdoku, Testabsicht und echte Nachweise. Dann verraet das Repo unnoetig den Umsetzungsprozess und behauptet frueher oder spaeter Dinge, die es nicht belastbar belegen kann.

## D-017 bat_sen trennt Messpfad, Prozentinterpretation und Profilwahl
Entscheidung:
Bei `bat_sen` gilt der reale Spannungspfad ueber `GPIO4` als eigener Kernpfad. `battery_pct` bleibt davon getrennt eine profilabhaengige Interpretation des gemessenen Spannungswerts. Die Profilwahl erfolgt bewusst ueber `BATTERY_PROFILE` in `firmware/src/bat_sen/AppConfig.h` und ist keine versteckte Defektstelle.

Grund:
Die Realnachweise belegen den lokalen ADC-Pfad sowie den offiziellen Serverpfad fuer `battery_mv` bereits. Offen ist nur, welches explizite Profil das konkrete reale Board fuer `battery_pct` am ehrlichsten abbildet. Wer beides vermischt, beschreibt einen funktionierenden Messpfad faelschlich als ungeklaert.

## D-018 Firmware-Vorbereitung vor der naechsten integrierten Hardware-Welle
Entscheidung:
Kommende `net_sen`-/`bat_sen`-Optionen werden zuerst additiv in der Firmware vorbereitet:
compile-time Featurepfade, klare Providerwahl, gemeinsame State-Assembly und explizit getrennte Stub-Scaffolds. Reale Nachweise folgen spaeter in groesseren integrierten Hardware-Wellen.

Grund:
Weitere Mikrotests auf halbfertiger Firmware fragmentieren die Codebasis und verwischen, was wirklich belegt ist. Die robustere Reihenfolge ist:
bestehende Referenzpfade stabil halten, Erweiterungen code-seitig ehrlich vorbereiten, Stub- und Realpfade trennen und erst dann Hardware gebuendelt pruefen.

## D-019 Sondermodule wachsen aus den vorhandenen Basisfamilien
Entscheidung:
Sondermodule entstehen aus `master`, `net_erl`, `net_zrl`, `net_sen` und `bat_sen` ueber Feature-Auswahl, Pin-Mapping und Profile. Es gibt keine neue Basisklasse fuer jedes Spezialgeraet. Nicht jedes Feature gehoert auf jeden Basistyp.

Grund:
Wer fuer jede Variante einen neuen Geraetetyp erfindet, bekommt wieder Namensvielfalt statt Architektur. Die Basisfamilien bleiben nur dann wiederverwendbar, wenn Sondergeraete als disziplinierte Ableitungen wachsen und unpassende Sensorik nicht auf jede Plattform gekippt wird.

## D-020 Reifestufen muessen explizit getrennt bleiben
Entscheidung:
Im Repo werden mindestens diese Zustaende sauber unterschieden:
`nicht vorgesehen`, `architekturvorbereitet`, `compile/stub vorbereitet` und `real hardware belegt`.
Build- oder Stub-Support ist kein Hardware-Nachweis.

Grund:
Ohne diese Trennung werden vorbereitete Pfade schnell als `eigentlich schon fertig` verkauft. Das ist fachlich unserioes und zerstoert die Aussagekraft von Doku, Review und spaeteren Nachweisen.

## D-021 Nodes bekommen einen einheitlichen Setup-, Provisionierungs- und Resetvertrag
Entscheidung:
Wenn auf einem Node keine Master-MAC persistent gespeichert ist, muss er automatisch in einen lokalen Setup-Modus starten. Dieser besteht aus gehostetem WLAN und einfachem lokalem Webserver. Die Setup-Seite darf per QR-/GET-Parameter eine Master-MAC uebernehmen, muss sie aber vor dem Speichern sichtbar zur Bestaetigung anzeigen. Relevante Felder duerfen mit Defaults oder gespeicherten Werten vorbefuellt sein. Ein 5-Sekunden-Halten des Boot-Tasters oeffnet den Setup-Modus erneut, ein 10-Sekunden-Halten loescht die gespeicherte Bindung und startet in den Setup-Modus zurueck. `bat_sen`-Sleep-Werte werden dabei benutzerseitig in Minuten/Stunden gefuehrt, nicht in Millisekunden.

Grund:
Node-Inbetriebnahme darf nicht an hart einkompilierten Einzelwerten oder seriellem Gefrickel haengen. Gleichzeitig darf der Setup-Pfad nicht zum zweiten Konfigurationssystem auswuchern. Ein einfacher, einheitlicher Vertrag ist robuster und spaeter besser wartbar.

## D-022 Server-Autoritaet bleibt ueber lokaler Bequemlichkeitslogik
Entscheidung:
Der Server hat beim fachlichen Zielzustand das letzte Wort. Lokale Automationen wie PIR-/Radar-getriggerte Relaislogik sind erlaubt, bleiben aber uebersteuerbar. Ein lokal automatisch eingeschaltetes Licht muss serverseitig wieder ausschaltbar bleiben. Relaisgeraete behalten trotzdem eine lokale manuelle Tasterfunktion.

Grund:
Ohne klare Autoritaet entstehen widerspruechliche Schaltquellen und unerklaerbare Zustaende. Rein lokale Automationshoheit waere genauso falsch wie das Weglassen lokaler Bedienbarkeit am Geraet.

## Pflegehinweis
Neue Architektur- oder Arbeitsentscheidungen hier kurz mit Entscheidung und Grund ergaenzen. Keine langen Rechtfertigungen, nur belastbare Regeln.
