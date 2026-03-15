# Firmware-Feature-Ownership und Modulmatrix

## Rolle dieser Datei
Diese Datei legt fest, welche Basisfamilie welches Feature primaer tragen soll und welche Erweiterungen nur begruendet zulaessig sind.

Nicht diese Datei:
- `14_test_und_nachweisstand.md` = belegter Ist-Stand
- `19_sondermodule_roadmap.md` = konkrete kuenftige Sondermodule

## Reifestufen
- `nicht vorgesehen`: gehoert nicht sinnvoll zu dieser Basisfamilie
- `architekturvorbereitet`: als Richtung erlaubt, aber noch ohne konkreten Build-/Stubpfad
- `compile/stub vorbereitet`: Buildpfad oder Stubpfad existiert, aber kein Hardwarebeleg
- `real hardware belegt`: im Repo mit echtem Nachweis belegt

## Basisfamilien in einem Satz

| Basisfamilie | Primaere Rolle | Zulaessige Erweiterung | Bewusste Grenze |
|---|---|---|---|
| `net_sen` | Umwelt-, Luftqualitaets-, Licht- und Praesenzsensorik | weitere Sensorprofile und spaetere Anzeigevarianten | kein Relais- oder Batterie-Event-Spielplatz |
| `bat_sen` | Batterie-, Deep-Sleep- und Event-Eingaenge | reed, button, rain und spaetere knappe Spezialvarianten | keine breite Dauer-Sensorik als Default |
| `net_erl` | Ein-Relais-Basis fuer Licht-/Schaltgeraete | begruendete Zusatzsensorik, lokale Bedienung, spaetere Anzeigeelemente | keine neue Basisklasse fuer jede Lampenidee |
| `net_zrl` | Zwei-Relais-/Cover-nahe Basis | Touch, Richtungs-LEDs und ausgewaehlte Zusatzsensorik, wenn das Modul es rechtfertigt | keine generische Sensorplattform und keine still aktive Cover-Sonderlogik |
| `master` | einzige ESP-NOW-zu-MQTT-Bridge | zweiter Master spaeter als weiterer Bridge-Knoten | kein generischer Sensor- oder Aktor-Spielplatz |

## Suitability-Matrix

Legende:
- `P` = primaerer Heimat-Basistyp
- `O` = optional, aber nur mit klarer Modulbegruendung
- `N` = nicht vorgesehen

| Feature | `net_sen` | `bat_sen` | `net_erl` | `net_zrl` | `master` | Repo-Stand |
|---|---|---|---|---|---|---|
| Mechanische Buttons bis 4, event-relevant | `N` | `P` | `O` | `O` | `N` | `bat_sen` nutzt jetzt eine gemeinsame debouncte Input-Grundlage mit Press-/Release-/Long-Press-Scaffold; build-validiert, aber kein Hardwarebeleg |
| Touch-Buttons bis 4, hohe Zuverlaessigkeit | `N` | `O` | `N` | `P` | `N` | nur architekturvorbereitet; kein Buildpfad und kein Realnachweis |
| `BME280` / `BMP280` | `P` | `N` | `O` | `O` | `N` | `net_sen`-Stubpfade fuer `BMP280` und `BME280` compile-validiert; kein Hardwarebeleg |
| `DHT22` | `P` | `N` | `O` | `O` | `N` | `net_sen` mit DHT22 auf `GPIO6` real hardware belegt; andere Familien nur architektonische Option |
| `ENS160` | `P` | `N` | `O` | `N` | `N` | `net_sen`-Stubpfad compile-validiert; kein Hardwarebeleg |
| `SHT41` | `P` | `N` | `O` | `O` | `N` | nur architekturvorbereitet |
| `VEML7700` | `P` | `N` | `O` | `O` | `N` | nur architekturvorbereitet |
| Standard High-PIR | `P` | `N` | `O` | `N` | `N` | `net_sen` laeuft jetzt ueber einen gemeinsamen Presence-Provider mit Pinpfad und Stubvorbereitung; kein PIR-Hardwarebeleg, `net_erl` weiter nur architekturvorbereitet |
| `HLK-LD2410C` Radar | `P` | `N` | `O` | `N` | `N` | gemeinsamer Presence-Provider compile-vorbereitet; OUT- und spaeterer UART-Zustand sind konzeptionell getrennt, aber kein UART-Parser und kein Hardwarebeleg |
| `HLK-LD2410B` Radar als Alt/Test | `O` | `N` | `O` | `N` | `N` | gemeinsamer Presence-Provider compile-vorbereitet; Test-/Alternativvariante ohne behaupteten UART-Fix und ohne Hardwarebeleg |
| Standard analoger Regensensor | `N` | `P` | `N` | `N` | `N` | `bat_sen`-Stubpfad compile-validiert; kein Hardwarebeleg |
| Reed-Fenster-/Tuersignal | `N` | `P` | `N` | `N` | `N` | `bat_sen` nutzt jetzt dieselbe gemeinsame debouncte Input-Grundlage fuer stabile Open/Close-Zustaende; build-validiert, aber kein Hardwarebeleg |
| 17er NeoPixel-Ring | `N` | `N` | `O` | `N` | `N` | nur architekturvorbereitet; aktuell nur fuer spaeteres Kuechenlicht gedacht |
| Zusatz-LEDs fuer Auf/Ab | `N` | `N` | `N` | `O` | `N` | nur architekturvorbereitet; aktuell nur fuer spaeteres Rolladenmodul gedacht |

## Harte Leseregeln aus der Matrix
- `net_sen` ist der primaere Heimathafen fuer Umwelt, Luftqualitaet, Licht und Praesenz.
- `bat_sen` ist der primaere Heimathafen fuer Batterie, Deep-Sleep und ereignisnahe Eingabe-Sensorik.
- `net_erl` und `net_zrl` bleiben Relaisfamilien; Zusatzsensorik ist dort nur dann sinnvoll, wenn ein konkretes Sondermodul sie wirklich braucht.
- `master` bleibt Bridge und Kontrollknoten, keine generische Sensorplattform.
- `compile/stub vorbereitet` ist absichtlich nur Vorbereitung. Wer daraus Hardware-Reife macht, faelscht den Projektstand.
