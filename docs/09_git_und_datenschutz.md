# Git und Datenschutz

## Grundsatz
Dieses Repository ist öffentlich bzw. veröffentlichbar aufgebaut.
Deshalb gilt:
Öffentliche Struktur und private Konfiguration werden strikt getrennt.

## Erlaubt im Repo
- Quellcode
- Dokumentation
- Beispielkonfigurationen
- Platzhalterwerte
- allgemeine Hardware- und Topic-Beschreibungen

## Verboten im Repo
- reale SSIDs
- reale WLAN-Passwörter
- MQTT-Zugangsdaten
- Tokens / API-Keys
- lokale Debug-Logs mit sensiblen Daten
- private Netzlisten
- lokale Secret-Dateien

## Muster
- `Secrets.example.h` -> öffentlich
- `Secrets.h` -> lokal, ignoriert
- `.env.example` -> öffentlich
- `.env` -> lokal, ignoriert

## Vor jedem Push prüfen
- `tools/check_public_repo.ps1` ausführen
- `git status` prüfen
- keine Build-Reste
- keine Editor-Reste
- keine lokale Testdatei versehentlich staged

## Arbeitsbereich
Der push-/pull-fähige Bereich ist dieses Repo selbst.
Nicht daneben arbeiten und später raten, was eigentlich commitbar ist.
