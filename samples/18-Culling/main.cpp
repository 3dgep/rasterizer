#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <imgui.h>

using namespace sr;

enum class PrimitiveType
{
    Triangle,
    Quad
};

int main()
{
    Window        window( "Culling", 800, 600 );
    Image         image { 800, 600 };
    Image         texture { "assets/textures/Smiley.png" };
    Rasterizer    rasterizer;
    PrimitiveType primitiveType    = PrimitiveType::Triangle;
    Font          font             = Font { 30 };
    Text          clockwise        = Text( font, "Clockwise", Color::Black );
    Text          counterClockwise = Text( font, "Counter\nclockwise", Color::Black );

    Timer timer;
    Text  fpsText { Font::DefaultFont, "FPS: 0", Color::Black };

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

        image.clear( Color::White );

        // ImGui panel for rasterizer state
        ImGui::Begin( "Rasterizer State" );
        // Primitive type.
        static const char* primitiveTypes[] = { "Triangle", "Quad" };
        ImGui::Combo( "Primitive Type", reinterpret_cast<int*>( &primitiveType ), primitiveTypes, IM_ARRAYSIZE( primitiveTypes ) );
        // Cull Mode
        static const char* cullModeItems[] = { "None", "Front", "Back" };
        ImGui::Combo( "Cull Mode", reinterpret_cast<int*>( &rasterizer.state.cullMode ), cullModeItems, IM_ARRAYSIZE( cullModeItems ) );
        // Front Counter-clockwise
        ImGui::Checkbox( "Front Counter Clockwise", &rasterizer.state.frontCounterClockwise );
        ImGui::End();

        switch ( primitiveType )
        {
        case PrimitiveType::Triangle:
        {
            {
                // Draw a clockwise triangle on the left.
                const Vertex2D v0 = { { 200, 150 }, { 0.5f, 0.0f } };
                const Vertex2D v1 = { { 350, 450 }, { 1.0f, 1.0f } };
                const Vertex2D v2 = { { 50, 450 }, { 0.0f, 1.0f } };

                rasterizer.drawTriangle( v0, v1, v2, texture );
            }
            {
                // Draw a counter-clockwise triangle on the right.
                const Vertex2D v0 = { { 600, 150 }, { 0.5f, 0.0f } };
                const Vertex2D v1 = { { 450, 450 }, { 1.0f, 1.0f } };
                const Vertex2D v2 = { { 750, 450 }, { 0.0f, 1.0f } };

                rasterizer.drawTriangle( v0, v1, v2, texture );
            }
        }
        break;
        case PrimitiveType::Quad:
        {
            {
                // Draw a clockwise quad on the left.
                const Vertex2D v0 = { { 50, 150 }, { 0.0f, 0.0f } };
                const Vertex2D v1 = { { 350, 150 }, { 1.0f, 0.0f } };
                const Vertex2D v2 = { { 350, 450 }, { 1.0f, 1.0f } };
                const Vertex2D v3 = { { 50, 450 }, { 0.0f, 1.0f } };

                rasterizer.drawQuad( v0, v1, v2, v3, texture );
            }
            {
                // Draw a counter-clockwise quad on the right.
                const Vertex2D v0 = { { 450, 150 }, { 0.0f, 0.0f } };
                const Vertex2D v1 = { { 450, 450 }, { 0.0f, 1.0f } };
                const Vertex2D v2 = { { 750, 450 }, { 1.0f, 1.0f } };
                const Vertex2D v3 = { { 750, 150 }, { 1.0f, 0.0f } };

                rasterizer.drawQuad( v0, v1, v2, v3, texture );
            }
        }
        break;
        }

        // "Clockwise" on the left.
        rasterizer.drawText( clockwise, 50, 75 );
        // "Counter-clockwise" on the right.
        rasterizer.drawText( counterClockwise, 450, 60 );

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