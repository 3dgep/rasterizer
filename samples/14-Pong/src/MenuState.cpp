#include "GameState.hpp"

#include <MenuState.hpp>
#include <Scores.hpp>
#include <Sound.hpp>

#include <input/Input.hpp>

#include <format>

using namespace input;
using namespace sr::graphics;

MenuState::MenuState( int screenWidth, int screenHeight )
: StateBase( screenWidth, screenHeight )
{
    // Setup input callbacks for Menu Left/Right movement
    Input::addButtonUpCallback( "Left", []( std::span<const GamepadStateTracker> gamepadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool left = false;
        for ( auto& gamepadState: gamepadStates )
        {
            left = left || gamepadState.dPadLeft == ButtonState::Released || gamepadState.leftStickLeft == ButtonState::Released;
        }
        const bool a = keyboardState.isKeyReleased( Keyboard::Key::A );
        left         = left || keyboardState.isKeyReleased( Keyboard::Key::Left );

        return left || a;
    } );
    Input::addButtonUpCallback( "Right", []( std::span<const GamepadStateTracker> gamepadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool right = false;
        for ( auto& gamepadState: gamepadStates )
        {
            right = right || gamepadState.dPadRight == ButtonState::Released || gamepadState.leftStickRight == ButtonState::Released;
        }
        const bool d = keyboardState.isKeyReleased( Keyboard::Key::D );
        right = right || keyboardState.isKeyReleased( Keyboard::Key::Right );

        return right || d;
    } );

    m_Ball.setPosition( { m_ScreenWidth / 2, m_ScreenHeight / 2 } );

    glm::vec2 v = normalize( glm::vec2 { 1, -2 } ) * 260.0f;
    m_Ball.setVelocity( v );
}

void MenuState::beginState()
{
    m_P1Score = std::format( "{}", Scores::getP1Score() );
    m_P2Score = std::format( "{}", Scores::getP2Score() );
}

State* MenuState::update( float deltaTime )
{
    m_Ball.update( deltaTime );

    // Check for collision with the sides of the screen.
    auto p    = m_Ball.getPosition();
    auto v    = m_Ball.getVelocity();
    auto aabb = m_Ball.getAABB();

    if ( aabb.left() <= 0 )
    {
        p.x -= aabb.left();
        v.x *= -1.0f;
        Sound::WallSound.play();
    }
    else if ( aabb.right() >= m_ScreenWidth )
    {
        p.x += m_ScreenWidth - aabb.right() - 1;
        v.x *= -1.0f;
        Sound::WallSound.play();
    }

    if ( aabb.top() <= 0 )
    {
        p.y -= aabb.top();
        v.y *= -1.0f;
        Sound::WallSound.play();
    }
    else if ( aabb.bottom() >= m_ScreenHeight )
    {
        p.y += m_ScreenHeight - aabb.bottom() - 1;
        v.y *= -1.0f;
        Sound::WallSound.play();
    }

    m_Ball.setPosition( p );
    m_Ball.setVelocity( v );

    // Cycle through AI difficulties with Left/Right arrow keys or A/D keys
    if ( Input::getButtonUp( "Left" ) )
    {
        int difficulty         = static_cast<int>( m_SelectedAIDifficulty );
        difficulty             = ( difficulty + 3 ) % 4;  // Cycle through 0-3 (None, Easy, Medium, Hard)
        m_SelectedAIDifficulty = static_cast<GameState::AIDifficulty>( difficulty );
    }
    else if ( Input::getButtonUp( "Right" ) )
    {
        int difficulty         = static_cast<int>( m_SelectedAIDifficulty );
        difficulty             = ( difficulty + 1 ) % 4;  // Cycle through 0-3 (None, Easy, Medium, Hard)
        m_SelectedAIDifficulty = static_cast<GameState::AIDifficulty>( difficulty );
    }

    // Switch to the game state with the "Submit" button is pressed.
    // This is mapped to the "Enter" key, the "Space" key, and the "A" button on any gamepad.
    if ( Input::getButtonUp( "Submit" ) )
    {
        GameState* gameState = new GameState( m_ScreenWidth, m_ScreenHeight );
        gameState->setAIDifficulty( m_SelectedAIDifficulty );
        return gameState;
    }

    return nullptr;
}

void MenuState::draw( sr::Rasterizer& rasterizer )
{
    StateBase::draw( rasterizer );

    m_Ball.draw( rasterizer );

    // Draw AI difficulty selection
    const char* difficultyText = "";
    switch ( m_SelectedAIDifficulty )
    {
    case GameState::AIDifficulty::None:
        difficultyText = "2 PLAYERS";
        break;
    case GameState::AIDifficulty::Easy:
        difficultyText = "AI: EASY";
        break;
    case GameState::AIDifficulty::Medium:
        difficultyText = "AI: MEDIUM";
        break;
    case GameState::AIDifficulty::Hard:
        difficultyText = "AI: HARD";
        break;
    }

    sr::Text difficultyDisplay { sr::Font::DefaultFont, difficultyText };
    int      textX = ( m_ScreenWidth - difficultyDisplay.getWidth() ) / 2;
    int      textY = m_ScreenHeight - 40;

    rasterizer.state.color = Color::White;
    rasterizer.drawText( difficultyDisplay, textX, textY );

    // Draw instructions
    static sr::Text instructions { sr::Font::DefaultFont, "LEFT/RIGHT: Change Mode  ENTER: Start" };
    int             instrX = ( m_ScreenWidth - instructions.getWidth() ) / 2;
    int             instrY = m_ScreenHeight - 20;

    rasterizer.drawText( instructions, instrX, instrY );
}