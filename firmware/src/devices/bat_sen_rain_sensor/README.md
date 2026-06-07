# BAT-SEN Rain Sensor

Concrete BAT-SEN device with a simple ADC-based rain input.

## V1 semantics

- `channel_bool_1`: `1 = wet`, `0 = dry`
- `channel_u16_1`: raw ADC value (`rain_raw`)
- Event on state change:
  - `event_type = SH_EVENT_RAIN_DETECTED`
  - `param1 = 1` when switching to `wet`
  - `param1 = 0` when switching to `dry`
  - `param2 = current ADC raw value`

## Notes

- The protocol currently only exposes `SH_EVENT_RAIN_DETECTED`.
- For that reason, `param1` carries the target state of the transition.
- The threshold is still a provisional V1 default and must be validated on real hardware.
- V1 remains timer-based, not GPIO-wake-based.
- GPIO8 board LED / WS2812 stays unused and is disabled by the BAT-SEN base type.
