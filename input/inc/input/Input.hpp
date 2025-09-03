#pragma once

#include "GamePadStateTracker.hpp"
#include "KeyboardStateTracker.hpp"
#include "MouseStateTracker.hpp"

#include <functional>
#include <span>
#include <string_view>

namespace sr
{
inline namespace input
{
/// <summary>
/// An AxisCallback function takes a GamePadStateTracker (for each connected GamePad), KeyboardStateTracker, and a MouseStateTracker and returns
/// the value of the axis being queried (in the range [-1...1]).
/// </summary>
using AxisCallback = std::function<float( std::span<const GamepadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& )>;

/// <summary>
/// A ButtonCallback function takes a GamePadStateTracker (for each connected GamePad), a KeyboardStateTracker, and a MouseStateTracker and returns
/// the state of the button.
/// </summary>
using ButtonCallback = std::function<bool( std::span<const GamepadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& )>;

namespace Input
{
/// <summary>
/// Update the input state. Should only be called once per frame.
/// </summary>
void update();

/// <summary>
/// Returns the value of the axis identified by axisName.
/// </summary>
/// <param name="axisName">The name of the axis to read.</param>
/// <returns>A value in the range [-1...1] that represents the value of the axis.</returns>
float getAxis( std::string_view axisName );

/// <summary>
/// Returns the value of the button identified by buttonName.
/// </summary>
/// <param name="buttonName">The name of the button to query.</param>
/// <returns>`true` if the button is pressed, `false` otherwise.</returns>
bool getButton( std::string_view buttonName );

/// <summary>
/// Returns `true` in the frame that the button is pressed.
/// </summary>
/// <param name="buttonName">The name of the button to query.</param>
/// <returns>`true` if the button was pressed this frame, `false` otherwise.</returns>
bool getButtonDown( std::string_view buttonName );

/// <summary>
/// Returns `true` in the frame that the button is released.
/// </summary>
/// <param name="buttonName">The name of the button to query.</param>
/// <returns>`true` if the button was released this frame, `false` otherwise.</returns>
bool getButtonUp( std::string_view buttonName );

/// <summary>
/// Returns `true` while the user is holding down a specific key on the keyboard.
/// </summary>
/// <param name="keyName">The name of the key to query.</param>
/// <returns>`true` if the key is being held, `false` otherwise.</returns>
bool getKey( std::string_view keyName );

/// <summary>
/// Returns `true` in the frame that the key is pressed.
/// </summary>
/// <param name="keyName">The name of the key to query.</param>
/// <returns>`true` if the key is pressed this frame, `false` otherwise.</returns>
bool getKeyDown( std::string_view keyName );

/// <summary>
/// Return `true` in the frame that the key is released.
/// </summary>
/// <param name="keyName">The name of the key to query.</param>
/// <returns>`true` if the key is released this frame.</returns>
bool getKeyUp( std::string_view keyName );

/// <summary>
/// Returns `true` while the user is holding down a specific key on the keyboard.
/// </summary>
/// <param name="key">The SDL_Scancode enum to query.</param>
/// <returns>`true` if the key is being held, `false` otherwise.</returns>
bool getKey( SDL_Scancode key );

/// <summary>
/// Returns `true` in the frame that the key is pressed.
/// </summary>
/// <param name="key">The SDL_Scancode enum to query.</param>
/// <returns>`true` if the key is pressed this frame, `false` otherwise.</returns>
bool getKeyDown( SDL_Scancode key );

/// <summary>
/// Returns `true` in the frame that the key is released.
/// </summary>
/// <param name="key">The SDL_Scancode enum to query.</param>
/// <returns></returns>
bool getKeyUp( SDL_Scancode key );

/// <summary>
/// Returns true when the given mouse button is held down.
/// </summary>
/// <param name="button">The MouseButton enum to check.</param>
/// <returns>`true` if the mouse button is pressed, `false` otherwise.</returns>
bool getMouseButton( MouseButton button );

/// <summary>
/// Returns true when the given mouse button is pressed this frame.
/// </summary>
/// <param name="button">The MouseButton enum to check.</param>
/// <returns>`true` if the mouse button is pressed this frame, `false` otherwise.</returns>
bool getMouseButtonDown( MouseButton button );

/// <summary>
/// Returns true when the give mouse button is released this frame.
/// </summary>
/// <param name="button">The MouseButton enum to check.</param>
/// <returns>`true` if the mouse button is released this frame.</returns>
bool getMouseButtonUp( MouseButton button );

/// <summary>
/// Map an axis name to an axis callback function.
/// </summary>
/// <param name="axisName">The name of the axis to map.</param>
/// <param name="callback">The callback function to use to return the value of the axis.</param>
void addAxisCallback( std::string_view axisName, AxisCallback callback );

/// <summary>
/// Map a button name to a button callback function.
/// </summary>
/// <param name="buttonName">The name of the button or action to map to the callback function.</param>
/// <param name="callback">The callback function used to return the value of the button.</param>
void addButtonCallback( std::string_view buttonName, ButtonCallback callback );

}  // namespace Input
}  // namespace input
}  // namespace sr