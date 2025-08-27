#include <Timer.hpp>
#include <iostream>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

using namespace sr;

int main()
{
    Window     window( "Circles", 800, 600 );
    Image      image { 800, 600 };
    Rasterizer rasterizer;
    Timer      timer;
    glm::ivec2 mousePos { 0 };
    int        radius = 10;

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
            case SDL_EVENT_MOUSE_MOTION:
                mousePos = window.clientToImage( event.motion.x, event.motion.y, image );
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                radius += event.wheel.y;
                break;
            }
        }

        image.clear( Color::White );

         rasterizer.state.color    = Color::Red;
         rasterizer.state.fillMode = FillMode::Solid;
         rasterizer.drawCircle( mousePos.x, mousePos.y, radius );

        rasterizer.state.color    = Color::Blue;
        rasterizer.state.fillMode = FillMode::WireFrame;
        rasterizer.drawCircle( mousePos.x, mousePos.y, radius );

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}