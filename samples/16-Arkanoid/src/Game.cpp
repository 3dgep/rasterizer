
#include <Game.hpp>
#include <GameOverState.hpp>
#include <HighScoreState.hpp>
#include <PlayState.hpp>
#include <TitleState.hpp>

#include <Timer.hpp>

#include <input/Input.hpp>

#include <format>

using namespace sr;
using namespace input;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arcadeN { "assets/fonts/ARCADE_N.ttf", 7 }
{
    rasterizer.state.colorTarget = &image;

    // Input that controls adding coins.
    Input::addButtonDownCallback( "Coin", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool back = false;

        for ( auto& gamePadState: gamePadStates )
        {
            back = back || gamePadState.back == ButtonState::Pressed;
        }

        const bool enter = keyboardState.isKeyPressed( Keyboard::Key::Enter );

        return back || enter;
    } );

    // Player 1 start.
    Input::addButtonDownCallback( "Start 1", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        const bool start = gamePadStates[0].start == ButtonState::Pressed;
        const bool _1    = keyboardState.isKeyPressed( Keyboard::Key::D1 );

        return start || _1;
    } );
    // Player 2 start.
    Input::addButtonDownCallback( "Start 2", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        const bool start = gamePadStates[1].start == ButtonState::Pressed;
        const bool _2    = keyboardState.isKeyPressed( Keyboard::Key::D2 );

        return start || _2;
    } );

    // Load high scores.
    highScores.load( "assets/Arkanoid/high_scores.txt" );

    setState( GameState::Playing );
}

void Game::processEvent( const SDL_Event& event )
{
    state->processEvent( event );
}

void Game::update( float deltaTime )
{
    // Check if the coin button was pressed.
    if ( Input::getButtonDown( "Coin" ) )
    {
        coins = std::min( 99, coins + 1 );
    }

    state->update( deltaTime );
    state->draw( rasterizer );

    // Draw the score board.
    {
        auto r        = rasterizer;
        r.state.color = Color::Red;
        // Player 1
        r.drawText( arcadeN, "1UP", 26, 7 );
        // Draw P1 score right-aligned.
        r.state.color = Color::White;
        r.drawText( arcadeN, std::format( "{:6d}", score1 ), 15, 15 );

        // High score
        r.state.color = Color::Red;
        int highScore = getHighScore();
        r.drawText( arcadeN, "HIGH SCORE", 73, 7 );
        r.state.color = Color::White;
        r.drawText( arcadeN, std::format( "{:6d}", highScore ), 87, 15 );
        if ( numPlayers > 1 )
        {
            r.state.color = Color::Red;
            // Player 2
            r.drawText( arcadeN, "2UP", 177, 7 );
            // Draw P2 score right-aligned.
            r.drawText( arcadeN, std::format( "{:6d}", score2 ), 164, 15 );
        }
    }

#if _DEBUG
    drawFPS();
#endif

    // If one of the states requested a state change, then
    // switch to the next state at the end of the update function.
    // Switching states while a state is executing will crash
    // since the states are objects and deleting an object while it
    // is still running is bad.
    setState( nextState );
}

Image& Game::getImage() noexcept
{
    return image;
}

void Game::setNextState( GameState _nextState )
{
    nextState = _nextState;
}

void Game::setState( GameState newState )
{
    if ( currentState != newState )
    {
        endState( currentState );
        startState( newState );

        currentState = newState;
        // Also make sure the next state is correct...
        nextState = currentState;

        switch ( newState )
        {
        case GameState::MainMenu:
            state = std::make_unique<TitleState>( *this );
            break;
        case GameState::Playing:
            state = std::make_unique<PlayState>( *this );
            break;
        case GameState::HighScore:
            state = std::make_unique<HighScoreState>( *this, std::max( score1, score2 ), levelId + 1 );
            break;
        case GameState::GameOver:
            state = std::make_unique<GameOverState>( *this );
            break;
        }
    }
}

void Game::addPoints( int points )
{
    switch ( currentPlayer )
    {
    case 0:
        score1 += points;
        break;
    case 1:
        score2 += points;
        break;
    default:
        break;
    }
}

void Game::addHighScore( const HighScore& score )
{
    highScores.addScore( score );
    highScores.save( "assets/Arkanoid/high_scores.txt" );
}

int Game::getHighScore() const noexcept
{
    return std::max( highScores.getHighScore(), std::max( score1, score2 ) );
}

const Font& Game::getFont() const noexcept
{
    return arcadeN;
}

void Game::endState( GameState _state )
{
    switch ( _state )
    {
    case GameState::Playing:
    {
        const PlayState* playState = dynamic_cast<PlayState*>( state.get() );
        levelId                    = playState->getCurrentLevel();
    }
    break;
    }
}

void Game::startState( GameState _state ) {}

void Game::drawFPS() const
{
    static Timer       timer;
    static double      totalTime = 0.0;
    static uint64_t    frames    = 0;
    static std::string fps       = "FPS: 0";

    timer.tick();
    ++frames;
    totalTime += timer.elapsedSeconds();
    if ( totalTime > 1.0 )
    {
        fps    = std::format( "FPS: {:.3f}", static_cast<double>( frames ) / totalTime );
        frames = 0;
        timer.reset();
    }

    auto r        = rasterizer;
    r.state.color = Color::Black;
    r.drawText( arcadeN, fps, 6, 6 );
    r.state.color = Color::White;
    r.drawText( arcadeN, fps, 4, 4 );
}
