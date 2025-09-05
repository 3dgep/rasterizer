#pragma once

#include <SDL3/SDL_gamepad.h>

namespace sr
{
inline namespace input
{
/// <summary>
/// Dead-zone mode.
/// </summary>
enum class DeadZone
{
    IndependentAxis = 0,
    Circular,
    None,
};

struct GamepadState
{
    struct Buttons
    {
        bool a;
        bool b;
        bool x;
        bool y;
        bool leftStick;
        bool rightStick;
        bool leftShoulder;
        bool rightShoulder;

        union
        {
            bool back;
            bool view;
        };

        union
        {
            bool start;
            bool menu;
        };
    } buttons;

    struct DPad
    {
        bool up;
        bool down;
        bool right;
        bool left;
    } dPad;

    struct ThumbSticks
    {
        float leftX;
        float leftY;
        float rightX;
        float rightY;
    } thumbSticks;

    struct Triggers
    {
        float left;
        float right;
    } triggers;

    bool connected;

    bool isConnected() const noexcept;

    bool isAPressed() const noexcept;
    bool isBPressed() const noexcept;
    bool isXPressed() const noexcept;
    bool isYPressed() const noexcept;

    bool isLeftStickPressed() const noexcept;
    bool isRightStickPressed() const noexcept;

    bool isLeftShoulderPressed() const noexcept;
    bool isRightShoulderPressed() const noexcept;

    bool isBackPressed() const noexcept;
    bool isViewPressed() const noexcept;
    bool isStartPressed() const noexcept;
    bool isMenuPressed() const noexcept;

    bool isDPadDownPressed() const noexcept;
    bool isDPadUpPressed() const noexcept;
    bool isDPadLeftPressed() const noexcept;
    bool isDPadRightPressed() const noexcept;

    bool isLeftThumbStickUp() const noexcept;
    bool isLeftThumbStickDown() const noexcept;
    bool isLeftThumbStickLeft() const noexcept;
    bool isLeftThumbStickRight() const noexcept;

    bool isRightThumbStickUp() const noexcept;
    bool isRightThumbStickDown() const noexcept;
    bool isRightThumbStickLeft() const noexcept;
    bool isRightThumbStickRight() const noexcept;

    bool isLeftTriggerPressed() const noexcept;
    bool isRightTriggerPressed() const noexcept;
};

class Gamepad
{
public:
    static constexpr int MAX_PLAYERS = 4;

    explicit Gamepad( int player );
    Gamepad( const Gamepad& ) = delete;
    Gamepad( Gamepad&& ) noexcept;
    ~Gamepad();

    Gamepad& operator=( const Gamepad& ) = delete;
    Gamepad& operator=( Gamepad&& ) noexcept;

    GamepadState getState( DeadZone deadZone = DeadZone::IndependentAxis );

    bool setVibration( float lowFrequency, float highFrequency, float leftTrigger = 0.0f, float rightTrigger = 0.0f );

private:
    // Poll the gamepad if it is attached or detached.
    bool pollGamepad();

    int          playerId = -1;
    SDL_Gamepad* gamepad  = nullptr;
};

inline bool GamepadState::isConnected() const noexcept
{
    return connected;
}

inline bool GamepadState::isAPressed() const noexcept
{
    return buttons.a;
}

inline bool GamepadState::isBPressed() const noexcept
{
    return buttons.b;
}

inline bool GamepadState::isXPressed() const noexcept
{
    return buttons.x;
}

inline bool GamepadState::isYPressed() const noexcept
{
    return buttons.y;
}

inline bool GamepadState::isLeftStickPressed() const noexcept
{
    return buttons.leftStick;
}

inline bool GamepadState::isRightStickPressed() const noexcept
{
    return buttons.rightStick;
}

inline bool GamepadState::isLeftShoulderPressed() const noexcept
{
    return buttons.leftShoulder;
}

inline bool GamepadState::isRightShoulderPressed() const noexcept
{
    return buttons.rightShoulder;
}

inline bool GamepadState::isBackPressed() const noexcept
{
    return buttons.back;
}

inline bool GamepadState::isViewPressed() const noexcept
{
    return buttons.view;
}

inline bool GamepadState::isStartPressed() const noexcept
{
    return buttons.start;
}

inline bool GamepadState::isMenuPressed() const noexcept
{
    return buttons.menu;
}

inline bool GamepadState::isDPadDownPressed() const noexcept
{
    return dPad.down;
}
inline bool GamepadState::isDPadUpPressed() const noexcept
{
    return dPad.up;
}
inline bool GamepadState::isDPadLeftPressed() const noexcept
{
    return dPad.left;
}
inline bool GamepadState::isDPadRightPressed() const noexcept
{
    return dPad.right;
}

inline bool GamepadState::isLeftThumbStickUp() const noexcept
{
    return thumbSticks.leftY > 0.5f;
}

inline bool GamepadState::isLeftThumbStickDown() const noexcept
{
    return thumbSticks.leftY < -0.5f;
}

inline bool GamepadState::isLeftThumbStickLeft() const noexcept
{
    return thumbSticks.leftX < -0.5f;
}

inline bool GamepadState::isLeftThumbStickRight() const noexcept
{
    return thumbSticks.leftX > 0.5f;
}

inline bool GamepadState::isRightThumbStickUp() const noexcept
{
    return thumbSticks.rightY > 0.5f;
}

inline bool GamepadState::isRightThumbStickDown() const noexcept
{
    return thumbSticks.rightY < -0.5f;
}

inline bool GamepadState::isRightThumbStickLeft() const noexcept
{
    return thumbSticks.rightX < -0.5f;
}

inline bool GamepadState::isRightThumbStickRight() const noexcept
{
    return thumbSticks.rightX > 0.5f;
}

inline bool GamepadState::isLeftTriggerPressed() const noexcept
{
    return triggers.left > 0.5f;
}

inline bool GamepadState::isRightTriggerPressed() const noexcept
{
    return triggers.right > 0.5f;
}

inline bool operator==( const GamepadState::Buttons& lhs, const GamepadState::Buttons& rhs )
{
    return lhs.a == rhs.a &&                          //
           lhs.b == rhs.b &&                          //
           lhs.x == rhs.x &&                          //
           lhs.y == rhs.y &&                          //
           lhs.leftStick == rhs.leftStick &&          //
           lhs.rightStick == rhs.rightStick &&        //
           lhs.leftShoulder == rhs.leftShoulder &&    //
           lhs.rightShoulder == rhs.rightShoulder &&  //
           lhs.back == rhs.back &&                    //
           lhs.start == rhs.start;
}

inline bool operator!=( const GamepadState::Buttons& lhs, const GamepadState::Buttons& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamepadState::DPad& lhs, const GamepadState::DPad& rhs )
{
    return lhs.left == rhs.left &&    //
           lhs.right == rhs.right &&  //
           lhs.up == rhs.up &&        //
           lhs.down == rhs.down;
}

inline bool operator!=( const GamepadState::DPad& lhs, const GamepadState::DPad& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamepadState::ThumbSticks& lhs,
                        const GamepadState::ThumbSticks& rhs )
{
    return lhs.leftX == rhs.leftX &&    //
           lhs.leftY == rhs.leftY &&    //
           lhs.rightX == rhs.rightX &&  //
           lhs.rightY == rhs.rightY;
}

inline bool operator!=( const GamepadState::ThumbSticks& lhs,
                        const GamepadState::ThumbSticks& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamepadState::Triggers& lhs,
                        const GamepadState::Triggers& rhs )
{
    return lhs.left == rhs.left &&  //
           lhs.right == rhs.right;
}

inline bool operator!=( const GamepadState::Triggers& lhs,
                        const GamepadState::Triggers& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamepadState& lhs, const GamepadState& rhs )
{
    return lhs.connected == rhs.connected &&      //
           lhs.buttons == rhs.buttons &&          //
           lhs.dPad == rhs.dPad &&                //
           lhs.thumbSticks == rhs.thumbSticks &&  //
           lhs.triggers == rhs.triggers;
}

inline bool operator!=( const GamepadState& lhs, const GamepadState& rhs )
{
    return !( lhs == rhs );
}
}  // namespace input
}  // namespace sr