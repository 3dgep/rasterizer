#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <Timer.hpp>

using namespace sr;

static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 600;

int main()
{
    Window     window( "Triangle", WINDOW_WIDTH, WINDOW_HEIGHT );
    Image      image { WINDOW_WIDTH, WINDOW_HEIGHT };
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

        float w  = static_cast<float>( image.getWidth() );
        float h  = static_cast<float>( image.getHeight() );
        float dx = static_cast<float>( std::sin( timer.totalSeconds() * 0.5 ) * w * 0.5 );

        // Triangle vertices.
        glm::vec2 p0 = { w * 0.5f + dx, h * 0.25f };
        glm::vec2 p1 = { w * 0.25f + dx, h * 0.75f };
        glm::vec2 p2 = { w * 0.75f + dx, h * 0.75f };

        image.clear( Color::White );

        rasterizer.state.fillMode = FillMode::Solid;
        rasterizer.state.color    = Color::Red;
        rasterizer.drawTriangle( p0, p1, p2 );

        rasterizer.state.fillMode = FillMode::WireFrame;
        rasterizer.state.color    = Color::Blue;
        rasterizer.drawTriangle( p0, p1, p2 );

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}