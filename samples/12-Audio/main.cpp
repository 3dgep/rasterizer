#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <Audio/Sound.hpp>

using namespace sr;

int main()
{
    Window     window( "Clear Screen", 800, 600 );
    Image      image { 800, 600 };
    Rasterizer rasterizer;
    Timer      timer;
    Text fpsText { Font::DefaultFont, "FPS: 0" };
    Sprite     ball { "assets/textures/ball.png", BlendMode::AlphaDiscard };
    Transform2D transform { { image.getWidth() / 2, image.getHeight() / 2 } };
    transform.setAnchor( { ball.getWidth() / 2, ball.getHeight() / 2 } );

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

        image.clear( Color::Black );

        rasterizer.drawSprite( ball, transform );

        if ( timer.totalSeconds() > 1.0 )
        {
            fpsText = std::format( "FPS: {:.0f}", timer.FPS() );
            timer.reset();
        }

        rasterizer.drawText( fpsText, 10, 10 );

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}