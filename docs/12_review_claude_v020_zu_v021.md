# Review Claude-Stand v0.2.0 → bereinigte Basis v0.2.1

## Kernfehler
Claude hat in `NET-ZRL` zwei widersprüchliche Aussagen kombiniert:

- `COVER_MODUS_AKTIV = false` sollte die allgemeine Zwei-Relais-Basis offenhalten.
- Die tatsächliche Schaltfunktion war trotzdem hart als Rolladen-Verriegelung gebaut.

Das war architektonisch falsch.

## Warum das falsch ist
`NET-ZRL` ist laut Architektur eine Basisklasse für:
- Rolladen
- Doppelschaltaktor
- Außenlicht mit zwei Kreisen

Eine harte gegenseitige Sperre ist nur beim Rolladen sinnvoll.
Für Licht oder zwei unabhängige Lasten ist sie ein Funktionsfehler.

## Korrektur in v0.2.1
- Verriegelung nur noch im Cover-Modus
- Reversier-Sperrzeit nur noch im Cover-Modus
- Cover-Befehle werden bei deaktiviertem Cover-Modus ignoriert
- Kommentare und Gerätekonfiguration präzisiert

## Architekturregel für die Zukunft
Basisklasse bleibt breit und wiederverwendbar.
Sonderverhalten wird aktiv zugeschaltet, nie stillschweigend in die Basis gemischt.
