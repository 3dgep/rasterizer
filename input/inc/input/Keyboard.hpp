#pragma once

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_keycode.h>

#include <array>

namespace sr
{
inline namespace input
{
struct KeyboardState
{
    bool isKeyDown( SDL_Scancode key ) const noexcept;

    bool isKeyUp( SDL_Scancode key ) const noexcept;

    bool operator[]( SDL_Scancode key ) const noexcept
    {
        return isKeyDown( key );
    }

    bool operator==( const KeyboardState& rhs ) const = default;
    bool operator!=( const KeyboardState& rhs ) const = default;

    std::array<bool, SDL_SCANCODE_COUNT> keys;
};

namespace Keyboard
{
KeyboardState getState();
}

}  // namespace input
}  // namespace sr