#include "Game.hpp"
#include "Sound.hpp"

#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <input/Input.hpp>

using namespace sr;

int main()
{
    Window     window { "Pong", 454, 262 };
    Rasterizer rasterizer;
    Image      image { 454, 262 };
    Game       game { image.getWidth(), image.getHeight() };

    Timer timer;
    Text  fpsText { Font::DefaultFont, "FPS: 0" };

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

        float deltaTime = std::min( static_cast<float>( timer.elapsedSeconds() ), 1.0f / 60.0f );

        Input::update(); // Update the input state after polling events.
        Sound::updateAll( deltaTime );  // Update sound effects.

        //ImGui::ShowDemoWindow();

        image.clear( Color::Black );

        game.update( deltaTime );
        game.draw( rasterizer );

        if ( timer.totalSeconds() > 1.0 )
        {
            fpsText = std::format( "FPS: {:.0f}", timer.FPS() );
            timer.reset();
        }

        rasterizer.drawText( fpsText, 10, 10 );

        window.clear( Color::White );
        window.present( image );
    }

    return 0;
}