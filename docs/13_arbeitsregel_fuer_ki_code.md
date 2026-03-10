# Arbeitsregel für KI-generierten Code

## 1. Basisklasse vor Sonderlogik
Wenn ein Gerät mehrere Einsatzzwecke abdecken soll, darf Sonderlogik nicht hart in der Basis stecken.

## 2. Schalter statt impliziter Annahmen
Sicherheits- oder Speziallogik muss an klar benannte Konfigurationsschalter gebunden sein.

## 3. Kommentare müssen zur tatsächlichen Logik passen
Ein Config-Flag ist wertlos, wenn die Implementierung es ignoriert.

## 4. Neue KI-Stände werden nie blind übernommen
Jeder Stand wird geprüft auf:
- Architekturtreue
- Wiederverwendbarkeit
- Geheimnisfreiheit
- Versionskonsistenz
- Trennung Basis/Sondergerät
