# Minimalstrecke net_erl_01 <-> master

## Kurzbeschreibung
Die erste lauffaehige Vertikalstrecke ist bewusst auf genau ein Pilotgeraet begrenzt:

- `net_erl_01` sendet beim Start `HELLO`
- nach `HELLO_ACK` sendet `net_erl_01` einen `STATE_REPORT`
- danach haelt `HEARTBEAT` den Node online
- `master` publiziert Status und Zustand nach MQTT
- MQTT-`set_relay` wird als `COMMAND_SET_RELAY` an `net_erl_01` zurueckgesendet
- nach Relaisaenderung sendet `net_erl_01` einen neuen `STATE_REPORT`

MQTT-Topics der Minimalstrecke:

- `smarthome/master/status`
- `smarthome/node/net_erl_01/status`
- `smarthome/node/net_erl_01/state`
- `smarthome/node/net_erl_01/command`

## Geaenderte Dateien
- `firmware/platformio.ini`
- `firmware/lib/ShProtocol/src/Protocol.h`
- `firmware/src/master/AppConfig.h`
- `firmware/src/master/main.cpp`
- `firmware/src/net_erl/AppConfig.h`
- `firmware/src/net_erl/main.cpp`
- `docs/03_protokoll.md`
- `docs/14_minimalstrecke_net_erl_master.md`
- `PROTOKOLL/beta03_minimalstrecke_net_erl_master.txt`
- `README.md`

## Testablauf
1. `master` flashen und seriellen Monitor oeffnen.
2. Lokale `Secrets.h` mit gueltigem WLAN und MQTT-Broker bereitstellen.
3. MQTT-Broker starten und `smarthome/master/status`, `smarthome/node/net_erl_01/status` und `smarthome/node/net_erl_01/state` abonnieren.
4. `net_erl` flashen und seriellen Monitor oeffnen.
5. Pruefen, dass `master` `HELLO` von `net_erl_01` loggt und den Node online publiziert.
6. Pruefen, dass nach `HELLO_ACK` ein `STATE_REPORT` in MQTT ankommt.
7. MQTT-Command senden:

```json
{"cmd":"set_relay","relay_1":true,"request_id":"cmd_0001"}
```

8. Pruefen, dass `master` den JSON-Befehl loggt, `COMMAND_SET_RELAY` per ESP-NOW sendet und `net_erl` den Relaiszustand logisch oder physisch aendert.
9. Pruefen, dass der aktualisierte `STATE_REPORT` wieder auf `smarthome/node/net_erl_01/state` landet.
10. `net_erl` ausschalten oder Funk trennen und pruefen, dass `master` nach rund 15 Sekunden offline publiziert.

## Bekannte Grenzen
- genau ein Pilot-Node: `net_erl_01`
- keine Unterstuetzung fuer `net_zrl`
- kein Cover-Modus
- keine Weboberflaeche
- kein generisches Registry-/Mehrgeraete-Handling
- keine ACK-/Retry-Logik fuer Commands
- `net_erl` arbeitet ohne lokale Taster- oder Komfortlogik
