#pragma once

#include "HighScores.hpp"
#include "State.hpp"

#include <graphics/Font.hpp>

#include <SDL3/SDL_events.h>

#include <memory>

class Game
{
public:
    enum class GameState
    {
        None,
        MainMenu,
        Intro,
        Playing,
        HighScore,
        Pause,
        GameOver,
    };

    Game( uint32_t screenWidth, uint32_t screenHeight );

    // Delete copy and move constructors and assign operators
    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void update( float deltaTime );

    void draw();

    void processEvent( const SDL_Event& event );

    sr::graphics::Image& getImage() noexcept;

    /// <summary>
    /// Set the state to transition.
    /// </summary>
    /// <param name="nextState">The next state to transition to.</param>
    void setNextState( GameState nextState );

    void setState( GameState newState );

    GameState getState() const noexcept
    {
        return currentState;
    }

    int getCoins() const noexcept
    {
        return coins;
    }

    void setNumPlayers( int _numPlayers ) noexcept
    {
        numPlayers = _numPlayers;
    }

    int getNumPlayers() const noexcept
    {
        return numPlayers;
    }

    /// <summary>
    /// Add the given points to the current player.
    /// </summary>
    /// <param name="points">The number of points to add to the current player.</param>
    void addPoints( int points );

    /// <summary>
    /// Add a given high score to the high score list.
    /// </summary>
    /// <param name="score">The high score to add.</param>
    void addHighScore( const HighScore& score );

    /// <summary>
    /// Get the current high score.
    /// </summary>
    /// <returns>The current highscore.</returns>
    int getHighScore() const noexcept;

    /// <summary>
    /// Get the one font used by the game.
    /// </summary>
    /// <returns></returns>
    const sr::graphics::Font& getFont() const noexcept;

private:
    void endState( GameState state );
    void startState( GameState state );
    void drawFPS() const;

    GameState currentState = GameState::None;
    GameState nextState    = GameState::None;

    std::unique_ptr<State> state;

    sr::graphics::Image image;
    sr::graphics::Rasterizer rasterizer;
    // Fonts.
    sr::graphics::Font arcadeN;

    int numPlayers = 1;
    int coins      = 0;
    // Current level.
    int levelId    = 0;
    // Player 1 score.
    int score1 = 0;
    // Player 2 score.
    int score2 = 0;

    // Current player.
    int currentPlayer = 0;

    HighScores highScores;
};