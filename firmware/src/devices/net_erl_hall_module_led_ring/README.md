# NET-ERL Hall Module LED Ring

Concrete hall-module variant with LED ring for the `net_erl` base type.

## Core behavior

- hall-module workflow with relay control, local button input, presence-triggered auto-on, lux lockout, and delayed auto-off
- hall-module pin line with LD2410C, VEML7700, BME680, ENS160, NeoPixel ring, and relay output

## Visible state

- `relay_1`
- `motion`
- `lux`
- `temp_01c`
- `hum_01pct`
- `pressure_pa`
- `gas_ohm`
- `aqi`
- `tvoc_ppb`
- `eco2_ppm`
- `fault`

## Notes

- `gas_ohm` is the raw BME680 gas reading and only meaningful after warm-up.
- `aqi`, `tvoc_ppb`, and `eco2_ppm` come from the ENS160.
