# Modular Control Board - KiCad Project

> Shared control board for all relay nodes (`net_erl` and `net_zrl`)

## Files

| File | Description |
|---|---|
| `steuerkreis.kicad_pro` | KiCad 9.0 project file |
| `steuerkreis.kicad_sch` | schematic (flat schematic) |
| `steuerkreis.kicad_pcb` | 2-layer PCB layout, 1.6 mm |
| `mod_Steuerkreis_ESP32.kicad_prl` | PCB rules |
| `steuerkreis.csv` | bill of materials |
| `steuerkreis.step` | 3D model (STEP export) |
| `Pinbelegung/Pinbelegung.txt` | GPIO map |
| `production/` | manufacturing data (BOM, placement, netlist) |

## Usage

This control board is combined with two different power boards:

- **[net_erl](../../net_erl/leistungskreis/)** - 1-relay power board
- **[net_zrl](../../net_zrl/leistungskreis/)** - 2-relay power board

The connection uses a 5-pin JST-PH cable.
