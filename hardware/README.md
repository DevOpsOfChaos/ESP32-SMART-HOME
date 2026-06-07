# Hardware

This folder contains the physical side of the project:

- shared reference hardware
- device-specific PCB designs
- manufacturing exports
- enclosure and 3D-print models

## Start here

- [Hardware overview](00_hardwareueberblick.md)

## Main areas

- `shared/` holds reusable circuit blocks and reference components
- `bat_sen/`, `net_sen/`, `net_erl/`, and `net_zrl/` hold device families
- `3d_models/` holds enclosure work, renders, and printable models
- `fabrication/` holds production-ready exports brought in from the final project state

## How to read it

Start with the shared hardware, then move into the relevant device family. Do not assume the whole system is required. Each family is meant to stand on its own.

## Fabrication exports

The `fabrication/` folder is the handoff layer:

- `gerbers/` for PCB production
- `kicad/` for board source files and exports
- `schematics/` for PDFs and schematic handoff material
- `bom/` for bill-of-materials CSV files

## 3D printing

The enclosure work under `3d_models/` is intentionally separate from the PCB source. You can use the boards without the enclosures, or the enclosures without the full software stack.
