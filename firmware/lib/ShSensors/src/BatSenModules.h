#pragma once

#include <Arduino.h>
#include <stdlib.h>

#include "DigitalInputSupport.h"
#include "ProviderIds.h"
#include "../../ShProtocol/src/DeviceTypes.h"

namespace SmartHome {
namespace ShSensors {

constexpr uint8_t BAT_SEN_WINDOW_UNGUELTIG = 0xFFU;
constexpr uint16_t BAT_SEN_RAIN_UNGUELTIG = 0xFFFFU;

struct BatSenInputState {
    bool window_open;
    DigitalInputMaskState button_state;
    uint16_t rain_raw;
};

struct ReedProviderConfig {
    int pin;
    bool active_low;
    bool use_pullup;
    unsigned long debounce_ms;
};

struct ButtonProviderConfig {
    int pin_1;
    int pin_2;
    int pin_3;
    int pin_4;
    bool active_low;
    bool use_pullup;
    unsigned long debounce_ms;
    unsigned long hold_ms;
};

struct RainProviderConfig {
    int pin;
    uint16_t delta_raw;
};

struct ReedUpdate {
    bool changed;
    bool window_open;
    uint8_t state_flags;
};

struct ButtonUpdate {
    bool changed;
    DigitalInputMaskState state;
    uint8_t press_mask;
    uint8_t release_mask;
    uint8_t long_press_mask;
    uint16_t duration_ms[4];
};

struct RainUpdate {
    bool changed;
    uint16_t rain_raw;
};

inline void resetBatSenInputs(BatSenInputState& state) {
    state.window_open = false;
    resetDigitalInputMaskState(state.button_state);
    state.rain_raw = BAT_SEN_RAIN_UNGUELTIG;
}

template<uint8_t REED_PROVIDER_KIND, uint8_t BUTTON_PROVIDER_KIND, uint8_t RAIN_PROVIDER_KIND, uint8_t BUTTON_CHANNEL_COUNT>
constexpr uint16_t buildBatSenCaps() {
    return SH_CAP_BATTERY |
           ((REED_PROVIDER_KIND != SH_BAT_SEN_REED_PROVIDER_NONE) ? SH_CAP_WINDOW : 0U) |
           ((BUTTON_PROVIDER_KIND != SH_BAT_SEN_BUTTON_PROVIDER_NONE)
                ? ((BUTTON_CHANNEL_COUNT <= 1U) ? SH_CAP_BUTTON : SH_CAP_MULTIBUTTON)
                : 0U) |
           ((RAIN_PROVIDER_KIND != SH_BAT_SEN_RAIN_PROVIDER_NONE) ? SH_CAP_RAIN : 0U);
}

template<uint8_t ProviderKind>
class ReedProvider;

template<>
class ReedProvider<SH_BAT_SEN_REED_PROVIDER_NONE> {
public:
    explicit ReedProvider(const ReedProviderConfig&) {}

    void begin() {}
    const char* name() const { return "none"; }

    ReedUpdate poll() {
        return {false, false, 0U};
    }
};

template<>
class ReedProvider<SH_BAT_SEN_REED_PROVIDER_PIN> {
public:
    explicit ReedProvider(const ReedProviderConfig& config)
        : input_({config.pin, config.active_low, config.use_pullup, config.debounce_ms, 0UL}) {}

    void begin() {
        input_.begin();
    }

    const char* name() const { return "reed_pin"; }

    ReedUpdate poll() {
        const DigitalInputUpdate update = input_.poll();
        return {update.changed, update.active, update.state_flags};
    }

private:
    DigitalInputChannel input_;
};

template<>
class ReedProvider<SH_BAT_SEN_REED_PROVIDER_STUB> {
public:
    explicit ReedProvider(const ReedProviderConfig&) {}

    void begin() {}
    const char* name() const { return "reed_stub"; }

    ReedUpdate poll() {
        return {false, false, 0U};
    }
};

template<uint8_t ProviderKind>
class ButtonProvider;

template<>
class ButtonProvider<SH_BAT_SEN_BUTTON_PROVIDER_NONE> {
public:
    explicit ButtonProvider(const ButtonProviderConfig&) {}

    void begin() {}
    const char* name() const { return "none"; }

    ButtonUpdate poll() {
        ButtonUpdate update = {};
        return update;
    }
};

template<>
class ButtonProvider<SH_BAT_SEN_BUTTON_PROVIDER_PIN> {
public:
    explicit ButtonProvider(const ButtonProviderConfig& config) {
        inputs_[0].configure({config.pin_1, config.active_low, config.use_pullup, config.debounce_ms, config.hold_ms});
        inputs_[1].configure({config.pin_2, config.active_low, config.use_pullup, config.debounce_ms, config.hold_ms});
        inputs_[2].configure({config.pin_3, config.active_low, config.use_pullup, config.debounce_ms, config.hold_ms});
        inputs_[3].configure({config.pin_4, config.active_low, config.use_pullup, config.debounce_ms, config.hold_ms});
    }

    void begin() {
        for (uint8_t i = 0U; i < 4U; ++i) {
            inputs_[i].begin();
        }
    }

    const char* name() const { return "button_pin"; }

    ButtonUpdate poll() {
        ButtonUpdate update = {};
        resetDigitalInputMaskState(update.state);

        for (uint8_t i = 0U; i < 4U; ++i) {
            const uint8_t bit = static_cast<uint8_t>(1U << i);
            const DigitalInputUpdate channelUpdate = inputs_[i].poll();
            applyDigitalInputMaskUpdate(update.state, bit, channelUpdate);
            update.duration_ms[i] = channelUpdate.active_ms;

            if (channelUpdate.activated()) {
                update.press_mask |= bit;
            }
            if (channelUpdate.deactivated()) {
                update.release_mask |= bit;
            }
            if (channelUpdate.longHold()) {
                update.long_press_mask |= bit;
            }
            if (channelUpdate.changed) {
                update.changed = true;
            }
        }

        return update;
    }

private:
    DigitalInputChannel inputs_[4];
};

template<>
class ButtonProvider<SH_BAT_SEN_BUTTON_PROVIDER_STUB> {
public:
    explicit ButtonProvider(const ButtonProviderConfig&) {}

    void begin() {}
    const char* name() const { return "button_stub"; }

    ButtonUpdate poll() {
        ButtonUpdate update = {};
        return update;
    }
};

template<uint8_t ProviderKind>
class RainProvider;

template<>
class RainProvider<SH_BAT_SEN_RAIN_PROVIDER_NONE> {
public:
    explicit RainProvider(const RainProviderConfig&) {}

    void begin() {}
    const char* name() const { return "none"; }

    RainUpdate poll() {
        return {false, BAT_SEN_RAIN_UNGUELTIG};
    }
};

template<>
class RainProvider<SH_BAT_SEN_RAIN_PROVIDER_ADC> {
public:
    explicit RainProvider(const RainProviderConfig& config) : pin_(config.pin), delta_raw_(config.delta_raw) {}

    void begin() {
        if (pin_ >= 0) {
            adcAttachPin(pin_);
        }
    }

    const char* name() const { return "rain_adc"; }

    RainUpdate poll() {
        if (pin_ < 0) {
            return {false, BAT_SEN_RAIN_UNGUELTIG};
        }

        int raw = analogRead(pin_);
        if (raw < 0) raw = 0;
        if (raw > 4095) raw = 4095;

        const uint16_t currentRaw = static_cast<uint16_t>(raw);
        const bool changed =
            last_raw_ == BAT_SEN_RAIN_UNGUELTIG ||
            abs((int)currentRaw - (int)last_raw_) >= delta_raw_;
        last_raw_ = currentRaw;
        return {changed, currentRaw};
    }

private:
    int pin_ = -1;
    uint16_t delta_raw_ = 0U;
    uint16_t last_raw_ = BAT_SEN_RAIN_UNGUELTIG;
};

template<>
class RainProvider<SH_BAT_SEN_RAIN_PROVIDER_STUB> {
public:
    explicit RainProvider(const RainProviderConfig&) {}

    void begin() {}
    const char* name() const { return "rain_stub"; }

    RainUpdate poll() {
        return {false, 2048U};
    }
};

}  // namespace ShSensors
}  // namespace SmartHome
