Stand: beta44
Datei: beta44_docs_hardware_readmes_und_bat_sen_profilstatus.txt
Datum: 2026-03-15
Autor: Projektdokumentation
Status: Doku-Harmonisierung; keine neuen Hardware- oder Servernachweise

1. Anlass
- Die Repo-Doku hatte zwei klare Schwaechen:
  - die Hardware-README-Dateien der fuenf Basisgeraete waren ganz oder weitgehend Platzhalter
  - der `bat_sen`-Restpunkt wurde an mehreren Stellen sprachlich zu unsauber gefuehrt, obwohl der reale Spannungspfad bereits belegt ist
- Das ist kein fehlender Hardwarefortschritt, sondern schlechte Doku-Disziplin. Wenn ein belegter Messpfad weiter wie eine Grundsatzluecke klingt, ist die Doku fachlich schlechter als der reale Stand.

2. Aenderung
- Aktualisiert:
  - `hardware/master/README.md`
  - `hardware/net_erl/README.md`
  - `hardware/net_zrl/README.md`
  - `hardware/net_sen/README.md`
  - `hardware/bat_sen/README.md`
  - `docs/DECISIONS.md`
  - `docs/14_test_und_nachweisstand.md`
  - `docs/README.md`
  - `docs/16_build_flash_bringup_master_net_erl.md`
- Neu:
  - task-lokaler Stand `PROTOKOLL/beta44_docs_hardware_readmes_und_bat_sen_profilstatus.txt`
- Inhaltlich nachgezogen:
  - kompakte Mindestdoku fuer alle fuenf Basisgeraete mit Zweck, Repo-Basis, relevanten Pins, Kommunikationspfad, belegtem Realstand und offenen Punkten
  - neue dauerhafte Klarstellung in `docs/DECISIONS.md`, dass `bat_sen` Messpfad, Prozentinterpretation und Profilwahl getrennt zu lesen sind
  - Shared Docs so korrigiert, dass `battery_mv` nicht mehr indirekt als ungeklaert wirkt, waehrend nur die Profildeutung von `battery_pct` offen bleibt

3. Begruendung
- Hardware-README-Platzhalter sind nach den vorhandenen Build-, Pin- und Nachweisartefakten nicht mehr zu rechtfertigen. Die Fakten liegen im Repo, also muss die Mindestdoku sie auch benennen.
- Beim `bat_sen` ist der echte Fehler nicht fehlende Implementierung, sondern unscharfe Sprache. Die reale Spannungsmessung ueber `GPIO4` ist belegt; offen bleibt nur, welches explizite Profil den Prozentwert fuer das konkrete Board ehrlich interpretiert.
- Diese Trennung ist fachlich wichtig, weil sonst eine konfigurierbare Deutungslage als defekter Grundpfad missverstanden wird.

4. Verbesserungseffekt
- Ein neuer Chat oder Lehrer findet fuer jede Basisgeraeteklasse sofort die relevanten Mindestinformationen an der erwarteten Hardware-Stelle.
- `bat_sen` wird in der Kern-Doku nicht mehr als halb kaputte Batteriebasis dargestellt.
- Die offenen Punkte bleiben sichtbar, aber enger und ehrlicher:
  - Profilwahl fuer `battery_pct`
  - Deep-Sleep erst nach bewusst aktiviertem Sleep-Pfad
  - Mehrgeraetebetrieb und weitere offizielle Nachweise

5. Risiken / Trade-offs
- Es wurden bewusst keine neuen Hardwarefakten erfunden. Wo das Repo keine Fotos, Schaltplaene oder variantenspezifische Verdrahtung hat, wird das weiterhin als offen benannt.
- Diese Aenderung erzeugt keine neuen Nachweise. Wer daraus neue Hardwarebelege ableitet, liest die Doku falsch.
- `BATTERY_PROFILE = coin_3v_primary` bleibt im aktuellen Repo-Stand gesetzt. Das ist eine reale Konfiguration, aber keine nachtraeglich "bewiesene" Chemieentscheidung.

6. Teststatus
- Ausgefuehrt:
  - Konsistenzpruefung der geaenderten Doku gegen `firmware/platformio.ini`, `PinConfig.h`, `AppConfig.h`, `docs/14_test_und_nachweisstand.md` und die referenzierten `PROTOKOLL`-Staende
  - Repo-Check gemaess lokaler Arbeitsanweisung
- Nicht ausgefuehrt:
  - neue Firmware-Builds
  - neue Hardware- oder Serverlaeufe
- Ergebnis:
  - Doku-Harmonisierung fachlich konsistent zum vorhandenen Repo-Stand

7. Lehrerblick
- Welche Eigenleistung steckt in diesem Stand?
  - die vorhandenen Nachweise und Konfigurationsdateien gegeneinander abgeglichen statt Platzhaltertext einfach auszuformulieren
  - den `bat_sen`-Restpunkt auf die echte offene Frage reduziert und die falsche Defekt-Erzaehlung entfernt
- Was wurde nur als Referenz genutzt?
  - vorhandene Repo-Artefakte: `platformio.ini`, `PinConfig.h`, `AppConfig.h`, `docs/14_test_und_nachweisstand.md`, `docs/BAT_SEN_ADC_FACT_GAP.md` und die einschlaegigen `PROTOKOLL`-Dateien
- Warum ist der Stand fachlich nachvollziehbar?
  - weil jede neue Formulierung an bereits versionierte Repo-Fakten oder bestehende Realnachweise gebunden ist und keine neuen Hardwarebehauptungen einfuehrt
