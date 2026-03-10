# Hardware und Geräte

## Basisgeräte
- `NET-ERL` – netzbetrieben, ein Relais
- `NET-ZRL` – netzbetrieben, zwei Relais
- `NET-SEN` – netzbetriebener Sensor-Node
- `BAT-SEN` – batteriebetriebener Sensor-/Event-Node

## Bereits vorgesehene Sondergeräte

### Rolladensteuerung
Basisklasse:
- `NET-ZRL`

Zusätze:
- DHT22
- drei Touch-Buttons
- Richtungs-LEDs
- Kalibrierungsanzeige

### Küchenlampe
Basisklasse:
- `NET-ERL`

Zusätze:
- Temperatur
- Feuchte
- Luftqualität
- Lichtwert
- Motion-Radar
- Taster
- 17x WS2812

### Flurlicht
Basisklasse:
- `NET-ERL`

Zusätze:
- PIR
- Lichtwert
- Temperatur
- Feuchte
- lokaler Taster

### Außenlicht
Basisklasse:
- `NET-ZRL`

Zusätze:
- Temperatur
- Feuchte
- später finaler Präsenzsensor

## Arbeitsregel
Konkrete Pinouts und Sensorvarianten werden erst nach sauberer Hardwareentscheidung finalisiert.
Dieses Dokument bleibt deshalb bewusst architekturseitig und nicht künstlich scheinpräzise.
