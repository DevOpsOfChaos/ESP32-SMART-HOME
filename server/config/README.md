# Server Konfigurationsvorlagen

V1 versioniert hier genau die oeffentlichen Basisvorlagen, nicht dein lokales Setup.

Offizielle versionierte Vorlagen:

- `../.env.example`: einzige Env-Vorlage fuer lokale Werte
- `mosquitto/mosquitto.conf`: einzige versionierte Mosquitto-Basis fuer V1

Lokale MQTT-Auth wird absichtlich nicht als zweite offizielle Vorlage versioniert.
Der Repo-Default in `mosquitto/mosquitto.conf` bleibt mit `allow_anonymous true` startbar.
Fuer lokale Auth legst du nur im ignorierten Ordner `mosquitto/config/` eigene Dateien an, zum Beispiel:

- `mosquitto/config/10-auth.conf`
- `mosquitto/config/passwd`

`10-auth.conf` ist der lokale Umschalter auf Passwortpflicht, zum Beispiel mit:

```conf
allow_anonymous false
password_file /mosquitto/data/passwd
```

Die lokale Datei `mosquitto/config/passwd` bleibt nur die Quelle.
Beim Containerstart kopiert Compose sie nach `/mosquitto/data/passwd` und setzt dort die Laufzeit-Rechte fuer Mosquitto.

Lokal und bewusst nicht versioniert:

- `../.env`
- `.env`
- `mosquitto/config/`
- `mosquitto/config/passwd`
- `mosquitto/config/10-auth.conf`
- echte Passwoerter, Tokens und private Host- oder Pfad-Overrides
