# Arbeitsregeln Repo und Privates

Diese Trennung ist verbindlich:

- Oeffentliche Dateien gehoeren ins Repository.
- Private Konfigurationen bleiben lokal und werden ignoriert.
- OneDrive ist kein Git-Arbeitsverzeichnis.

Praktische Folgen:

- Beispiel-Dateien wie `Secrets.example.h` und `.env.example` duerfen versioniert werden.
- Echte lokale Dateien wie `Secrets.h`, `.env`, Passwortdateien, Tokens oder WLAN-Zugaenge duerfen nicht ins Repo.
- Begleitdokumente, Exporte, ZIP-Backups und sonstige Ablagen koennen in OneDrive liegen, aber nicht als aktive Git-Arbeitskopie.
