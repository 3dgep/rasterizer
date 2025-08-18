#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Vertex.hpp>
#include <graphics/Window.hpp>

#include <Timer.hpp>

using namespace sr;

static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 600;

int main()
{
    Window     window( "Fill Rules", WINDOW_WIDTH, WINDOW_HEIGHT );
    Image      image { 128, 128 };
    Rasterizer rasterizer;
    Timer      timer;

    rasterizer.state.colorTarget = &image;

    while ( window )
    {
        SDL_Event event;
        while ( window.pollEvent( event ) )
        {
            switch ( event.type )
            {
            case SDL_EVENT_WINDOW_RESIZED:
                //                image.resize( event.window.data1, event.window.data2 );
                break;
            case SDL_EVENT_KEY_DOWN:
                switch ( event.key.key )
                {
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                case SDLK_R:
                    timer.reset();
                    break;
                case SDLK_V:
                    window.toggleVSync();
                    break;
                case SDLK_RETURN:
                    if ( ( event.key.mod & SDL_KMOD_ALT ) != 0 )
                    {
                    case SDLK_F11:  // NOLINT(clang-diagnostic-implicit-fallthrough)
                        window.toggleFullscreen();
                    }
                    break;
                }
                break;
            }
        }

        timer.tick();

        float w = static_cast<float>( image.width() );
        float h = static_cast<float>( image.height() );

        // Quad vertices.
        // glm::vec2 verts[] = {
        //    { -0.5f, -0.5f },  // Top-left
        //    { 0.5f, -0.5f },  // Top-right
        //    { 0.5f, 0.5f },  // Bottom-right
        //    { -0.5f, 0.5f },  // Bottom-left
        //};

        //glm::vec2 verts[] = {
        //    { 0, 0 },  // Top-left
        //    { 1, 0 },  // Top-right
        //    { 1, 1 },  // Bottom-right
        //    { 0, 1 },  // Bottom-left
        //};

        glm::vec2 verts[] = {
            { w * 0.25f, h * 0.25f },  // Top-left
            { w * 0.75f, h * 0.25f },  // Top-right
            { w * 0.75f, h * 0.75f },  // Bottom-right
            { w * 0.25f, h * 0.75f },  // Bottom-left
        };

        Transform2D transform {
            { 0, 0 }, { 1, 1 }, static_cast<float>( timer.totalSeconds() ), { w * 0.25f, h * 0.25f }
        };

        for ( auto& v: verts )
        {
            v = transform.transformPoint( v );
        }

        image.clear( Color::White );

        rasterizer.state.fillMode  = FillMode::Solid;
        rasterizer.state.color     = Color::Gray.withAlpha( 0.5f );
        rasterizer.state.blendMode = BlendMode::AdditiveBlend;
        rasterizer.drawQuad( verts[0], verts[1], verts[2], verts[3] );

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}