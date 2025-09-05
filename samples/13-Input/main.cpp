#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <input/Input.hpp>

using namespace sr;

int main()
{
    Window     window( "Input", 800, 600 );
    Image      image { 800, 600 };
    Rasterizer rasterizer;
    Timer      timer;
    Text       fpsText { Font::DefaultFont, "FPS: 0" };
    Text       mousePosText { Font::DefaultFont, "0, 0" };

    // Setup the rasterizer's render target state.
    rasterizer.state.colorTarget = &image;

    while ( window )
    {
        timer.tick();

        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
            // switch ( event.type )
            //{
            // case SDL_EVENT_KEY_DOWN:
            //     switch ( event.key.key )
            //     {
            //     case SDLK_ESCAPE:
            //         window.destroy();
            //         break;
            //     case SDLK_V:
            //         window.toggleVSync();
            //         break;
            //     case SDLK_RETURN:
            //         if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
            //         {
            //         case SDLK_F11:
            //             window.toggleFullscreen();
            //         }
            //         break;
            //     }
            //     break;
            // }
        }

        Input::update();  // This has to be called once every frame to update the input state.

        if ( Input::getKeyDown( SDL_SCANCODE_ESCAPE ) )
            window.destroy();

        if ( Input::getKeyDown( SDL_SCANCODE_V ) )
            window.toggleVSync();

        if ( Input::getKeyDown( SDL_SCANCODE_F11 ) || Input::getMod( SDL_KMOD_ALT ) && Input::getKeyDown( SDL_SCANCODE_RETURN ) )
            window.toggleFullscreen();

        MouseState mouseState = Mouse::getState();
        // Convert mouse position to image space:
        auto imagePos = window.clientToImage( mouseState.x, mouseState.y, image );

        mousePosText = std::format( "({}, {})", static_cast<int>( imagePos.x ), static_cast<int>( imagePos.y ) );

        image.clear( Color::Black );

        rasterizer.drawCircle( imagePos, 7 );
        rasterizer.drawText( mousePosText, imagePos.x - mousePosText.getWidth() / 2, imagePos.y - mousePosText.getHeight() - 10);

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