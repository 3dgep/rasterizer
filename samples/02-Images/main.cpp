#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

using namespace sr;

int main()
{
    bool isPlaying = true;

    Window     window( "Images", 1280, 720 );
    Image      colorTarget { 1280, 720 };
    Rasterizer rasterizer;
    rasterizer.state.colorTarget = &colorTarget;

    Image monaLisa { "assets/textures/Mona_Lisa.jpg" };
    glm::vec2 mousePos { 0 };
    float scale = 1.0f;

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
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            case SDL_EVENT_MOUSE_MOTION:
                mousePos = window.clientToImage( event.motion.x, event.motion.y, colorTarget );
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                scale += event.wheel.y / 100.0f;
                break;
            }
        }

        rasterizer.clear( Color::White );

        float           dW = monaLisa.getWidth() * scale;
        float           dH = monaLisa.getHeight() * scale;

        sr::math::RectF rect { mousePos.x - dW / 2.0f, mousePos.y - dH / 2.0f, dW, dH };
        rasterizer.drawImage( monaLisa, {}, rect );

        window.clear( Color::Black );
        window.present( colorTarget );
    }

    return 0;
}