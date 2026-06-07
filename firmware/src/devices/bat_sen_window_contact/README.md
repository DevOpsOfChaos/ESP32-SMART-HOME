# bat_sen_window_contact

The first concrete device built on the generic `bat_sen` base type.

## Includes

- fixed device identity and default values
- dedicated contact pin (`GPIO3`) with wake pin mapping
- simple contact semantics: `open` or `closed`
- event mapping to `SH_EVENT_WINDOW_OPENED` and `SH_EVENT_WINDOW_CLOSED`
- ESP32-C3 GPIO deep-sleep wakeup via the device pin
- RTC state storage for window events after wake-up

## Deliberately does not include

- rain logic
- profile multiplexing
- device-specific special paths beyond the window contact

## Wake strategy for the C3

- GPIO wakeup is limited to RTC-capable C3 pins (`GPIO0..GPIO5`).
- The device therefore uses `GPIO3` instead of the boot button pin `GPIO9`.
- Wake is level-based, but the firmware sets the target level before each deep sleep to match the current contact state.
- Closed -> open and open -> closed both wake immediately.
- The last contact state stays in RTC memory so the wake can be reported as a window event.

## Setup button

- A short press toggles stay-awake mode during normal operation. When active, the device will not enter deep sleep.
- A second short press allows deep sleep again; after the RX window expires, the device may sleep normally.
- Holding the button for 5 seconds starts setup mode.
