#pragma once

namespace sr
{
inline namespace input
{
enum class ButtonState
{
    Up       = 0,  ///< The button is not being held.
    Held     = 1,  ///< The button is being held.
    Released = 2,  ///< The button was released this frame.
    Pressed  = 3   ///< The button was pressed this frame.
};
}
}  // namespace sr
