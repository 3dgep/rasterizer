#include <input/Gamepad.hpp>

#include <algorithm>  // for std::clamp
#include <climits>    // for UINT_MAX
#include <cmath>      // for std::sqrt

using namespace sr::input;

namespace
{
float ApplyLinearDeadZone( float value, float maxValue, float deadZoneSize = 0.0f ) noexcept
{
    if ( value < -deadZoneSize )
    {
        // Increase negative values to remove the dead-zone discontinuity.
        value += deadZoneSize;
    }
    else if ( value > deadZoneSize )
    {
        // Decrease positive values to remove the dead-zone discontinuity.
        value -= deadZoneSize;
    }
    else
    {
        // Values inside the dead-zone come out zero.
        return 0;
    }

    // Scale into 0-1 range.
    const float scaledValue = value / ( maxValue - deadZoneSize );
    return std::clamp( scaledValue, -1.0f, 1.f );
}

void ApplyStickDeadZone( float x, float y, DeadZone deadZoneMode, float maxValue, float deadZoneSize, float& resultX,
                         float& resultY ) noexcept
{
    switch ( deadZoneMode )
    {
    case DeadZone::IndependentAxis:
        resultX = ApplyLinearDeadZone( x, maxValue, deadZoneSize );
        resultY = ApplyLinearDeadZone( y, maxValue, deadZoneSize );
        break;
    case DeadZone::Circular:
    {
        const float dist   = std::sqrt( x * x + y * y );
        const float wanted = ApplyLinearDeadZone( dist, maxValue, deadZoneSize );

        const float scale = ( wanted > 0.f ) ? ( wanted / dist ) : 0.0f;

        resultX = std::clamp( x * scale, -1.0f, 1.f );
        resultY = std::clamp( y * scale, -1.0f, 1.0f );
    }
    break;
    case DeadZone::None:
        resultX = ApplyLinearDeadZone( x, maxValue, 0 );
        resultY = ApplyLinearDeadZone( y, maxValue, 0 );
        break;
    }
}
}  // namespace

Gamepad::Gamepad( int player )
: playerId( player )
{
    pollGamepad();
}

Gamepad::Gamepad( Gamepad&& other ) noexcept
: playerId { std::exchange( other.playerId, -1 )}
, gamepad { std::exchange( other.gamepad, nullptr )}
{}

Gamepad::~Gamepad()
{
    if ( gamepad )
        SDL_CloseGamepad( gamepad );
}

Gamepad& Gamepad::operator=( Gamepad&& rhs ) noexcept
{
    if ( this == &rhs )
        return *this;

    if (gamepad)
        SDL_CloseGamepad( gamepad );

    playerId = std::exchange( rhs.playerId, -1 );
    gamepad  = std::exchange( rhs.gamepad, nullptr );

    return *this;
}

GamepadState Gamepad::getState( DeadZone deadZone )
{
    GamepadState state {};
    if ( !pollGamepad() )
    {
        state.connected = false;
        return state;
    }

    state.connected = true;

    state.buttons.a             = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_SOUTH ) != 0;
    state.buttons.b             = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_EAST ) != 0;
    state.buttons.x             = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_WEST ) != 0;
    state.buttons.y             = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_NORTH ) != 0;
    state.buttons.leftStick     = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_LEFT_STICK ) != 0;
    state.buttons.rightStick    = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_RIGHT_STICK ) != 0;
    state.buttons.leftShoulder  = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER ) != 0;
    state.buttons.rightShoulder = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER ) != 0;
    state.buttons.back          = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_BACK ) != 0;
    state.buttons.start         = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_START ) != 0;

    state.dPad.up    = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP ) != 0;
    state.dPad.down  = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN ) != 0;
    state.dPad.left  = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT ) != 0;
    state.dPad.right = SDL_GetGamepadButton( gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT ) != 0;

    state.thumbSticks.leftX  = SDL_GetGamepadAxis( gamepad, SDL_GAMEPAD_AXIS_LEFTX );
    state.thumbSticks.leftY  = SDL_GetGamepadAxis( gamepad, SDL_GAMEPAD_AXIS_LEFTY );
    state.thumbSticks.rightX = SDL_GetGamepadAxis( gamepad, SDL_GAMEPAD_AXIS_RIGHTX );
    state.thumbSticks.rightY = SDL_GetGamepadAxis( gamepad, SDL_GAMEPAD_AXIS_RIGHTY );

    state.triggers.left  = SDL_GetGamepadAxis( gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER );
    state.triggers.right = SDL_GetGamepadAxis( gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER );

    float deadzoneSize = deadZone == DeadZone::None ? 0.0f : 30.0f;  // TODO: Check if this is reasonable deadzone size for triggers.

    state.triggers.left  = ApplyLinearDeadZone( state.triggers.left, SDL_JOYSTICK_AXIS_MAX, deadzoneSize );
    state.triggers.right = ApplyLinearDeadZone( state.triggers.right, SDL_JOYSTICK_AXIS_MAX, deadzoneSize );

    ApplyStickDeadZone( state.thumbSticks.leftX, state.thumbSticks.leftY, deadZone, SDL_JOYSTICK_AXIS_MAX, 7849.0f,
                        state.thumbSticks.leftX, state.thumbSticks.leftY );
    ApplyStickDeadZone( state.thumbSticks.rightX, state.thumbSticks.rightY, deadZone, SDL_JOYSTICK_AXIS_MAX, 8689.0f,
                        state.thumbSticks.rightX, state.thumbSticks.rightY );

    return state;
}

bool Gamepad::setVibration( float lowFrequency, float highFrequency, float leftTrigger, float rightTrigger )
{
    return SDL_RumbleGamepad( gamepad, static_cast<Uint16>( lowFrequency * 65535.0f ), static_cast<Uint16>( highFrequency * 65535.0f ), UINT_MAX ) &&
           SDL_RumbleGamepadTriggers( gamepad, static_cast<Uint16>( leftTrigger * 65535.0f ), static_cast<Uint16>( rightTrigger * 65535.0f ), UINT_MAX );
}

bool Gamepad::pollGamepad()
{
    if ( gamepad )
    {
        if ( !SDL_GamepadConnected( gamepad ) )
        {
            // Game pad is disconnected...
            SDL_CloseGamepad( gamepad );
            gamepad = nullptr;
        }
    }
    else
    {
        if ( SDL_IsGamepad( playerId ) )
        {
            gamepad = SDL_OpenGamepad( playerId );
        }
    }

    return gamepad != nullptr;
}