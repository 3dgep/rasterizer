#pragma once

#include "Keyboard.hpp"

namespace sr
{
inline namespace input
{
struct KeyboardStateTracker
{
    KeyboardState pressed;   ///< True fi the key was pressed this frame.
    KeyboardState released;  ///< True if the key was released this frame.

    KeyboardStateTracker() noexcept;

    bool update( const KeyboardState& state ) noexcept;

    void reset() noexcept;

    bool isKeyPressed( SDL_Scancode key ) const noexcept;

    bool isKeyReleased( SDL_Scancode key ) const noexcept;

    [[nodiscard]] KeyboardState getLastState() const noexcept;

private:
    KeyboardState lastState;
};
}  // namespace input
}  // namespace sr