#include <iostream>
#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

using namespace sr;

int main()
{
    Window     window( "Clear Screen", 1280, 720 );
    Image      image { 320, 180 };
    Rasterizer rasterizer;
    Timer      timer;

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

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

        image.clear( Color::CornFlowerBlue );
        for ( int i = 0; i < image.getWidth(); ++i )
        {
            float hue              = static_cast<float>( i ) / static_cast<float>( image.getWidth() ) * 360.0f;
            rasterizer.state.color = Color::fromHSV( hue );
            rasterizer.drawLine( i, 0, i, static_cast<int>( image.getHeight() ) );
        }

        window.present( image );
    }

    return 0;
}