#include <HighScoreState.hpp>

#include <input/Input.hpp>

#include <format>
#include <numbers>

using namespace sr;
using namespace input;

constexpr float PI = std::numbers::pi_v<float>;

static const std::string chars    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.! ";  // I don't know what the possible characters are...
static const std::string place[] = {
    "1ST",
    "2ND",
    "3RD",
    "4TH",
    "5TH"
};

HighScoreState::HighScoreState( Game& _game, int score, int round )
: game { _game }
, highScore { score, round, "   " }
{
    using Keyboard::Key;

    // Map controls for entering the initials for a high score.
    Input::addButtonDownCallback( "Up", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool up = false;

        for ( auto& gamePadState: gamePadStates )
        {
            up = up || gamePadState.dPadUp == ButtonState::Pressed;
        }

        up           = up || keyboardState.isKeyPressed( Key::Up );
        const bool w = keyboardState.isKeyPressed( Key::W );

        return up || w;
    } );

    Input::addButtonDownCallback( "Down", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool down = false;

        for ( auto& gamePadState: gamePadStates )
        {
            down = down || gamePadState.dPadDown == ButtonState::Pressed;
        }

        down         = down || keyboardState.isKeyPressed( Key::Down );
        const bool s = keyboardState.isKeyPressed( Key::S );

        return down || s;
    } );

    Input::addButtonDownCallback( "Left", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool left = false;

        for ( auto& gamePadState: gamePadStates )
        {
            left = left || gamePadState.dPadLeft == ButtonState::Pressed;
        }

        left         = left || keyboardState.isKeyPressed( Key::Left );
        const bool a = keyboardState.isKeyPressed( Key::A );

        return left || a;
    } );

    Input::addButtonDownCallback( "Right", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool right = false;

        for ( auto& gamePadState: gamePadStates )
        {
            right = right || gamePadState.dPadRight == ButtonState::Pressed;
        }

        right        = right || keyboardState.isKeyPressed( Key::Right );
        const bool d = keyboardState.isKeyPressed( Key::D );

        return right || d;
    } );

    Input::addButtonDownCallback( "Back", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool b = false;

        for ( auto& gamePadState: gamePadStates )
        {
            b = b || gamePadState.b == ButtonState::Pressed;
        }

        const bool backspace = keyboardState.isKeyPressed( Key::Back );

        return b || backspace;
    } );

    Input::addButtonDownCallback( "Enter", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        const bool enter = keyboardState.isKeyPressed( Key::Enter );

        return a || enter;
    } );
}

void HighScoreState::update( float deltaTime )
{
    timer += deltaTime;

    // The number of characters in the character array.
    const int numChars = static_cast<int>( std::size( chars ) );

    if ( Input::getButtonDown( "Up" ) )
    {
        character[initial] = ( character[initial] + 1 ) % numChars;
    }
    if ( Input::getButtonDown( "Down" ) )
    {
        // This is a trick to make sure it doesn't become negative, but still wraps around to the previous character.
        character[initial] = ( character[initial] - 1 + numChars ) % numChars;
    }
    if ( Input::getButtonDown( "Left" ) || Input::getButtonDown( "Back" ) )
    {
        initial = std::max( 0, initial - 1 );
    }
    if ( Input::getButtonDown( "Right" )  )
    {
        initial = std::min( 2, initial + 1 );
    }
    if ( Input::getButtonDown( "Enter" ) )
    {
        if ( initial == 2 )
        {
            game.addHighScore( highScore );
            game.setNextState( Game::GameState::GameOver );
        }
        else
        {
            ++initial;
        }
    }

    highScore.name[initial] = chars[character[initial]];
}

void HighScoreState::draw( Rasterizer& rasterizer )
{
    const auto& font = game.getFont();

    auto r = rasterizer;
    r.clear( Color::Black );

    r.state.color = Color::Red;
    r.drawText( font, "ENTER YOUR INITIALS !", 31, 79 );
    r.state.color = Color::Yellow;
    r.drawText( font, "SCORE ROUND   NAME", 40, 104 );
    r.state.color = Color::White;
    r.drawText( font, std::format( "{:8d}", highScore.score ), 20, 120 );
    r.state.color = Color::Yellow;
    r.drawText( font, std::format( "{:>3}", highScore.round ), 95, 120 );

    for ( int i = 0; i < 3; ++i )
    {

        r.state.color = Color::Yellow;
        if (i == initial)
        {
            r.state.color = std::sin( timer * PI * 12.0f ) > 0.0f ? Color::Red : Color::White;
        }
        r.drawText( font, std::format( "{}", highScore.name[i] ), 144 + i * 7, 120 );
    }
}

void HighScoreState::processEvent( const SDL_Event& event )
{
}
