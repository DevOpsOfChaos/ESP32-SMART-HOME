# Implementierungsphasen

## Phase 0 – Fundament
- Repo-Struktur
- Dokumente
- Git-/Secrets-Regeln
- Platzhalter für Firmware und Server

## Phase 1 – Basisprotokoll
- Header
- Nachrichtentypen
- Sequenznummern
- ACK-Mechanik
- Retry-Limits
- Duplikaterkennung

## Phase 2 – Firmware-Basis
- Master-Grundgerüst
- `NET-ERL`
- `NET-ZRL`
- `NET-SEN`
- `BAT-SEN`
- gemeinsamer Konfigurationspfad
- Setup-Portal-Grundstruktur

## Phase 3 – End-to-End
- Node -> Master -> MQTT
- MQTT -> Master -> Node
- retained / non-retained Verhalten
- Availability

## Phase 4 – Erste reale Beispielgeräte
- einfache Lampe
- Rolladen-Grundfunktion
- Standardsensor
- Fensterkontakt oder Wandschalter

## Phase 5 – Sondergeräte
- Küchenlampe
- Flurlicht
- Außenlicht
- weitere BAT-SEN-Varianten

## Phase 6 – Härtung
- Verschlüsselung
- Robustheit
- Testautomatisierung
- bessere Serverlogik
