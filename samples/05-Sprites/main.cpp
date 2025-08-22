#include "glm/trigonometric.hpp"

#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Sprite.hpp>
#include <graphics/Window.hpp>

using namespace sr;

int main()
{
    Window window( "Sprites", 1280, 720 );
    Image  image { 1280, 720 };
    Sprite sprite {
        std::make_shared<Image>( "assets/textures/Smiley.png" ), BlendMode::AlphaBlend
    };
    Transform2D transform;
    transform.setScale( 0.25f );
    transform.setAnchor( { sprite.getWidth() * 0.5f, sprite.getHeight() * 0.5f } ); // Center the anchor point on the sprite.
    transform.setPosition( { image.getWidth() * 0.5f, image.getHeight() * 0.5f } ); // Center the sprite on the screen.

    Rasterizer rasterizer;
    Timer      timer;

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;
    rasterizer.state.blendMode   = BlendMode::AlphaBlend;

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

        transform.setRotation( static_cast<float>( timer.totalSeconds() ) );
        rasterizer.drawSprite( sprite, transform );

        window.clear( Color::White );
        window.present( image );
    }

    return 0;
}