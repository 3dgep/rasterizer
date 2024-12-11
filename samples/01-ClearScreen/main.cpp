#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Window.hpp>

#include <iostream>

using namespace sr;

int main()
{
    Window window( "Clear Screen", 1280, 720 );
    Image  image { 800, 600 };
    Timer  timer;

    while ( window )
    {
        timer.tick();

        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
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

        window.clear( Color::Black );

        image.clear( Color::fromHSV( static_cast<float>( timer.totalSeconds() * 60.0 ) ) );

        window.present( image );
    }
}