#include <graphics/Image.hpp>
#include <graphics/Window.hpp>

using namespace sr;

int main()
{
    bool isPlaying = true;

    Window window( "Clear Screen", 1280, 720 );
    Image  image { 800, 600 };

    while ( window )
    {
        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
            switch ( event.type )
            {
            case SDL_EVENT_KEY_DOWN:
                switch ( event.key.scancode )
                {
                case SDL_SCANCODE_ESCAPE:
                    window.destroy();
                    break;
                }
                break;
            }
        }

        window.clear( Color::Black );

        image.clear( Color::Red );

        window.present( image );
    }
}