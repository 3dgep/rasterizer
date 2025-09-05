#pragma once

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>

#include <array>

namespace sr
{
inline namespace input
{
struct KeyboardState
{
    bool isKeyDown( SDL_Scancode key ) const noexcept;

    bool isKeyUp( SDL_Scancode key ) const noexcept;

    bool isModDown( SDL_Keymod mod ) const noexcept;

    bool isModUp( SDL_Keymod mod ) const noexcept;

    bool operator[]( SDL_Scancode key ) const noexcept
    {
        return keys[key];
    }

    bool operator[]( SDL_Keymod mod ) const noexcept {
        return ( mods & mod ) != 0;
    }

    bool operator==( const KeyboardState& rhs ) const = default;
    bool operator!=( const KeyboardState& rhs ) const = default;

    std::array<bool, SDL_SCANCODE_COUNT> keys; // Keyboard keys.
    SDL_Keymod                           mods; // Modifier keys.
};

namespace Keyboard
{
KeyboardState getState();
}

}  // namespace input
}  // namespace sr