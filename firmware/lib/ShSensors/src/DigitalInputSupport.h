#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace SmartHome {
namespace ShSensors {

constexpr uint8_t DIGITAL_INPUT_STATE_ACTIVE = 0x01U;
constexpr uint8_t DIGITAL_INPUT_STATE_HOLD = 0x02U;
constexpr uint8_t DIGITAL_INPUT_STATE_LONG_REPORTED = 0x04U;

constexpr uint8_t DIGITAL_INPUT_EVENT_ACTIVATED = 0x01U;
constexpr uint8_t DIGITAL_INPUT_EVENT_DEACTIVATED = 0x02U;
constexpr uint8_t DIGITAL_INPUT_EVENT_LONG_HOLD = 0x04U;

struct DigitalInputConfig {
    int pin;
    bool active_low;
    bool use_pullup;
    unsigned long debounce_ms;
    unsigned long hold_ms;
};

struct DigitalInputUpdate {
    bool changed;
    bool active;
    uint8_t state_flags;
    uint8_t event_flags;
    uint16_t stable_ms;
    uint16_t active_ms;

    bool activated() const {
        return (event_flags & DIGITAL_INPUT_EVENT_ACTIVATED) != 0U;
    }

    bool deactivated() const {
        return (event_flags & DIGITAL_INPUT_EVENT_DEACTIVATED) != 0U;
    }

    bool longHold() const {
        return (event_flags & DIGITAL_INPUT_EVENT_LONG_HOLD) != 0U;
    }

    bool holdActive() const {
        return (state_flags & DIGITAL_INPUT_STATE_HOLD) != 0U;
    }
};

struct DigitalInputMaskState {
    uint8_t active_mask;
    uint8_t hold_mask;
    uint8_t long_mask;
};

inline void resetDigitalInputMaskState(DigitalInputMaskState& state) {
    state.active_mask = 0U;
    state.hold_mask = 0U;
    state.long_mask = 0U;
}

inline void applyDigitalInputMaskUpdate(DigitalInputMaskState& state, uint8_t bit, const DigitalInputUpdate& update) {
    if (update.active) {
        state.active_mask |= bit;
    }
    if ((update.state_flags & DIGITAL_INPUT_STATE_HOLD) != 0U) {
        state.hold_mask |= bit;
    }
    if ((update.state_flags & DIGITAL_INPUT_STATE_LONG_REPORTED) != 0U) {
        state.long_mask |= bit;
    }
}

class DigitalInputChannel {
public:
    explicit DigitalInputChannel(const DigitalInputConfig& config = {-1, true, true, 0UL, 0UL})
        : config_(config) {}

    void configure(const DigitalInputConfig& config) {
        config_ = config;
        ready_ = false;
        raw_active_ = false;
        stable_active_ = false;
        long_reported_ = false;
        raw_changed_ms_ = 0UL;
        stable_since_ms_ = 0UL;
        active_since_ms_ = 0UL;
    }

    void begin(unsigned long nowMs = millis()) {
        if (config_.pin >= 0) {
            pinMode(config_.pin, config_.use_pullup ? INPUT_PULLUP : INPUT);
        }

        ready_ = true;
        raw_active_ = readRawActive();
        stable_active_ = raw_active_;
        long_reported_ = false;
        raw_changed_ms_ = nowMs;
        stable_since_ms_ = nowMs;
        active_since_ms_ = stable_active_ ? nowMs : 0UL;
    }

    DigitalInputUpdate poll(unsigned long nowMs = millis()) {
        if (!ready_) {
            begin(nowMs);
        }

        if (config_.pin < 0) {
            return {false, false, 0U, 0U, 0U, 0U};
        }

        const bool rawActive = readRawActive();
        if (rawActive != raw_active_) {
            raw_active_ = rawActive;
            raw_changed_ms_ = nowMs;
        }

        DigitalInputUpdate update = {
            false,
            stable_active_,
            buildStateFlags(nowMs),
            0U,
            clampElapsedMs(nowMs - stable_since_ms_),
            stable_active_ ? clampElapsedMs(nowMs - active_since_ms_) : static_cast<uint16_t>(0U),
        };

        if (raw_active_ != stable_active_ && (nowMs - raw_changed_ms_) >= config_.debounce_ms) {
            stable_active_ = raw_active_;
            stable_since_ms_ = nowMs;
            update.changed = true;

            if (stable_active_) {
                active_since_ms_ = nowMs;
                long_reported_ = false;
                update.event_flags |= DIGITAL_INPUT_EVENT_ACTIVATED;
                update.active_ms = 0U;
            } else {
                update.event_flags |= DIGITAL_INPUT_EVENT_DEACTIVATED;
                update.active_ms = clampElapsedMs(nowMs - active_since_ms_);
                long_reported_ = false;
            }
        }

        if (stable_active_ && config_.hold_ms > 0UL) {
            const unsigned long activeForMs = nowMs - active_since_ms_;
            if (!long_reported_ && activeForMs >= config_.hold_ms) {
                long_reported_ = true;
                update.changed = true;
                update.event_flags |= DIGITAL_INPUT_EVENT_LONG_HOLD;
                update.active_ms = clampElapsedMs(activeForMs);
            }
        }

        update.active = stable_active_;
        update.state_flags = buildStateFlags(nowMs);
        update.stable_ms = clampElapsedMs(nowMs - stable_since_ms_);
        if (stable_active_) {
            update.active_ms = clampElapsedMs(nowMs - active_since_ms_);
        } else if (!update.deactivated()) {
            update.active_ms = 0U;
        }

        return update;
    }

private:
    static uint16_t clampElapsedMs(unsigned long elapsedMs) {
        return elapsedMs > 0xFFFFUL ? 0xFFFFU : static_cast<uint16_t>(elapsedMs);
    }

    uint8_t buildStateFlags(unsigned long nowMs) const {
        uint8_t flags = stable_active_ ? DIGITAL_INPUT_STATE_ACTIVE : 0U;
        if (stable_active_ && config_.hold_ms > 0UL && (nowMs - active_since_ms_) >= config_.hold_ms) {
            flags |= DIGITAL_INPUT_STATE_HOLD;
        }
        if (stable_active_ && long_reported_) {
            flags |= DIGITAL_INPUT_STATE_LONG_REPORTED;
        }
        return flags;
    }

    bool readRawActive() const {
        if (config_.pin < 0) {
            return false;
        }

        const bool levelHigh = digitalRead(config_.pin) == HIGH;
        return config_.active_low ? !levelHigh : levelHigh;
    }

    DigitalInputConfig config_;
    bool ready_ = false;
    bool raw_active_ = false;
    bool stable_active_ = false;
    bool long_reported_ = false;
    unsigned long raw_changed_ms_ = 0UL;
    unsigned long stable_since_ms_ = 0UL;
    unsigned long active_since_ms_ = 0UL;
};

}  // namespace ShSensors
}  // namespace SmartHome
