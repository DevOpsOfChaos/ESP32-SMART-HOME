# BAT_SEN ADC Fact Gap

Stand: `2026-03-12`

## Scope

Geprueft wurden nur diese Pfade:

- `firmware/src/bat_sen/`
- `docs/`
- `PROTOKOLL/`
- `migration/device_profiles/bat_sen.md`
- `migration/special_devices/index.md`
- `worktrees/night_03_extract_helpers/`
- `worktrees/night_04_device_profiles/`
- `worktrees/night_06_morning_report/`

Nicht geprueft wurden bewusst:

- `hardware/bat_sen/`
- andere aktive Firmware-Dateien ausserhalb des freigegebenen Scopes

Wichtig fuer die Einordnung:

- `migration/device_profiles/` und `migration/special_devices/` existieren im aktiven Repo, sind dort aber aktuell leer.
- Die einzigen vorhandenen `bat_sen`-Migrationsnotizen liegen damit nur in den genannten Worktrees.

## 1. Was im aktuellen Repo bereits ueber den `bat_sen`-Batteriepfad bekannt ist

### Aktive Firmware-Fakten

- `bat_sen` ist im aktiven Stand eine batteriegetriebene ESP-NOW-Node-Basis mit `HELLO`, `HEARTBEAT`, `STATE_REPORT` und `EVENT`.
  Quelle: `firmware/src/bat_sen/main.cpp:10-20`
- `STATE_REPORT` transportiert bereits `battery_pct` und `battery_mv`.
  Quelle: `firmware/src/bat_sen/main.cpp:202-215`
- Die aktive `bat_sen`-Basis fuehrt den Batterie-ADC ueber `PIN_BATTERY_ADC`.
  Quelle: `firmware/src/bat_sen/PinConfig.h:21`
- Der Pinstandard legt dafuer `GPIO4` fest und sagt explizit, dass die ADC-Kalibrierung noch offen ist.
  Quelle: `docs/15_hardware_pinstandard.md:22-24`, `PROTOKOLL/beta04_hardware_pinstandard_und_pinfixes.txt:24`, `PROTOKOLL/beta04_hardware_pinstandard_und_pinfixes.txt:42-43`
- Alle anderen moeglichen `bat_sen`-Eingabepfade sind im aktiven Build deaktiviert:
  `PIN_BUTTON_1..4 = -1`, `PIN_REED = -1`, `PIN_RAIN_ADC = -1`.
  Quelle: `firmware/src/bat_sen/PinConfig.h:15-20`
- Damit ist der Batteriepfad im aktiven Build der einzige konkrete Sensorpfad mit festem Hardwareanschluss; Button-, Reed- und Regenpfad sind aktuell nur vorbereitet, aber nicht eingeschaltet.
- `bat_sen` hat bereits Batterie-/Sleep-Konfigwerte:
  `SLEEP_INTERVAL_S = 300`, `RX_WINDOW_MS = 300`, `WACH_NACH_EVENT_MS = 1000`, `LOW_BATTERY_PCT = 20`.
  Quelle: `firmware/src/bat_sen/AppConfig.h:35-49`
- Deep-Sleep ist trotzdem ausdruecklich noch nicht aktiv.
  Quelle: `firmware/src/bat_sen/main.cpp:18-20`, `PROTOKOLL/beta07_basisgeraete_net_zrl_net_sen_bat_sen.txt:33-35`

### Protokoll- und Nachweis-Fakten

- `bat_sen` ist im Repo als Basistyp und als offene reale Hardwareluecke gefuehrt.
  Quelle: `docs/14_test_und_nachweisstand.md:23`, `docs/14_test_und_nachweisstand.md:28`, `docs/TASK_QUEUE.md:3-4`
- Die Server-/Ingest-Seite akzeptiert fuer `state` bereits die Felder `battery_pct` und `battery_mv`.
  Quelle: `PROTOKOLL/beta08_server_mqtt_ingest_realpayloads.txt:19-20`
- Der Topic-Pfad `smarthome/node/bat_sen_01/state` ist ebenfalls bereits im Repo-Nachweis vorhanden.
  Quelle: `PROTOKOLL/beta08_server_mqtt_ingest_realpayloads.txt:50-53`
- Influx kennt `battery_mv` als numerischen Messwert, aber dieser Nachweis stammt nicht aus einem realen `bat_sen`-Hardwarelauf.
  Quelle: `PROTOKOLL/beta09_influx_schreibpfad_verifiziert.txt:34-48`

### Worktree-only-Migrationsfakten

- Das `bat_sen`-Profil in Stage 4 sagt korrekt: zuerst die Platzhalter-Batteriemessung durch einen echten ADC-Pfad ersetzen; Sleep vorher ist Selbsttaeuschung.
  Quelle: `worktrees/night_04_device_profiles/migration/device_profiles/bat_sen.md:37-40`
- Die Sondergeraete `BAT-WSW`, `BAT-SEN-RXXXX-001` und `BAT-SEN-WXXXX-001` sind als spaetere `bat_sen`-Featurepakete einsortiert, nicht als eigener neuer Basistyp.
  Quelle: `worktrees/night_04_device_profiles/migration/special_devices/index.md:7-35`
- Die aktive Doku stellt klar, dass die groesste fachliche Luecke nicht bei Shared-Helpern liegt, sondern beim echten Batteriepfad.
  Quelle: `docs/NIGHT_PIPELINE_REVIEW.md:83-93`

## 2. Welche exakten ADC-/Hardware-Fakten noch fehlen

Folgende Fakten sind im geprueften Repo-Scope nicht belegt:

- exaktes Spannungsteiler-Verhaeltnis des Pfads `Batterie -> GPIO4`
- konkrete Widerstandswerte des Teilers
- gewollte ADC-Attenuation bzw. der beabsichtigte Messbereich am ESP32-C3
- die vorgesehene ADC-Kalibriermethode
  - rohe `analogRead()`-Werte
  - kalibrierte mV
  - per-pin Attenuation
  - eFuse-/Board-Korrektur
- Batterietyp / Zellchemie
  - z. B. Li-Ion, LiPo, Alkali, NiMH
- Zellanzahl / Packaufbau
  - 1 Zelle, 2 Zellen oder etwas anderes
- Zielwerte fuer `leer` und `voll` in Millivolt
- die fachliche Regel, ob Prozent aus Leerlaufspannung oder aus Spannung unter Last abgeleitet werden soll
- ob der ADC-Pfad permanent am Akku haengt oder ueber eine Schaltung nur zeitweise messbar ist

Ohne diese Fakten ist jede mV-zu-Prozent-Umrechnung geraten.
Das sagt die aktive Doku inzwischen selbst explizit.
Quelle: `docs/NIGHT_PIPELINE_REVIEW.md:142-149`

## 3. Gibt es Evidenz fuer Spannungsteiler, ADC-Attenuation, Chemie oder leer/voll?

| Thema | Positive Evidenz im Repo | Was exakt fehlt | Urteil |
|---|---|---|---|
| Spannungsteiler | Ja. `docs/15_hardware_pinstandard.md` sagt: Batteriespannung ueber Spannungsteiler auf `GPIO4`. `PinConfig.h` verweist auf den festen Batterie-ADC-Pin. | Kein Verhaeltnis, keine Widerstandswerte. | Nur Anschlussprinzip bekannt, Umrechnung unbekannt. |
| ADC-Attenuation / Messbereich | Nein. Im aktiven Scope gibt es keine positive Festlegung. | Keine Attenuation, kein Full-Scale-Bereich, keine Kalibriermethode. | Unbekannt. |
| Batteriezellchemie | Nein. | Kein Hinweis auf Li-Ion, LiPo, Alkali, NiMH oder Zellanzahl. | Unbekannt. |
| `leer` / `voll` in mV | Nein. | Keine unteren/oberen mV-Grenzen fuer die Prozentkurve. | Unbekannt. |

Zusaetzliche Praezisierung:

- Die aktive Doku nennt diese vier Punkte inzwischen selbst als offene Hardware-Fakten:
  - Spannungsteiler-Verhaeltnis
  - ADC-Attenuation / Messbereich
  - Zellchemie
  - Zielwerte fuer `leer` und `voll` in `mV`
  Quelle: `docs/NIGHT_PIPELINE_REVIEW.md:142-149`
- Es gibt zwar einen konzeptionellen Prozent-Warnwert:
  - `LOW_BATTERY_PCT = 20`
  - Architekturfeld `low_battery_threshold`
  Aber das ist kein Nachweis fuer `leer`/`voll` in Millivolt.
  Quelle: `firmware/src/bat_sen/AppConfig.h:48-49`, `docs/01_architektur.md:588-592`
- Die eine sichtbare Zahl `battery_mv = 3748` aus `PROTOKOLL/beta09_influx_schreibpfad_verifiziert.txt` ist nur ein Influx-/Ingest-Testpayload fuer `net_sen_test_fix_02`, kein `bat_sen`-Hardwarebeleg und kein Schwellenwert.
  Quelle: `PROTOKOLL/beta09_influx_schreibpfad_verifiziert.txt:35-48`
- Die Extracted-Helper-Stage liefert fuer den Batteriepfad nur generische Utilities wie Clamp/Timing/Button-Helper.
  Gerade `BatConfigStore.h` wurde nicht als Low-Risk-Extrakt uebernommen.
  Quelle: `worktrees/night_03_extract_helpers/migration/extracted_helpers/index.md:9-11`, `worktrees/night_03_extract_helpers/migration/extracted_helpers/index.md:18-20`

## 4. Welche Werte aktuell Platzhalter sind

### Direkte Platzhalter im aktiven `bat_sen`

- `messeBatterie()` setzt aktuell bei vorhandenem ADC-Pin stumpf:
  - `battery_pct = 100`
  - `battery_mv = 3700`
  Quelle: `firmware/src/bat_sen/main.cpp:294-302`

### Sentinels / Ersatzwerte im selben Pfad

- Initial und im Pfad "kein Batterie-ADC vorhanden" werden gesetzt:
  - `battery_pct = 0xFF`
  - `battery_mv = 0`
  Quelle: `firmware/src/bat_sen/main.cpp:248-249`, `firmware/src/bat_sen/main.cpp:295-297`
- Fuer nicht aktive Zusatzsensorik werden gesendet:
  - `window_open = 0xFF`
  - `rain_raw = 0xFFFF`
  Quelle: `firmware/src/bat_sen/main.cpp:212-213`

### Platzhalter-/Reservekonfig in verwandten Dateien

- `LOW_BATTERY_PCT = 20` ist definiert, aber im aktuellen `messeBatterie()`-Pfad nicht mit irgendeiner realen mV-Kurve verknuepft.
  Quelle: `firmware/src/bat_sen/AppConfig.h:48-49`
- `RX_WINDOW_MS = 300` und `WACH_NACH_EVENT_MS = 1000` sind konfiguriert, aber im aktuellen `main.cpp` nicht als echter Sleep-/Wake-Ablauf verdrahtet.
  Quelle: `firmware/src/bat_sen/AppConfig.h:39-46`
- `SLEEP_INTERVAL_S = 300` lebt aktuell praktisch nur als Heartbeat-/State-Intervall und Bannertext weiter, nicht als echter Deep-Sleep-Rhythmus.
  Quelle: `firmware/src/bat_sen/AppConfig.h:35-37`, `firmware/src/bat_sen/main.cpp:288-289`
- `PIN_BUTTON_1..4`, `PIN_REED` und `PIN_RAIN_ADC` stehen auf `-1`; die Stage-06-Empfehlung, zuerst Button-Helper shared zu integrieren, trifft damit nicht den kleinsten aktiven Engpass.
  Quelle: `firmware/src/bat_sen/PinConfig.h:15-20`, `worktrees/night_06_morning_report/migration/reports/REPORT_night_run.md:63-68`

## 5. Kleinster sichere Implementierungsplan ohne breiten Umbau

Der kleinste sichere Plan ist eng. Alles andere ist Themenflucht.

1. Fehlende Hardware-Fakten zuerst festzurren.
   Pflicht:
   - Spannungsteiler-Verhaeltnis
   - ADC-Attenuation / Messbereich
   - Zellchemie / Zellanzahl
   - `leer` / `voll` in `mV`

2. Danach nur den lokalen `bat_sen`-Pfad aendern.
   Zielort:
   - primaer `firmware/src/bat_sen/main.cpp`
   - optional kleine lokale Konstanten in `firmware/src/bat_sen/AppConfig.h`

3. Die bestehende Payload-Schiene unveraendert lassen.
   Also:
   - weiter `nodeStatus.batterie_mv` fuellen
   - weiter `nodeStatus.batterie_pct` fuellen
   - `BatteryStateReportPayload` nicht anfassen
   - keine Server-, MQTT- oder Master-Aenderung

4. Kein Shared-Refactor als Einstieg.
   Also:
   - kein `ShNodeBase`
   - kein `ShCommon`-Umbau als Voraussetzung
   - keine `button_event_helpers.h`- oder `time_helpers.h`-Integration vor dem echten ADC-Pfad

5. Den ersten Realnachweis schmal halten.
   Minimaler Zielnachweis:
   - Boot
   - `HELLO` / `HELLO_ACK`
   - `STATE_REPORT` mit plausiblen `battery_mv` und `battery_pct`
   - Sichtbarkeit dieses States im bestehenden Pfad

Das deckt sich mit der aktiven Doku:
- erst Hardware-Fakten klaeren
- dann `messeBatterie()` lokal und ohne Architekturshow ehrlich machen
Quelle: `docs/NIGHT_PIPELINE_REVIEW.md:140-152`

## 6. Was auf Hardware gemessen werden muss, wenn der Repo-Stand nicht reicht

Wenn die fehlenden Fakten nicht aus belastbarer Hardware-Doku kommen, muessen sie am realen Board gemessen werden:

1. Spannungsteiler messen.
   - Widerstandswerte beider Teilerwiderstaende oder direkt das Verhaeltnis `VBAT : GPIO4`

2. ADC-Knoten gegen Multimeter validieren.
   - reale Batteriespannung am Akku
   - reale Spannung am ADC-Knoten `GPIO4`
   - beides gleichzeitig fuer mindestens einen stabilen Betriebspunkt

3. ADC-Verhalten des ESP32-C3 fuer diesen Pfad bestimmen.
   - roher ADC-Wert bei bekannter Eingangsspannung
   - verwendete Attenuation
   - daraus ableitbarer Messbereich

4. Batterietyp und Zellanzahl feststellen.
   - eingesetzte Zelle / Akkupack real identifizieren
   - ohne das sind `leer` und `voll` fachlich wertlos

5. `leer` und `voll` fuer genau diese Hardware festlegen.
   - Zielwert in `mV` fuer `voll`
   - Zielwert in `mV` fuer `leer`
   - idealerweise nicht nur aus Bauchgefuehl, sondern aus Batterietyp + realem Einsatzfall

6. Pruefen, ob der Messpfad permanent oder geschaltet ist.
   - wenn der Teiler immer am Akku haengt, ist auch sein Ruhestrom Teil der Wahrheit
   - wenn der Pfad geschaltet ist, muss die Messlogik das beruecksichtigen

## Kurzfazit

Der aktive Repo-Stand kennt fuer den `bat_sen`-Batteriepfad genau drei harte Dinge:

- es gibt die Felder `battery_pct` und `battery_mv`
- sie werden ueber `STATE_REPORT` bereits transportiert
- der Messpin ist `GPIO4` ueber einen Spannungsteiler

Der Rest, der fuer eine echte Messung zaehlt, ist im geprueften Scope offen:

- Teilerverhaeltnis
- ADC-Attenuation / Kalibrierung
- Zellchemie / Zellanzahl
- `leer` / `voll` in `mV`

Solange diese Fakten fehlen, sind `100%` und `3700 mV` kein "provisorisch okay", sondern schlicht Platzhalter.
