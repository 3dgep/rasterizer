#include <Game.hpp>

#include <graphics/Text.hpp>

#include <input/Input.hpp>

#include <format>
#include <iostream>

using namespace sr::graphics;
using namespace input;
using Keyboard::Key;

Game::Game( int screenWidth, int screenHeight )
: m_Window( "Pixel Adventure", screenWidth, screenHeight, true )
, m_Image( screenWidth, screenHeight )
, m_FPSText( Font::DefaultFont )
{

    // Input that controls the characters horizontal movement.
    Input::addAxisCallback( "Horizontal", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a     = keyState.A ? 1.0f : 0.0f;
        const float d     = keyState.D ? 1.0f : 0.0f;
        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d - left + right, -1.0f, 1.0f );
    } );

    // Input that controls jumping.
    Input::addButtonDownCallback( "Jump", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        const bool space = keyboardState.isKeyPressed( Key::Space );
        const bool up    = keyboardState.isKeyPressed( Key::Up );
        const bool w     = keyboardState.isKeyPressed( Key::W );

        return a || space || up || w;
    } );

    // Input to go to the next map.
    Input::addButtonDownCallback( "Next", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool start = false;

        for ( auto& gamePadState: gamePadStates )
        {
            start = start || gamePadState.start == ButtonState::Pressed;
        }

        return start;
    } );

    // Input to go to the previous map.
    Input::addButtonDownCallback( "Previous", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool back = false;

        for ( auto& gamePadState: gamePadStates )
        {
            back = back || gamePadState.back == ButtonState::Pressed;
        }

        return back;
    } );

    // Input to go to reload the current map.
    Input::addButtonDownCallback( "Reload", []( std::span<const GamepadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool b = false;

        for ( auto& gamePadState: gamePadStates )
        {
            b = b || gamePadState.b == ButtonState::Pressed;
        }

        const bool enter = keyboardState.isKeyPressed( Key::Enter );

        return b || enter;
    } );

    m_Rasterizer.state.colorTarget = &m_Image;

    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Blue.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Brown.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Gray.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Green.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Pink.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Purple.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    m_Backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Yellow.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );

    m_CurrentBackground = m_Backgrounds.end() - 1;

    loadLevel( 0 );
}

void Game::loadLevel( size_t levelId )
{
    m_CurrentLevelId = levelId % 4; // levels.size();

    if ( ++m_CurrentBackground == m_Backgrounds.end() )
    {
        m_CurrentBackground = m_Backgrounds.begin();
    }
}

bool Game::update()
{
    SDL_Event event;
    while ( m_Window.pollEvent( event ) )
    {
        switch ( event.type )
        {
        case SDL_EVENT_KEY_DOWN:
            switch ( event.key.key )
            {
            case SDLK_ESCAPE:
                m_Window.destroy();
                return false;
            case SDLK_V:
                m_Window.toggleVSync();
                break;
            case SDLK_RETURN:
                if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                {
                case SDLK_F11:
                    m_Window.toggleFullscreen();
                }
                break;
            }
            break;
        }
    }

    m_Timer.tick();

    m_CurrentBackground->update( m_Timer );
    m_CurrentBackground->draw( m_Rasterizer );

    auto elapsedTime = static_cast<float>( m_Timer.elapsedSeconds() );
    do
    {
        // Update the input state.
        Input::update();

        // Check if next/previous input buttons have been pressed.
        if ( Input::getButtonDown( "Next" ) )
        {
            onNext();
        }
        if ( Input::getButtonDown( "Previous" ) )
        {
            onPrevious();
        }
        if ( Input::getButtonDown( "Reload" ) )
        {
            onRestart();
        }

        // m_CurrentLevel.update( std::min( elapsedTime, m_PhysicsTick ) );
        elapsedTime -= m_PhysicsTick;
    } while ( elapsedTime > 0.0f );

    // Draw an FPS counter on the screen.
    if ( m_Timer.totalSeconds() > 1.0 )
    {
        m_FPSText = std::format( "FPS: {:.0f}", m_Timer.FPS() );
        m_Timer.reset();
    }

    m_Rasterizer.drawText( m_FPSText, 10, 10 );

    m_Window.clear( Color { 0x301F21 } );
    m_Window.present( m_Image );

    return true;
}

void Game::onPrevious()
{
    std::cout << "Previous level!" << std::endl;

    loadLevel( --m_CurrentLevelId );
}

void Game::onNext()
{
    std::cout << "Next level!" << std::endl;

    loadLevel( m_CurrentLevelId++ );
}

void Game::onRestart()
{
    std::cout << "Restart level!" << std::endl;

    loadLevel( m_CurrentLevelId );
}
