#include <input/Keyboard.hpp>

#include <SDL3/SDL_keyboard.h>

using namespace sr::input;

namespace Keyboard
{
sr::KeyboardState getState()
{
    sr::KeyboardState state {};

    int         numKeys = 0;
    const bool* keys    = SDL_GetKeyboardState( &numKeys );

    std::memcpy( state.keys.data(), keys, sizeof( bool ) * numKeys );

    return state;
}
}  // namespace Keyboard

bool KeyboardState::isKeyDown( SDL_Scancode key ) const noexcept
{
    return keys[key];
}

bool KeyboardState::isKeyUp( SDL_Scancode key ) const noexcept
{
    return !keys[key];
}
