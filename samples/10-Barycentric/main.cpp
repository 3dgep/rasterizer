#include <Timer.hpp>

#include <graphics/Image.hpp>
#include <graphics/Rasterizer.hpp>
#include <graphics/Window.hpp>

#include <imgui.h>

#include <cstring> // for std::memcpy

using namespace sr;

constexpr int   VERTEX_RADIUS  = 10;
constexpr float LINE_THICKNESS = 10;

void drawVertex( Rasterizer& rasterizer, const Vertex2D& v )
{
    rasterizer.state.color    = v.color;
    rasterizer.state.fillMode = FillMode::Solid;
    rasterizer.drawCircle( v.position, VERTEX_RADIUS );

    rasterizer.state.color    = Color::Black;
    rasterizer.state.fillMode = FillMode::WireFrame;
    rasterizer.drawCircle( v.position, VERTEX_RADIUS );
}

void drawTriangle( Rasterizer& rasterizer, const Vertex2D& v0, const Vertex2D& v1, const Vertex2D& v2 )
{
    Image* image = rasterizer.state.colorTarget;

    if ( !image )
        return;

    BlendMode blendMode = BlendMode::AdditiveBlend;

    float area = static_cast<float>( orient2D( v0.position, v1.position, v2.position ) );

    AABB dstAABB = image->getAABB().clamped( AABB::fromTriangle( v0.position, v1.position, v2.position ) );

    int minX = static_cast<int>( dstAABB.min.x );
    int minY = static_cast<int>( dstAABB.min.y );
    int maxX = static_cast<int>( dstAABB.max.x );
    int maxY = static_cast<int>( dstAABB.max.y );

    for ( int y = minY; y <= maxY; ++y )
    {
        for ( int x = minX; x <= maxX; ++x )
        {
            glm::ivec2 p { x, y };

            int w0 = orient2D( v1.position, v2.position, p );
            int w1 = orient2D( v2.position, v0.position, p );
            int w2 = orient2D( v0.position, v1.position, p );

            if ( ( w0 | w1 | w2 ) >= 0 )
            {
                // Compute barycentric coordinates.
                float bc0 = static_cast<float>( w0 ) / area;
                float bc1 = static_cast<float>( w1 ) / area;
                float bc2 = 1.0f - ( bc0 + bc1 );  // Must sum to 1!

                Color color = v0.color * bc0 + v1.color * bc1 + v2.color * bc2;

                image->plot<false, false>( x, y, color );
            }
        }
    }

    drawVertex( rasterizer, v0 );
    drawVertex( rasterizer, v1 );
    drawVertex( rasterizer, v2 );
}

void drawTexturedTriangle( Rasterizer& rasterizer, const Vertex2D& v0, const Vertex2D& v1, const Vertex2D& v2, const Image& texture )
{
    Image* image = rasterizer.state.colorTarget;

    if ( !image )
        return;

    BlendMode blendMode = BlendMode::AdditiveBlend;

    float area = static_cast<float>( orient2D( v0.position, v1.position, v2.position ) );

    AABB dstAABB = image->getAABB().clamped( AABB::fromTriangle( v0.position, v1.position, v2.position ) );

    int minX = static_cast<int>( dstAABB.min.x );
    int minY = static_cast<int>( dstAABB.min.y );
    int maxX = static_cast<int>( dstAABB.max.x );
    int maxY = static_cast<int>( dstAABB.max.y );

    for ( int y = minY; y <= maxY; ++y )
    {
        for ( int x = minX; x < maxX; ++x )
        {
            glm::ivec2 p { x, y };

            int w0 = orient2D( v1.position, v2.position, p );
            int w1 = orient2D( v2.position, v0.position, p );
            int w2 = orient2D( v0.position, v1.position, p );

            if ( ( w0 | w1 | w2 ) >= 0 )
            {
                // Compute barycentric coordinates.
                float bc0 = static_cast<float>( w0 ) / area;
                float bc1 = static_cast<float>( w1 ) / area;
                float bc2 = 1.0f - ( bc0 + bc1 );  // Must sum to 1!

                glm::vec2 texCoord = v0.texCoord * bc0 + v1.texCoord * bc1 + v2.texCoord * bc2;
                // Color     color    = Color { texCoord.x, texCoord.y, 0.0f };
                Color color = texture.sample( texCoord, SamplerState::ClampNormalized );
                image->plot<false, false>( x, y, color );
            }
        }
    }

    drawVertex( rasterizer, v0 );
    drawVertex( rasterizer, v1 );
    drawVertex( rasterizer, v2 );
}
int main()
{
    enum class DrawMode
    {
        Barycentric,
        Textured
    } drawMode = DrawMode::Barycentric;

    Window     window( "Barycentric Coordinates", 800, 600 );
    Image      image { 800, 600 };
    Image      texture { "assets/textures/Smiley.png" };
    Rasterizer rasterizer;
    Text       fpsText( Font::DefaultFont, "FPS: 0" );
    Timer      timer;

    Vertex2D initialVerts[] = {
        { { 3 * image.getWidth() / 4, image.getHeight() / 4 }, { 1.0f, 0.0f }, Color::Red },
        { { 3 * image.getWidth() / 4, 3 * image.getHeight() / 4 }, { 1.0f, 1.0f }, Color::Lime },
        { { image.getWidth() / 4, 3 * image.getHeight() / 4 }, { 0.0f, 1.0f }, Color::Blue }
    };

    Vertex2D verts[std::size( initialVerts )];
    std::memcpy( verts, initialVerts, std::size( initialVerts ) * sizeof( Vertex2D ) );

    // Set if the mouse is clicked over one of the vertices.
    Vertex2D* selectedVert = nullptr;
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
                case SDLK_ESCAPE:
                    window.destroy();
                    break;
                case SDLK_1:
                    drawMode = DrawMode::Barycentric;
                    break;
                case SDLK_2:
                    drawMode = DrawMode::Textured;
                    break;
                case SDLK_R:
                    // Reset vertices to their original positions.
                    std::memcpy( verts, initialVerts, std::size( initialVerts ) * sizeof( Vertex2D ) );
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
                    if ( Circle { v.position, VERTEX_RADIUS }.intersect( mousePos ) )
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
                    selectedVert->position += mouseDelta;
                }
                break;
            }
        }

        // ImGui: DrawMode selection
        {
            const char* drawModeItems[] = { "Barycentric", "Textured" };
            int         drawModeIndex   = static_cast<int>( drawMode );
            if ( ImGui::Begin( "Settings" ) )
            {
                if ( ImGui::Combo( "Draw Mode", &drawModeIndex, drawModeItems, IM_ARRAYSIZE( drawModeItems ) ) )
                {
                    drawMode = static_cast<DrawMode>( drawModeIndex );
                }
                ImGui::End();
            }
        }

        image.clear( Color::Black );

        switch ( drawMode )
        {
        case DrawMode::Barycentric:
             drawTriangle( rasterizer, verts[0], verts[1], verts[2] );
            break;
        case DrawMode::Textured:
            drawTexturedTriangle( rasterizer, verts[0], verts[1], verts[2], texture );
            break;
        }

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