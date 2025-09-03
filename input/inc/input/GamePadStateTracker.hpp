#pragma once

#include "Gamepad.hpp"
#include "ButtonState.hpp"

namespace sr
{
inline namespace input
{
struct GamepadStateTracker
{
    ButtonState a;
    ButtonState b;
    ButtonState x;
    ButtonState y;

    ButtonState leftStick;
    ButtonState rightStick;

    ButtonState leftShoulder;
    ButtonState rightShoulder;

    union
    {
        ButtonState back;
        ButtonState view;
    };

    union
    {
        ButtonState start;
        ButtonState menu;
    };

    ButtonState dPadUp;
    ButtonState dPadDown;
    ButtonState dPadLeft;
    ButtonState dPadRight;

    ButtonState leftStickUp;
    ButtonState leftStickDown;
    ButtonState leftStickLeft;
    ButtonState leftStickRight;

    ButtonState rightStickUp;
    ButtonState rightStickDown;
    ButtonState rightStickLeft;
    ButtonState rightStickRight;

    ButtonState leftTrigger;
    ButtonState rightTrigger;

    GamepadStateTracker() noexcept;

    bool update( const GamepadState& state ) noexcept;

    void reset() noexcept;

    [[nodiscard]] GamepadState getLastState() const noexcept;

private:
    GamepadState lastState;
};

inline GamepadStateTracker::GamepadStateTracker() noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    reset();
}

inline GamepadState GamepadStateTracker::getLastState() const noexcept
{
    return lastState;
}

}  // namespace input
}  // namespace sr