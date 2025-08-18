#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Vertex.hpp>
#include <graphics/Window.hpp>

#include <Timer.hpp>

#include <numbers>

using namespace sr;

static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 600;

static constexpr float PI    = std::numbers::pi_v<float>;

int main()
{
    enum class TestCase
    {
        UnitCenter,
        UnitTopLeft,
        ScreenSpace
    } testCase = TestCase::UnitCenter;

    Window     window( "Fill Rules", WINDOW_WIDTH, WINDOW_HEIGHT );
    Image      image { 128, 128 };
    Rasterizer rasterizer;
    Timer      timer;
    bool       pause     = false;
    bool       drawEdges = true;
    float      rotation  = 0.0f;

    rasterizer.state.colorTarget = &image;

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
                case SDLK_1:
                    testCase = TestCase::UnitCenter;
                    break;
                case SDLK_2:
                    testCase = TestCase::UnitTopLeft;
                    break;
                case SDLK_3:
                    testCase = TestCase::ScreenSpace;
                    break;
                case SDLK_LEFT:
                    rotation -= 0.05f / PI;
                    break;
                case SDLK_RIGHT:
                    rotation += 0.05f / PI;
                    break;
                case SDLK_E:
                    drawEdges = !drawEdges;
                    break;
                case SDLK_SPACE:
                case SDLK_P:
                    pause = !pause;
                    break;
                case SDLK_R:
                    rotation = 0.0f;
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

        if ( !pause )
            rotation += static_cast<float>( timer.elapsedSeconds() );

        float w = static_cast<float>( image.width() );
        float h = static_cast<float>( image.height() );

        glm::vec2   verts[4] {};
        Transform2D transform;

        switch ( testCase )
        {
        case TestCase::UnitCenter:
            verts[0]  = { -0.5f, -0.5f };  // Top-left
            verts[1]  = { 0.5f, -0.5f };   // Top-right
            verts[2]  = { 0.5f, 0.5f };    // Bottom-right
            verts[3]  = { -0.5f, 0.5f };   // Bottom-left
            transform = Transform2D { { w * 0.5f, h * 0.5f }, rotation, { w * 0.5f, h * 0.5f }, { 0, 0 } };
            break;
        case TestCase::UnitTopLeft:
            verts[0]  = { 0, 0 };  // Top-left
            verts[1]  = { 1, 0 };  // Top-right
            verts[2]  = { 1, 1 };  // Bottom-right
            verts[3]  = { 0, 1 };  // Bottom-left
            transform = Transform2D { { ( w - 1.0f ) / 2.0f, ( h - 1 ) / 2.0f }, rotation, { w * 0.5f, h * 0.5f }, { 0.5f, 0.5f } };
            break;
        case TestCase::ScreenSpace:
            verts[0]  = { w * 0.25f, h * 0.25f };  // Top-left
            verts[1]  = { w * 0.75f, h * 0.25f };  // Top-right
            verts[2]  = { w * 0.75f, h * 0.75f };  // Bottom-right
            verts[3]  = { w * 0.25f, h * 0.75f };  // Bottom-left
            transform = Transform2D { { 0, 0 }, rotation, { 1, 1 }, { w * 0.5f, h * 0.5f } };
            break;
        }

        for ( auto& v: verts )
        {
            v = transform.transformPoint( v );
        }

        image.clear( Color::Black );

        rasterizer.state.fillMode  = FillMode::Solid;
        rasterizer.state.color     = Color::Gray.withAlpha( 0.5f );
        rasterizer.state.blendMode = BlendMode::AdditiveBlend;
        rasterizer.drawQuad( verts[0], verts[1], verts[2], verts[3] );

        if ( drawEdges )
        {
            rasterizer.state.color = Color::Red.withAlpha( 0.5f );
            if ( isTopLeft( verts[0], verts[1] ) )
            {
                rasterizer.drawLine( verts[0], verts[1] );
            }
            if ( isTopLeft( verts[1], verts[2] ) )
            {
                rasterizer.drawLine( verts[1], verts[2] );
            }
            if ( isTopLeft( verts[2], verts[3] ) )
            {
                rasterizer.drawLine( verts[2], verts[3] );
            }
            if ( isTopLeft( verts[3], verts[0] ) )
            {
                rasterizer.drawLine( verts[3], verts[0] );
            }

            // Draw diagonals.
            rasterizer.state.color = Color::Blue.withAlpha( 0.5f );
            if ( isTopLeft( verts[2], verts[0] ) )
            {
                rasterizer.drawLine( verts[2], verts[0] );
            }
            rasterizer.state.color = Color::Green.withAlpha( 0.5f );
            if ( isTopLeft( verts[0], verts[2] ) )
            {
                rasterizer.drawLine( verts[0], verts[2] );
            }
        }

        window.clear( Color::White );
        window.present( image );
    }

    return 0;
}