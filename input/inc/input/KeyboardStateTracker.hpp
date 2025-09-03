#pragma once

#include "Keyboard.hpp"

namespace sr
{
inline namespace input
{
struct KeyboardStateTracker
{
    KeyboardState released;
    KeyboardState pressed;

    KeyboardStateTracker() noexcept;

    bool update( const KeyboardState& state ) noexcept;

    void reset() noexcept;

    bool isKeyPressed( SDL_Scancode key ) const noexcept;

    bool isKeyReleased( SDL_Scancode key ) const noexcept;

    [[nodiscard]] KeyboardState getLastState() const noexcept;

private:
    KeyboardState lastState;
};
}
}