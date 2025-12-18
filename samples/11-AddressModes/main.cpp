#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <imgui.h>

using namespace sr;

constexpr int SCREEN_WIDTH = 768;
constexpr int SCREEN_HEIGHT = 768;

int main()
{
    Window      window( "Clear Screen", SCREEN_WIDTH, SCREEN_HEIGHT );
    Image       image { SCREEN_WIDTH, SCREEN_HEIGHT };
    Image       texture { "assets/textures/256px-UV_Checker.png" };
    Rasterizer  rasterizer;
    Timer       timer;
    AddressMode addressMode = AddressMode::Clamp;
    Text        fpsText { Font::DefaultFont, "FPS: 0" };

    window.setVSync( false );

    int w = image.getWidth();
    int h = image.getHeight();

    // Define the quad vertices.
    Vertex2D verts[] = {
        { { 0, 0 }, { -1, -1 } },
        { { w, 0 }, { 2, -1 } },
        { { w, h }, { 2, 2 } },
        { { 0, h }, { -1, 2 } }
    };

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
                case SDLK_1:
                    addressMode = AddressMode::Wrap;
                    break;
                case SDLK_2:
                    addressMode = AddressMode::Mirror;
                    break;
                case SDLK_3:
                    addressMode = AddressMode::Clamp;
                    break;
                }
                break;
            }
        }

        // ImGui drop-down for AddressMode selection
        if ( ImGui::Begin( "Address Mode" ) )
        {
            static const char* addressModes[] = { "Wrap", "Mirror", "Clamp" };
            int                currentMode    = static_cast<int>( addressMode );
            if ( ImGui::Combo( "AddressMode", &currentMode, addressModes, IM_ARRAYSIZE( addressModes ) ) )
            {
                addressMode = static_cast<AddressMode>( currentMode );
            }
            ImGui::End();
        }

        image.clear( Color::Black );

        rasterizer.drawQuad( verts[0], verts[1], verts[2], verts[3], texture, addressMode );

        if ( timer.totalSeconds() > 1.0 )
        {
            fpsText = std::format( "FPS: {:.0f}", timer.FPS() );
            timer.reset();
        }

        fpsText.setColor( Color::Black );
        rasterizer.drawText( fpsText, 12, 12 );
        fpsText.setColor( Color::White );
        rasterizer.drawText( fpsText, 10, 10 );

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}