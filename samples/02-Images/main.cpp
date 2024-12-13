#include <graphics/Image.hpp>
#include <graphics/Window.hpp>

using namespace sr;

int main()
{
    bool isPlaying = true;

    Window window( "Images", 1280, 720 );
    Image  image { "assets/textures/Mona_Lisa.jpg" };

    while ( window )
    {
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
                    if ( (event.key.mod & SDL_KMOD_ALT) != 0 )
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
        window.present( image );
    }
}