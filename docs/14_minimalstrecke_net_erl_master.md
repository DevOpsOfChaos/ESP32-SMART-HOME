# Minimalstrecke net_erl_01 <-> master

## Rolle dieses Dokuments
Dieses Dokument beschreibt die technische Minimalstrecke und einen manuellen Pruefablauf. Es ist kein globales Nachweisdokument.

Belegter Ist-Stand:
- `docs/14_test_und_nachweisstand.md`
- Der kombinierte serverseitige Live-Nachweis fuer `net_erl_01` ist dort und in `PROTOKOLL/beta14_kombinierter_live_nachweis_offizieller_rueckweg_net_erl_01_aktueller_repo_stand.txt` getrennt verankert; dieses Dokument bleibt trotzdem nur das Detailblatt der Minimalstrecke.

## Kurzbeschreibung
Die erste lauffaehige Vertikalstrecke ist bewusst auf genau ein Pilotgeraet begrenzt:

- `net_erl_01` sendet beim Start `HELLO`
- nach `HELLO_ACK` sendet `net_erl_01` einen `STATE_REPORT`
- danach haelt `HEARTBEAT` den Node online
- `master` publiziert Status und Zustand nach MQTT
- MQTT-`set_relay` wird als `COMMAND_SET_RELAY` an `net_erl_01` zurueckgesendet
- nach Relaisaenderung sendet `net_erl_01` einen neuen `STATE_REPORT`

Erweiterungsstand derselben Firmware-Linie:

- `master` kennt jetzt zusaetzlich die festen Basisnodes `net_zrl_01`, `net_sen_01` und `bat_sen_01`
- diese drei Basen sind im Code und Build integriert, aber in diesem Dokument nicht als real verifizierte Minimalstrecke behauptet
- die reale Nachweisstrecke dieses Dokuments bleibt `net_erl_01`

MQTT-Topics der Minimalstrecke:

- `smarthome/master/MASTER-001/status`
- `smarthome/node/net_erl_01/meta`
- `smarthome/node/net_erl_01/status`
- `smarthome/node/net_erl_01/state`
- `smarthome/node/net_erl_01/cmd/set`
- `smarthome/node/net_erl_01/cmd/get`

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
3. MQTT-Broker starten und `smarthome/master/MASTER-001/status`, `smarthome/node/net_erl_01/meta`, `smarthome/node/net_erl_01/status` und `smarthome/node/net_erl_01/state` abonnieren.
4. `net_erl` flashen und seriellen Monitor oeffnen.
5. Pruefen, dass `master` `HELLO` von `net_erl_01` loggt und den Node online publiziert.
6. Pruefen, dass nach `HELLO_ACK` ein `STATE_REPORT` in MQTT ankommt.
7. MQTT-Command an Topic `smarthome/node/net_erl_01/cmd/set` senden:

```json
{"cmd":"set_relay","relay_1":true,"request_id":"cmd_0001"}
```

8. Pruefen, dass `master` den JSON-Befehl loggt, `COMMAND_SET_RELAY` per ESP-NOW sendet und `net_erl` den Relaiszustand logisch oder physisch aendert.
9. Pruefen, dass der aktualisierte `STATE_REPORT` wieder auf `smarthome/node/net_erl_01/state` landet.
10. `net_erl` ausschalten oder Funk trennen und pruefen, dass `master` nach rund 15 Sekunden offline publiziert.

## Bekannte Grenzen
- reale Nachweisstrecke dieses Dokuments bleibt genau ein Pilot-Node: `net_erl_01`
- der separat belegte kombinierte Live-Nachweis aendert die Rolle dieses Dokuments nicht: kein Gesamtprojektnachweis
- keine offene Mehrgeraete-Registry; im Master sind vier feste Basisnodes hinterlegt:
  `net_erl_01`, `net_zrl_01`, `net_sen_01`, `bat_sen_01`
- kein Cover-Modus
- keine Weboberflaeche
- kein generisches Registry-/Mehrgeraete-Handling
- keine ACK-/Retry-Logik fuer Commands
- der hier beschriebene Offline-Timeout-Pruefschritt ist kein eigener offizieller Ist-Nachweis im aktuellen Repo-Stand
- `net_erl` arbeitet ohne lokale Taster- oder Komfortlogik
- `net_zrl`, `net_sen` und `bat_sen` sind in diesem Stand nur build- und codeintegriert, nicht real auf Hardware nachgewiesen
