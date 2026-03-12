# NIGHT_PIPELINE_REVIEW

Stand: `2026-03-12`

Basis dieser Review:
- `worktrees/night_01_inventory/`
- `worktrees/night_02_architecture_filter/`
- `worktrees/night_03_extract_helpers/`
- `worktrees/night_04_device_profiles/`
- `worktrees/night_05_low_risk_candidates/`
- `worktrees/night_06_morning_report/`
- aktive Firmware unter `firmware/`

## Kurzurteil

Die Night-Pipeline hat einen kleinen harten Kern mit echtem Nutzen produziert:
- die Altbestands-Landkarte aus Stage 1
- die Architekturfilterung aus Stage 2
- die konkreten Helper-Snapshots aus Stage 3
- das `bat_sen`-Profil und die Sondergeraete-Einordnung aus Stage 4

Der Rest ist zum grossen Teil Wiederholung derselben Aussagen in immer neuen Reports. Das ist kein Fortschritt, sondern Verpackung.

Die wichtigste Korrektur gegen den Pipeline-Ton ist:
- Die aktive Firmware ist noch keine echte `ShCommon`/`ShNodeBase`/`ShMasterBase`-Architektur.
- Die Shared-Libs sind Platzhalter.
- Die reale Firmware sitzt noch in geraetespezifischen `main.cpp`-Dateien.

Wer jetzt so tut, als sei der naechste sinnvolle Schritt ein breiter Shared-Import, baut zuerst Papierarchitektur und erst danach Verhalten. Das waere genau der falsche Move.

## 1. Was wirklich nuetzlich ist und was nur Doku-Rauschen ist

| Stage | Wirklich nuetzlich | Hauptsaechlich Rauschen | Urteil |
|---|---|---|---|
| `night_01_inventory` | `migration/00_inventory/index.md`, `migration/00_inventory/device_candidates.md` | `migration/00_inventory/file_map.csv`, der Stage-Report | Nuetzlich als Altbestands-Landkarte. Nicht als Implementierungsgrundlage. |
| `night_02_architecture_filter` | `migration/01_reuse_matrix/reuse_matrix.md` | die Zusammenfassung ist nur Kurzfassung derselben Matrix | Das ist die staerkste Stage. Sie trennt brauchbare Kleinteile von unbrauchbarem Legacy-Muell. |
| `night_03_extract_helpers` | die echten Header unter `migration/extracted_helpers/*`, plus `index.md` | `README.md` | Nuetzlich, weil hier erstmals konkreter Code statt nur Meinung vorliegt. |
| `night_04_device_profiles` | `migration/device_profiles/bat_sen.md`, `migration/device_profiles/net_zrl.md`, `migration/special_devices/index.md` | `master.md`, `net_erl.md`, `net_sen.md` wiederholen Stage 2 weitgehend | Teilweise hilfreich. Der eigentliche Mehrwert liegt bei `bat_sen` und Sondergeraeten. |
| `night_05_low_risk_candidates` | die Shortlist-Idee als Filter | die Reihenfolge und Teile der Begruendung | Nuetzlich als Triage, aber nicht als Implementierungsplan. |
| `night_06_morning_report` | schneller Ueberblick fuer Menschen | als Source of Truth fuer die naechsten Schritte | Nur Management-Zusammenfassung. Nicht als technische Entscheidungsbasis verwenden. |

## 2. Aktuelle Repo-Fakten, die die Pipeline zu glatt darstellt

1. `firmware/lib/ShCommon/`, `firmware/lib/ShNodeBase/` und `firmware/lib/ShMasterBase/` sind aktuell nur Platzhalter-Ordner mit README. Die aktive Logik lebt nicht dort.
2. `bat_sen` hat weiter eine Platzhalter-Batteriemessung in `firmware/src/bat_sen/main.cpp:294`. Es werden schlicht `100%` und `3700 mV` gesetzt.
3. `bat_sen` fuehrt in `firmware/src/bat_sen/AppConfig.h` bereits Schlaf-, RX-Window- und Wake-Zeiten, aber `firmware/src/bat_sen/main.cpp:18` sagt offen, dass Deep-Sleep bewusst noch nicht aktiv ist. Das ist ein echter Architektur-/Statusmismatch.
4. Die aktuelle `bat_sen`-Konfiguration hat gar keine aktiven Taster-, Reed- oder Regenpins. In `firmware/src/bat_sen/PinConfig.h` stehen `PIN_BUTTON_1..4 = -1`, `PIN_REED = -1`, `PIN_RAIN_ADC = -1`. Der aktive Basispfad ist heute vor allem Batterie, nicht Button-Logik.
5. `firmware/lib/ShProtocol/src/Protocol.h:872` und `:890` enthalten bereits `isValidMac` und `macToString`. Teile der extrahierten MAC-Helper sind also keine Luecke, sondern Dublette.
6. Die groessten echten Duplikate im aktiven Repo sind nicht die extrahierten Mini-Helper, sondern das wiederholte Node-Grundgeruest:
   - `copyText`
   - `logf`
   - `stellePeerSicher`
   - `sendePaket`
   - `HELLO`-/`HEARTBEAT`-Ablauf
   - ESP-NOW-Receive/Send-Callbacks

Diese Duplikate sind real, aber ihre Beseitigung waere jetzt ein Querschnittsrefactor. Genau das ist nicht der Auftrag.

## 3. Welche extrahierten Helper mit der aktuellen Firmware wirklich kompatibel sind

| Helper | Urteil | Zielort | Warum | Jetzt integrieren? |
|---|---|---|---|---|
| `timing/time_helpers.h` | kompatibel | `ShCommon` | reine Zeitprimitive ohne Geraete- oder MQTT-Kopplung; Master und Nodes koennen sie beide nutzen | `ja, aber nur bei echtem Bedarf` |
| `timing/button_event_helpers.h` | teilweise kompatibel | `ShCommon` fuer den Polling-/Debounce-Teil; ISR-Teile vorerst geraetespezifisch | Debounce ist allgemein brauchbar, aber nicht jede Node hat Taster; ISR-Helfer haben aktuell keinen aktiven Consumer | `noch nicht als erster Schritt` |
| `validation/value_range_helpers.h` | teilweise kompatibel | `ShCommon` | `clamp<T>` ist sauber; die `cfg*`-Wrapper sind bereits vom alten Config-Denken gefaerbt | `nur zusammen mit realem ADC- oder Config-Bedarf` |
| `parsing/mac_address_helpers.h` | teilweise kompatibel | `ShCommon` | nur `parseMacString` ist neu; `isValidMAC` ist bereits in `ShProtocol` vorhanden | `nein, solange es keinen MAC-String-Eingabepfad gibt` |
| `serialization/mac_string_helpers.h` | technisch kompatibel, praktisch nutzlos | nicht integrieren | `macToString` existiert bereits in `ShProtocol`; die Lowercase-Variante hat aktuell keinen Consumer | `nein` |
| `sensor_helpers/ens160_aqi_mapping.h` | technisch kompatibel | geraetespezifischer Code | aktuell kein ENS160-Pfad in der aktiven Firmware; fuer die gegebene Architektur kein akuter Shared-Bedarf | `nein` |

## 4. Welche extrahierten Helper aktuell nicht kompatibel genug sind

| Helper | Nicht integrieren, weil | Spaeterer Zielort, falls ueberhaupt |
|---|---|---|
| `config/cfg_nvs_wrapper.h` | haengt fachlich an altem `cfg`-/`sh_cfg`-Denken; im aktiven Firmware-Stand gibt es noch kein sauberes neues Config-/Preferences-Modell | spaeter manuell neu gebaut, nicht importiert |
| `retry_dedup/packet_dedup_window.h` | ist schwaecher als die heutige Duplikatpruefung in `net_erl` und `net_zrl`, weil nur `cmdType + seq + Zeitfenster` betrachtet werden, nicht das Payload | wenn neu gedacht, dann `ShMasterBase`; der Extrakt selbst nicht |

Wichtiger Punkt:
`packet_dedup_window.h` ist kein Upgrade gegen den aktiven Stand. In `firmware/src/net_erl/main.cpp:214` und `firmware/src/net_zrl/main.cpp:221` wird Duplikat-Erkennung heute ueber `seq + komplettes Cmd-Payload` gemacht. Das ist fuer den aktuellen engen `cmd/set`-Pfad sauberer als das extrahierte Zeitfenster.

## 5. Was zuerst integriert werden sollte, in welcher Reihenfolge, und warum

Hier gilt die unbequeme Wahrheit:
Nicht die Helper sind zuerst faellig, sondern die fachliche Luecke in `bat_sen`.

Deshalb ist die sinnvolle Reihenfolge:

1. `bat_sen`-Batteriepfad fachlich klaeren und dann lokal umsetzen.
   - Grund: Das ist die einzige offene Basisgeraete-Luecke laut `docs/TASK_QUEUE.md` und `docs/14_test_und_nachweisstand.md`.
   - Ohne echten `battery_mv`-/`battery_pct`-Pfad ist `bat_sen` aktuell nur formal vorhanden.

2. Erst im Zuge dieser `bat_sen`-Arbeit den minimal noetigen Teil von `value_range_helpers.h` nutzen.
   - Grund: Fuer die Umrechnung auf Prozent und fuer harte Grenzen ist ein einfacher Clamp sinnvoll.
   - Aber: wahrscheinlich reicht anfangs sogar nur `clamp<T>`. Die `cfg*`-Wrapper sind dafuer zu altlastig benannt.

3. `button_event_helpers.h` erst dann ziehen, wenn die gewaehlte `bat_sen`-Variante wirklich einen aktiven Eingabepin bekommt.
   - Grund: Im aktiven Stand sind alle Button-/Reed-/Rain-Pins deaktiviert.
   - Wer den Button-Helper davor shared importiert, loest ein Problem, das im aktiven Build noch gar nicht eingeschaltet ist.

4. `time_helpers.h` danach, aber gezielt.
   - Grund: Der Helper ist sauber und allgemein. Der echte technische Nutzen liegt heute vor allem dort, wo absolute Deadlines direkt mit `millis()` verglichen werden, also im `net_zrl`-Cover-Lock (`firmware/src/net_zrl/main.cpp:260` und `:302`).
   - Nicht als grossen Repo-weiten Cleanup verkaufen. Dafuer ist der aktuelle Nutzen zu klein.

5. `parseMacString` aus `mac_address_helpers.h` nur bei echtem Eingabepfad.
   - Grund: Im aktiven Firmware-Stand gibt es keinen realen MAC-String-Input in Nodes oder Master.

## 6. Was aus der Pipeline ausdruecklich noch nicht integriert werden sollte

1. `serialization/mac_string_helpers.h`
   - Dublette gegen `ShProtocol`.

2. `config/cfg_nvs_wrapper.h`
   - zieht genau den Legacy-Config-Ballast wieder herein, den die Neubasis loswerden will.

3. `retry_dedup/packet_dedup_window.h`
   - kein sauberer Drop-in-Ersatz fuer die heutige enge Cmd-Duplikatpruefung.

4. Alles aus `BAT_Slave.h`, `BatConfigStore.h`, `NetConfigStore.h`, `NetConfigHandler.h` per Schnellimport
   - fachlich interessant, aber architektonisch noch nicht neu modelliert.

5. `BAT-WSW`, `BAT-SEN-RXXXX-001`, `BAT-SEN-WXXXX-001` als direkte Implementierungsziele
   - das sind spaetere `bat_sen`-Featurepakete, nicht die naechste Basisarbeit.

6. Weitere `net_zrl`-Cover-Kalibrierung aus Altpfaden
   - solange keine neuen Config-Keys und kein sauberer Persistenzpfad stehen, waere das nur halb migrierter Legacy-Code.

7. Ein breites Shared-Zusammenziehen aller Node-Helfer in `ShNodeBase` oder `ShMasterBase`
   - ja, die Duplikate existieren.
   - nein, jetzt ist nicht der Zeitpunkt.
   - Das waere ein Refactor-Projekt, kein kontrollierter naechster Schritt.

## 7. Soll `bat_sen` der naechste Fokus sein?

Ja.

Nicht, weil die Pipeline das gerne so haette, sondern weil:
- `docs/TASK_QUEUE.md` es bereits als Prioritaet `1` setzt
- `docs/14_test_und_nachweisstand.md` `bat_sen` weiter als offene reale Hardwareluecke fuehrt
- die aktive Firmware exakt bestaetigt, wo die Luecke liegt: `battery_mv`/`battery_pct` sind noch Platzhalter

## 8. Exakte erste `bat_sen`-Schritte

1. Die fehlenden Hardware-Fakten fuer den Batteriepfad klaeren.
   - Aktuell dokumentiert das Repo nur den Pin `GPIO4`.
   - Nicht dokumentiert sind im aktiven Dokusatz:
     - Spannungsteiler-Verhaeltnis
     - gewollte ADC-Attenuation / Messbereich
     - Zellchemie
     - Zielwerte fuer `leer` und `voll` in `mV`
   - Solange diese Werte nicht feststehen, ist jede Prozentkurve geraten.

2. Danach `messeBatterie()` in `firmware/src/bat_sen/main.cpp` real machen, aber nur dort.
   - kein Start in `ShNodeBase`
   - kein vorgezogener Shared-Umbau
   - erst den aktiven Geraetepfad ehrlich machen

3. Den ersten `bat_sen`-Nachweis auf den bestehenden Pfad begrenzen:
   - Boot
   - HELLO / HELLO_ACK
   - realer `STATE_REPORT` mit plausiblen `battery_mv` und `battery_pct`
   - Sichtbarkeit im Master

4. Erst danach entscheiden, welche `bat_sen`-Variante als naechstes wirklich gebaut wird:
   - Fensterkontakt
   - Wandschalter
   - Regensensor

5. Nur wenn fuer diese gewaehlte Variante ein echter Eingabepin aktiviert wird, `button_event_helpers.h` fuer Debounce/Polling nutzen.
   - Vorher nicht.

6. Deep-Sleep, RX-Window und Wake-on-change erst nach dem echten Batteriepfad und nach einem real laufenden Eingabepfad anpacken.
   - Alles andere ist Show-Fortschritt.

## 9. Unklare Punkte, die nicht geraten werden duerfen

1. Die elektrische Umrechnung fuer den `bat_sen`-ADC-Pfad ist im aktuellen Repo nicht sauber dokumentiert.
2. Es ist im aktiven `bat_sen`-Build nicht festgelegt, ob der naechste reale Hardwarepfad ueber Batterie-only, Reed, Button oder Regen laufen soll.
3. Fuer einen sauberen Shared-Zielort existiert die Bibliotheksstruktur zwar namentlich, aber noch nicht als echte Laufzeitarchitektur. Deshalb ist `ShCommon` fuer kleine pure Utilities sinnvoll, `ShNodeBase` und `ShMasterBase` aber fuer den naechsten Schritt noch kein ehrlicher Landeplatz.
