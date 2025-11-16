#include "Game.hpp"

#include <graphics/Window.hpp>

using namespace sr;

constexpr int SCREEN_WIDTH  = 480;
constexpr int SCREEN_HEIGHT = 256;

int main()
{
    Game   game { SCREEN_WIDTH, SCREEN_HEIGHT };
    Window window { "Pixel Adventure", SCREEN_WIDTH, SCREEN_HEIGHT, true };

    while ( window )
    {
        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
            // Let the game process the event (for the on-screen buttons)
            game.processEvent( event );

            switch ( event.type )
            {
            case SDL_EVENT_KEY_DOWN:
                switch ( event.key.key )
                {
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                case SDLK_V:
                    window.toggleVSync();
                    std::cout << "VSync " << (window.isVSync() ? "enabled." : "disabled.") << std::endl;
                    break;
                case SDLK_RETURN:
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            }
        }

        // Update the game.
        game.update();

        // Clear window background and present.
        window.clear( Color { 0x301f21u } );
        window.present( game.getImage() );
    }

    return 0;
}