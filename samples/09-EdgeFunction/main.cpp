#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

using namespace sr;

constexpr int VERTEX_RADIUS = 10;

void drawLine( Rasterizer& rasterizer, const glm::ivec2& v0, const glm::ivec2& v1 )
{
    Image* image = rasterizer.state.colorTarget;
    if ( !image )
        return;

    for ( int y = 0; y < image->getHeight(); ++y )
    {
        for ( int x = 0; x < image->getWidth(); ++x )
        {
            if ( orient2D( v0, v1, { x, y } ) > 0 )
            {
                image->plot<false, false>( x, y, Color::Red );
            }
        }
    }

    rasterizer.state.color    = Color::Red;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v0, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Green;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v1, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Black;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawCircle( v0, VERTEX_RADIUS );
    rasterizer.drawCircle( v1, VERTEX_RADIUS );
}

void drawTriangle( Rasterizer& rasterizer, const glm::ivec2& v0, const glm::ivec2& v1, const glm::ivec2& v2 )
{
    Image* image = rasterizer.state.colorTarget;
    if ( !image )
        return;

    for ( int y = 0; y < image->getHeight(); ++y )
    {
        for ( int x = 0; x < image->getWidth(); ++x )
        {
            if ( orient2D( v0, v1, { x, y } ) > 0 )
            {
                image->plot<false, false>( x, y, Color::Red );
            }
            if ( orient2D( v1, v2, { x, y } ) > 0 )
            {
                image->plot<false, false>( x, y, Color::Green );
            }
            if ( orient2D( v2, v0, { x, y } ) > 0 )
            {
                image->plot<false, false>( x, y, Color::Blue );
            }
        }
    }

    rasterizer.state.color    = Color::Red;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v0, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Green;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v1, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Blue;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v2, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Black;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawCircle( v0, VERTEX_RADIUS );
    rasterizer.drawCircle( v1, VERTEX_RADIUS );
    rasterizer.drawCircle( v2, VERTEX_RADIUS );
}

int main()
{
    enum class DrawMode
    {
        Line,
        Triangle,
        Quad,
    } drawMode = DrawMode::Line;

    Window     window( "Edge Function", 800, 600 );
    Image      image { 800, 600 };
    Rasterizer rasterizer;
    Timer      timer;

    glm::vec2 initialVerts[] = {
        { image.getWidth() / 2, image.getHeight() / 4 },
        { image.getWidth() / 2, 3 * image.getHeight() / 4 },
        { image.getWidth() / 4, 3 * image.getHeight() / 4 },
        { image.getWidth() / 4, image.getHeight() / 4 },
    };

    glm::vec2 verts[std::size( initialVerts )];
    std::memcpy( verts, initialVerts, std::size( initialVerts ) * sizeof( glm::vec2 ) );

    // Set if the mouse is clicked over one of the vertices.
    glm::vec2* selectedVert = nullptr;

    glm::vec2 mousePos { 0 };

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
                case SDLK_1:
                    drawMode = DrawMode::Line;
                    break;
                case SDLK_2:
                    drawMode = DrawMode::Triangle;
                    break;
                case SDLK_3:
                    drawMode = DrawMode::Quad;
                    break;
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                case SDLK_R:
                    // Reset vertices to their original positions.
                    std::memcpy( verts, initialVerts, std::size( initialVerts ) * sizeof( glm::vec2 ) );
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
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mousePos = window.clientToImage( event.button.x, event.button.y, image );
                for ( auto& v: verts )
                {
                    if ( Circle { v, VERTEX_RADIUS }.intersect( mousePos ) )
                    {
                        selectedVert = &v;
                        break;
                    }
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                selectedVert = nullptr;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                if ( selectedVert )
                {
                    glm::vec2 mousePos2  = window.clientToImage( event.motion.x, event.motion.y, image );
                    glm::vec2 mouseDelta = mousePos2 - mousePos;
                    mousePos             = mousePos2;
                    *selectedVert += mouseDelta;
                }
                break;
            }
        }

        image.clear( Color::White );

        switch ( drawMode )
        {
        case DrawMode::Line:
            drawLine( rasterizer, verts[0], verts[1] );
            break;
        case DrawMode::Triangle:
            drawTriangle( rasterizer, verts[0], verts[1], verts[2] );
            break;
        case DrawMode::Quad:
            break;
        }

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}