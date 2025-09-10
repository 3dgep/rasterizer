#pragma once

#include <graphics/Rasterizer.hpp>

/// <summary>
/// Represents a base class for a state in a state-driven system, providing lifecycle methods for state management, updating, and rendering.
/// </summary>
class State
{
public:
    State() = default;
    virtual ~State() = default;

    /// <summary>
    /// Begins or initializes a new state. Intended to be overridden in derived classes.
    /// </summary>
    virtual void beginState() = 0;

    /// <summary>
    /// Updates the state based on the elapsed time and returns the resulting state.
    /// </summary>
    /// <param name="deltaTime">The time elapsed since the last update, in seconds.</param>
    /// <returns>A pointer to the next state, or null if the current state should continue.</returns>
    virtual State* update( float deltaTime ) = 0;

    /// <summary>
    /// Draws the object using the specified rasterizer.
    /// </summary>
    /// <param name="rasterizer">A reference to the Rasterizer used for rendering.</param>
    virtual void draw( sr::graphics::Rasterizer& rasterizer ) = 0;

};