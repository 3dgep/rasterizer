#include <input/Keyboard.hpp>

#include <SDL3/SDL_keyboard.h>

using namespace sr::input;

bool KeyboardState::isKeyDown( SDL_Scancode key ) const noexcept
{
    return keys[key];
}

bool KeyboardState::isKeyUp( SDL_Scancode key ) const noexcept
{
    return !keys[key];
}

bool KeyboardState::isModDown( SDL_Keymod mod ) const noexcept
{
    return ( mods & mod ) != 0;
}

bool KeyboardState::isModUp( SDL_Keymod mod ) const noexcept
{
    return ( mods & mod ) == 0;
}

KeyboardState Keyboard::getState()
{
    KeyboardState state {};

    int         numKeys = 0;
    const bool* keys    = SDL_GetKeyboardState( &numKeys );

    std::memcpy( state.keys.data(), keys, sizeof( bool ) * numKeys );

    state.mods = SDL_GetModState();

    return state;
}
