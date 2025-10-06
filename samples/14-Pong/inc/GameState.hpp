#pragma once

#include "StateBase.hpp"

class GameState : public StateBase
{
public:
    enum class AIDifficulty
    {
        None,    // Two-player mode
        Easy,    // AI reacts slowly and makes mistakes
        Medium,  // AI reacts moderately
        Hard     // AI reacts quickly and accurately
    };

    GameState() = default;

    GameState( int screenWidth, int screenHeight );

    /// <summary>
    /// Set the AI difficulty for player 2.
    /// </summary>
    /// <param name="difficulty">The AI difficulty level.</param>
    void setAIDifficulty( AIDifficulty difficulty );

    /// <summary>
    /// Get the current AI difficulty.
    /// </summary>
    /// <returns>The current AI difficulty level.</returns>
    AIDifficulty getAIDifficulty() const noexcept;

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
    void updateAIPaddle( Paddle& paddle, float deltaTime );

    void checkWallCollisions();

    void drawServe( sr::graphics::Rasterizer& rasterizer );
    void drawPlay( sr::graphics::Rasterizer& rasterizer );
    void drawGameOver( sr::graphics::Rasterizer& rasterizer );

    // Internal game state.
    State m_State = State::None;
    float m_TotalTime = 0.0f;

    // AI settings
    AIDifficulty m_AIDifficulty = AIDifficulty::None;
    float        m_AIReactionDelay = 0.0f;  // Time before AI reacts
    float        m_AIErrorMargin = 0.0f;    // How far off the AI aims

    sr::Text m_GameOverText;
};