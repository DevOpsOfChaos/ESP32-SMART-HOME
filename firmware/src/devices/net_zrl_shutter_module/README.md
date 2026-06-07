# NET-ZRL Shutter Module

Standalone firmware for the concrete shutter module with master communication.

- `main.cpp`: ESP-NOW, master binding, provisioning, calibration, relay interlock, buttons, LEDs, and serial diagnostics
- `DeviceConfig.h`: device ID, pin mapping, and level logic
- `NetZrlProvisioning.h`: setup portal configuration for this device

## Acceptance check

1. Build the device firmware: `pio run -e net_zrl_shutter_module`
2. Build the master: `pio run -e master_firmware`
3. First boot without a stored master MAC should enter setup mode.
4. Set the master MAC in setup, leave setup, then verify HELLO/STATE in the master.
5. Test the local buttons: up, down, stop. Relay dead time must apply on direction changes.
6. Start calibration by holding stop and accept the travel times.
7. Verify master commands: open, close, stop, set_position.
