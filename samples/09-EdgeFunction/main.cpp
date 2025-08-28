#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <imgui.h>

using namespace sr;

constexpr int   VERTEX_RADIUS  = 10;
constexpr float LINE_THICKNESS = 10;

void drawThickLine( Rasterizer& rasterizer, const glm::vec2& v0, const glm::vec2& v1, const Color& color )
{
    // Pseudocode:
    // 1. Set the rasterizer color to the given color.
    // 2. Compute the direction vector of the line (v1 - v0).
    // 3. Compute the perpendicular vector to the direction.
    // 4. Normalize the perpendicular vector.
    // 5. Scale the perpendicular vector by half the desired thickness.
    // 6. Compute the four corners of the thick line as a quad.
    // 7. Use rasterizer.drawQuad to draw the thick line.

    glm::vec2 dir = v1 - v0;
    if ( glm::length( dir ) == 0.0f )
        return;

    glm::vec2 perp          = glm::normalize( glm::vec2( -dir.y, dir.x ) );
    float     halfThickness = LINE_THICKNESS * 0.5f;

    glm::vec2 offset = perp * halfThickness;

    glm::vec2 p0 = v0 - offset;
    glm::vec2 p1 = v1 - offset;
    glm::vec2 p2 = v1 + offset;
    glm::vec2 p3 = v0 + offset;

    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.state.color    = color;
    rasterizer.drawQuad(
        glm::ivec2( glm::round( p0 ) ),
        glm::ivec2( glm::round( p1 ) ),
        glm::ivec2( glm::round( p2 ) ),
        glm::ivec2( glm::round( p3 ) ) );

    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.state.color    = Color::Black;
    rasterizer.drawQuad(
        glm::ivec2( glm::round( p0 ) ),
        glm::ivec2( glm::round( p1 ) ),
        glm::ivec2( glm::round( p2 ) ),
        glm::ivec2( glm::round( p3 ) ) );
}

void drawVertex( Rasterizer& rasterizer, const glm::ivec2& v, const Color& color )
{
    rasterizer.state.color    = color;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Black;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawCircle( v, VERTEX_RADIUS );
}

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

    drawThickLine( rasterizer, v0, v1, Color::Red );
    drawVertex( rasterizer, v0, Color::Magenta );
    drawVertex( rasterizer, v1, Color::Yellow );
}

void drawTriangle( Rasterizer& rasterizer, const glm::ivec2& v0, const glm::ivec2& v1, const glm::ivec2& v2 )
{
    Image* image = rasterizer.state.colorTarget;
    if ( !image )
        return;

    BlendMode blendMode = BlendMode::AdditiveBlend;

    for ( int y = 0; y < image->getHeight(); ++y )
    {
        for ( int x = 0; x < image->getWidth(); ++x )
        {
            if ( orient2D( v0, v1, { x, y } ) > 0 )
            {
                image->plot<false>( x, y, Color::Red, blendMode );
            }
            if ( orient2D( v1, v2, { x, y } ) > 0 )
            {
                image->plot<false>( x, y, Color::Green, blendMode );
            }
            if ( orient2D( v2, v0, { x, y } ) > 0 )
            {
                image->plot<false>( x, y, Color::Blue, blendMode );
            }
        }
    }

    drawThickLine( rasterizer, v0, v1, Color::Red );
    drawThickLine( rasterizer, v1, v2, Color::Green );
    drawThickLine( rasterizer, v2, v0, Color::Blue );

    drawVertex( rasterizer, v0, Color::Magenta );
    drawVertex( rasterizer, v1, Color::Yellow );
    drawVertex( rasterizer, v2, Color::Cyan );
}

void drawQuad( Rasterizer& rasterizer, const glm::ivec2& v0, const glm::ivec2& v1, const glm::ivec2& v2, const glm::ivec2& v3 )
{
    Image* image = rasterizer.state.colorTarget;
    if ( !image )
        return;

    BlendMode blendMode = BlendMode::AdditiveBlend;

    for ( int y = 0; y < image->getHeight(); ++y )
    {
        for ( int x = 0; x < image->getWidth(); ++x )
        {
            glm::ivec2 p { x, y };
            int        o01 = orient2D( v0, v1, p );
            int        o12 = orient2D( v1, v2, p );
            int        o20 = orient2D( v2, v0, p );
            int        o23 = orient2D( v2, v3, p );
            int        o30 = orient2D( v3, v0, p );
            int        o02 = orient2D( v0, v2, p );

            if ( o01 > 0 )
            {
                image->plot<false>( x, y, Color::Red, blendMode );
            }
            if ( o12 > 0 )
            {
                image->plot<false>( x, y, Color::Green, blendMode );
            }
            if ( o20 > 0 )
            {
                image->plot<false>( x, y, Color::Blue, blendMode );
            }
            if ( o23 > 0 )
            {
                image->plot<false>( x, y, Color::Red, blendMode );
            }
            if ( o30 > 0 )
            {
                image->plot<false>( x, y, Color::Green, blendMode );
            }
            if ( o02 > 0 )
            {
                image->plot<false>( x, y, Color::Blue, blendMode );
            }
        }
    }

    drawThickLine( rasterizer, v0, v1, Color::Red );
    drawThickLine( rasterizer, v1, v2, Color::Green );
    drawThickLine( rasterizer, v2, v0, Color::Blue );

    drawThickLine( rasterizer, v2, v3, Color::Red );
    drawThickLine( rasterizer, v3, v0, Color::Green );

    drawVertex( rasterizer, v0, Color::Magenta );
    drawVertex( rasterizer, v1, Color::Yellow );
    drawVertex( rasterizer, v2, Color::Cyan );
    drawVertex( rasterizer, v3, Color::Yellow );
}

int main()
{
    enum class DrawMode
    {
        Line,
        Triangle,
        Quad,
    } drawMode = DrawMode::Quad;

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

        // ImGui window for drawMode selection
        ImGui::Begin( "Draw Mode" );
        static const char* drawModeItems[] = { "Line", "Triangle", "Quad" };
        ImGui::Combo( "Draw Mode", reinterpret_cast<int*>( &drawMode ), drawModeItems, IM_ARRAYSIZE( drawModeItems ) );
        ImGui::End();

        image.clear( Color::Black );

        switch ( drawMode )
        {
        case DrawMode::Line:
            drawLine( rasterizer, verts[0], verts[1] );
            break;
        case DrawMode::Triangle:
            drawTriangle( rasterizer, verts[0], verts[1], verts[2] );
            break;
        case DrawMode::Quad:
            drawQuad( rasterizer, verts[0], verts[1], verts[2], verts[3] );
            break;
        }

        window.clear( Color::Black );
        window.present( image );
    }

    return 0;
}