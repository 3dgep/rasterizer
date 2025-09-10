#pragma once

#include "StateBase.hpp"

class GameState : public StateBase
{
public:
    GameState() = default;

    GameState( int screenWidth, int screenHeight );

    /// <summary>
    /// Starts the game state.
    /// </summary>
    void beginState() override;

    /// <summary>
    /// Updates the state based on the elapsed time and returns the resulting state.
    /// </summary>
    /// <param name="deltaTime">The time elapsed since the last update, in seconds.</param>
    /// <returns>A pointer to the next state, or null if the current state should continue.</returns>
    ::State* update( float deltaTime ) override;

    /// <summary>
    /// Draws the object using the specified rasterizer.
    /// </summary>
    /// <param name="rasterizer">A reference to the Rasterizer used for rendering.</param>
    void draw( sr::graphics::Rasterizer& rasterizer ) override;

private:
    enum class State
    {
        None,
        P1Serve,
        P2Serve,
        Play,
        GameOver,
    };

    void setState( State newState );
    void startState( State state );

    void startServe( State state );

    void updateServe( float deltaTime );
    void updatePlay( float deltaTime );
    void updateGameOver( float deltaTime );
    void updatePaddle( Paddle& paddle, std::string_view input, float deltaTime );

    void checkWallCollisions();

    void drawServe( sr::graphics::Rasterizer& rasterizer );
    void drawPlay( sr::graphics::Rasterizer& rasterizer );
    void drawGameOver( sr::graphics::Rasterizer& rasterizer );

    // Internal game state.
    State m_State = State::None;
    float m_TotalTime = 0.0f;

    sr::Text m_GameOverText;
};