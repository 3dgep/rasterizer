#include <input/KeyboardStateTracker.hpp>

using namespace sr::input;

KeyboardStateTracker::KeyboardStateTracker() noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    reset();
}

bool KeyboardStateTracker::update( const KeyboardState& state ) noexcept
{
    for ( size_t i = 0; i < lastState.keys.size(); ++i )
    {
        pressed.keys[i]  = state.keys[i] && !lastState.keys[i];
        released.keys[i] = !state.keys[i] && lastState.keys[i];
    }

    if ( lastState != state )
    {
        lastState = state;
        return true;
    }

    return false;
}

void KeyboardStateTracker::reset() noexcept
{
    memset( this, 0, sizeof( KeyboardStateTracker ) );
}

bool KeyboardStateTracker::isKeyPressed( SDL_Scancode key ) const noexcept
{
    return pressed.isKeyDown( key );
}

bool KeyboardStateTracker::isKeyReleased( SDL_Scancode key ) const noexcept
{
    return released.isKeyDown( key );
}

KeyboardState KeyboardStateTracker::getLastState() const noexcept
{
    return lastState;
}