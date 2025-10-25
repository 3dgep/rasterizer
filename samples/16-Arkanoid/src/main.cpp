#include "Game.hpp"
#include <Timer.hpp>

#include <Graphics/Window.hpp>
#include <input/Input.hpp>

#include <format>
#include <iostream>

using namespace sr;
using namespace input;

int main( int argc, char* argv[] )
{
    // Parse command-line arguments.
    if ( argc > 1 )
    {
        for ( int i = 0; i < argc; ++i )
        {
            if ( strcmp( argv[i], "-cwd" ) == 0 )
            {
                std::string workingDirectory = argv[++i];
                std::filesystem::current_path( workingDirectory );
            }
        }
    }

    constexpr int WINDOW_WIDTH  = 224;
    constexpr int WINDOW_HEIGHT = 256;

    Game game { WINDOW_WIDTH, WINDOW_HEIGHT };

    Window window { "09 - Breakout", WINDOW_WIDTH, WINDOW_HEIGHT, true };

    Timer timer;

    while ( window )
    {
        SDL_Event e;
        while ( window.pollEvent( e ) )
        {
            // Allow the game to process events.
            game.processEvent( e );

            switch ( e.type )
            {
            case SDL_EVENT_KEY_DOWN:
                switch ( e.key.key )
                {
                case SDLK_V:
                    window.setVSync( !window.isVSync() );
                    std::cout << "Vsync: " << window.isVSync() << std::endl;
                    break;
                case SDLK_RETURN:
                    if ( ( e.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                }
                break;
            }
        }

        timer.tick();
        auto elapsedTime = static_cast<float>( timer.elapsedSeconds() );

        // Maximum tick time for physics.
        constexpr float physicsTick = 1.0f / 60.0f;

        do
        {
            Input::update();
            game.update( std::min( elapsedTime, physicsTick ) );
            elapsedTime -= physicsTick;
        } while ( elapsedTime > 0.0f );

        game.draw();

        window.clear( Color::Black );
        window.present( game.getImage() );

        //timer.limitFPS( 15 );
    }
}