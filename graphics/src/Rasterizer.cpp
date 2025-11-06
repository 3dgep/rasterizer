#include <graphics/Rasterizer.hpp>
#include <graphics/Vertex.hpp>
#include <math/Intrinsics.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_query.hpp>  // glm::isIdentity.

#include <iostream>

using namespace sr::graphics;
using namespace sr::math;

// 2D Edge functions for triangle rasterization.
// See: https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
struct Edge2D
{
    glm::ivec3 dX;    // X deltas.
    glm::ivec3 dY;    // Y deltas.
    glm::ivec3 w0;    // Starting weights per row.
    glm::ivec3 w;     // Current weight.
    float      area;  // Area of the triangle (used to compute the barycentric coordinates).

    Edge2D( const glm::ivec2& p0, const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p )
    : dX { p2.x - p1.x, p0.x - p2.x, p1.x - p0.x }
    , dY { p1.y - p2.y, p2.y - p0.y, p0.y - p1.y }
    {
        area = static_cast<float>( orient2D( p0, p1, p2 ) );

        int bias0 = isTopLeft( p1, p2 ) ? 0 : -1;
        int bias1 = isTopLeft( p2, p0 ) ? 0 : -1;
        int bias2 = isTopLeft( p0, p1 ) ? 0 : -1;

        w0.x = orient2D( p1, p2, p ) + bias0;
        w0.y = orient2D( p2, p0, p ) + bias1;
        w0.z = orient2D( p0, p1, p ) + bias2;

        // If using pixel centers, add half-pixel offset (0.5, 0.5)
        // orient2D(a, b, p) = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x)
        // Adding 0.5 to p.x and p.y means we add: 0.5 * (b.x - a.x - b.y + a.y)
        w0 += ( dX + dY ) / 2;  // Half-pixel offset
        w = w0;
    }

    bool inside() const
    {
        return ( w.x | w.y | w.z ) >= 0;  // Check if all weights are non-negative.
    }

    void stepX()
    {
        w += dY;
    }

    void stepY()
    {
        w0 += dX;
        w = w0;
    }

    glm::vec3 barycentric() const
    {
        float u = static_cast<float>( w.x ) / area;
        float v = static_cast<float>( w.y ) / area;

        return { u, v, 1.0f - ( u + v ) };
    }
};

void Rasterizer::drawText( const Font& font, std::string_view text, int x, int y )
{
    drawText( Text { font, text, state.color }, x, y );
}

void Rasterizer::drawText( const Font& font, std::wstring_view text, int x, int y )
{
    drawText( Text { font, text, state.color }, x, y );
}

void Rasterizer::drawText( const Text& text, int x, int y )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    if ( !TTF_DrawSurfaceText( text.getTTF_Text(), x, y, image->getSDL_Surface() ) )
    {
        std::cerr << "Failed to draw text to the surface: " << SDL_GetError() << std::endl;
    }
}

// Source: Claud Sonnet 4 "Create a 2D Software Rasterizer in C++"
void Rasterizer::drawLineLow( int x0, int y0, int x1, int y1 )
{
    Image*    image     = state.colorTarget;
    BlendMode blendMode = state.blendMode;

    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;

    if ( dy < 0 )
    {
        yi = -1;
        dy = -dy;
    }

    int D = 2 * dy - dx;
    int y = y0;

    for ( int x = x0; x <= x1; ++x )
    {
        image->plot<false>( x, y, state.color, blendMode );

        if ( D > 0 )
        {
            y += yi;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

void Rasterizer::drawLineHigh( int x0, int y0, int x1, int y1 )
{
    Image*    image     = state.colorTarget;
    BlendMode blendMode = state.blendMode;

    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if ( dx < 0 )
    {
        xi = -1;
        dx = -dx;
    }

    int D = 2 * dx - dy;
    int x = x0;

    for ( int y = y0; y <= y1; ++y )
    {
        image->plot<false>( x, y, state.color, blendMode );

        if ( D > 0 )
        {
            x += xi;
            D -= 2 * dy;
        }

        D += 2 * dx;
    }
}

void Rasterizer::clear( const Color& color )
{
    Image* image = state.colorTarget;
    if ( image )
        image->clear( color );
}

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    auto aabb = image->getAABB();
    aabb.clamp( AABB::fromViewport( state.viewport ) );

    if ( !aabb.clip( x0, y0, x1, y1 ) )
        return;

    if ( std::abs( y1 - y0 ) < std::abs( x1 - x0 ) )
    {
        if ( x0 > x1 )
        {
            drawLineLow( x1, y1, x0, y0 );
        }
        else
        {
            drawLineLow( x0, y0, x1, y1 );
        }
    }
    else
    {
        if ( y0 > y1 )
        {
            drawLineHigh( x1, y1, x0, y0 );
        }
        else
        {
            drawLineHigh( x0, y0, x1, y1 );
        }
    }
}

// Source: Grok (Aug 27, 2025): What is the most efficient way to draw a circle in a 2D software rasterizer?
void Rasterizer::drawCircle( int cx, int cy, int r )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    AABB aabb       = image->getAABB().clamped( AABB::fromViewport( state.viewport ) );
    AABB circleAABB = AABB::fromCircle( Circle { { cx, cy }, static_cast<float>( r ) } );

    if ( !circleAABB.intersect( aabb ) )
        return;

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    switch ( state.fillMode )
    {
    case FillMode::WireFrame:
    {
        while ( x <= y )
        {
            // Plot the 8 octants of the circle.
            for ( auto offset: { glm::ivec2 { x, y }, glm::ivec2 { -x, y }, glm::ivec2 { x, -y }, glm::ivec2 { -x, -y }, glm::ivec2 { y, x }, glm::ivec2 { -y, x }, glm::ivec2 { y, -x }, glm::ivec2 { -y, x }, glm::ivec2 { -y, -x } } )
            {
                glm::ivec2 p { cx + offset.x, cy + offset.y };
                if ( aabb.contains( p ) )  // Clipping.
                {
                    image->plot<false>( p.x, p.y, state.color, state.blendMode );
                }
            }

            ++x;
            if ( d < 0 )
            {
                d += 4 * x + 6;
            }
            else
            {
                --y;
                d += 4 * ( x - y ) + 10;
            }
        }
    }
    break;
    case FillMode::Solid:
    {
        while ( x <= y )
        {
            // Draw horizontal lines to fill the circle.
            for ( auto offset: { glm::ivec4 { -x, y, x, y }, glm::ivec4 { -x, -y, x, -y }, glm::ivec4 { -y, x, y, x }, glm::ivec4 { -y, -x, y, -x } } )
            {
                // Lines are already clipped.
                drawLine( cx + offset.x, cy + offset.y, cx + offset.z, cy + offset.w );
            }

            ++x;
            if ( d < 0 )
            {
                d += 4 * x + 6;
            }
            else
            {
                --y;
                d += 4 * ( x - y ) + 10;
            }
        }
    }
    break;
    }
}

void Rasterizer::drawTriangle( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2 )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    switch ( state.fillMode )
    {
    case FillMode::WireFrame:
    {
        drawLine( p0, p1 );
        drawLine( p1, p2 );
        drawLine( p2, p0 );
    }
    break;
    case FillMode::Solid:
    {
        const int area = orient2D( p0, p1, p2 );
        if ( area == 0 )  // Skip degenerate triangles.
            return;

        bool ccw     = area > 0;
        bool isFront = state.frontCounterClockwise ? ccw : !ccw;

        switch ( state.cullMode )
        {
        case CullMode::Front:  // Cull front-facing triangles.
            if ( isFront )
                return;
            break;
        case CullMode::Back:  // Cull back-facing triangles.
            if ( !isFront )
                return;
            break;
        case CullMode::None:  // No culling.
            break;
        }

        // Swap vertices if triangle is not counter-clockwise.
        if ( !ccw )
            std::swap( p1, p2 );

        auto aabb         = image->getAABB().clamped( AABB { state.viewport } );
        auto triangleAABB = AABB::fromTriangle( p0, p1, p2 );

        if ( !triangleAABB.intersect( aabb ) )
            return;

        aabb.clamp( triangleAABB );

        int minX = static_cast<int>( aabb.min.x );
        int minY = static_cast<int>( aabb.min.y );
        int maxX = static_cast<int>( aabb.max.x );
        int maxY = static_cast<int>( aabb.max.y );

        glm::ivec2 p { minX, minY };

        // Edge setup.
        Edge2D e = Edge2D { p0, p1, p2, p };  // Flip winding order if vertices are the wrong way around.

        for ( p.y = minY; p.y <= maxY; p.y++ )
        {
            for ( p.x = minX; p.x <= maxX; p.x++ )
            {
                if ( e.inside() )
                    image->plot<false>( p.x, p.y, state.color, state.blendMode );

                e.stepX();
            }

            e.stepY();
        }
    }
    break;
    }
}

void Rasterizer::drawTriangle( Vertex2Di v0, Vertex2Di v1, Vertex2Di v2, const Image& texture, AddressMode addressMode, std::optional<BlendMode> _blendMode )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    const int area = orient2D( v0.position, v1.position, v2.position );

    if ( area == 0 )  // Ignore degenerate triangles.
        return;

    bool ccw     = area > 0;
    bool isFront = state.frontCounterClockwise ? ccw : !ccw;

    switch ( state.cullMode )
    {
    case CullMode::Front:  // Cull front-facing triangles.
        if ( isFront )
            return;
        break;
    case CullMode::Back:  // Cull back-facing triangles.
        if ( !isFront )
            return;
        break;
    case CullMode::None:  // No culling.
        break;
    }

    // Swap vertices if triangle is not counter-clockwise.
    if ( !ccw )
        std::swap( v1, v2 );

    const BlendMode blendMode    = _blendMode.value_or( state.blendMode );
    auto            aabb         = image->getAABB().clamped( AABB { state.viewport } );
    auto            triangleAABB = AABB::fromTriangle( v0.position, v1.position, v2.position );

    if ( !triangleAABB.intersect( aabb ) )
        return;

    aabb.clamp( triangleAABB );

    int minX = static_cast<int>( aabb.min.x );
    int minY = static_cast<int>( aabb.min.y );
    int maxX = static_cast<int>( aabb.max.x );
    int maxY = static_cast<int>( aabb.max.y );

    glm::ivec2 p { minX, minY };

    // Edge setup.
    Edge2D e = Edge2D { v0.position, v1.position, v2.position, p };

    for ( p.y = minY; p.y <= maxY; p.y++ )
    {
        for ( p.x = minX; p.x <= maxX; p.x++ )
        {
            if ( e.inside() )
            {
                const glm::vec3  bc       = e.barycentric();
                const glm::ivec2 texCoord = glm::round( math::interpolate( v0.texCoord, v1.texCoord, v2.texCoord, bc ) );
                const Color      color    = interpolate( v0.color, v1.color, v2.color, bc );
                const Color      srcColor = texture.sample( texCoord.x, texCoord.y, addressMode ) * color;
                image->plot<false>( p.x, p.y, srcColor, blendMode );
            }

            e.stepX();
        }

        e.stepY();
    }
}

void Rasterizer::drawTriangle( const Vertex2D& _v0, const Vertex2D& _v1, const Vertex2D& _v2, const Image& texture, AddressMode addressMode, std::optional<BlendMode> blendMode )
{
    // Convert to integer texture coordinates.
    float w = static_cast<float>( texture.getWidth() - 1 );
    float h = static_cast<float>( texture.getHeight() - 1 );

    const Vertex2Di v0 { _v0.position, { _v0.texCoord.x * w, _v0.texCoord.y * h }, _v0.color };
    const Vertex2Di v1 { _v1.position, { _v1.texCoord.x * w, _v1.texCoord.y * h }, _v1.color };
    const Vertex2Di v2 { _v2.position, { _v2.texCoord.x * w, _v2.texCoord.y * h }, _v2.color };

    drawTriangle( v0, v1, v2, texture, addressMode, blendMode );
}

void Rasterizer::drawQuad( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3 )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    AABB dstAABB = image->getAABB().clamped( AABB { state.viewport } );
    AABB srcAABB = AABB { p0, p1, p2, p3 };

    if ( !srcAABB.intersect( dstAABB ) )
        return;

    dstAABB.clamp( srcAABB );

    switch ( state.fillMode )
    {
    case FillMode::WireFrame:
        drawLine( p0, p1 );
        drawLine( p1, p2 );
        drawLine( p2, p3 );
        drawLine( p3, p0 );
        break;
    case FillMode::Solid:
    {
        // Check culling for both triangles of the quad.
        int area1 = orient2D( p0, p1, p2 );
        int area2 = orient2D( p2, p3, p0 );

        // Check for degenerate cases.
        if ( area1 == 0 && area2 == 0 )
            return;

        bool ccw1 = area1 > 0;
        bool ccw2 = area2 > 0;

        bool isFront1 = state.frontCounterClockwise ? ccw1 : !ccw1;
        bool isFront2 = state.frontCounterClockwise ? ccw2 : !ccw2;

        switch ( state.cullMode )
        {
        case CullMode::Front:
            if ( isFront1 && isFront2 )
                return;
            break;
        case CullMode::Back:
            if ( !( isFront1 && isFront2 ) )
                return;
            break;
        case CullMode::None:
            break;
        }

        // Swap vertices if triangles are not counter-clockwise.
        if ( !ccw1 )
            std::swap( p0, p1 );

        if ( !ccw2 )
            std::swap( p2, p3 );

        int minX = static_cast<int>( dstAABB.min.x );
        int minY = static_cast<int>( dstAABB.min.y );
        int maxX = static_cast<int>( dstAABB.max.x );
        int maxY = static_cast<int>( dstAABB.max.y );

        glm::ivec2 p { minX, minY };

        // Edge setup
        Edge2D e[] = {
            { p0, p1, p2, p },
            { p2, p3, p0, p },
        };

        for ( p.y = minY; p.y <= maxY; ++p.y )
        {
            for ( p.x = minX; p.x <= maxX; ++p.x )
            {
                for ( const auto& i: e )
                {
                    if ( i.inside() )
                    {
                        image->plot<false>( p.x, p.y, state.color, state.blendMode );
                    }
                }

                e[0].stepX();
                e[1].stepX();
            }
            e[0].stepY();
            e[1].stepY();
        }
    }
    break;
    }
}

void Rasterizer::drawQuad( Vertex2Di v0, Vertex2Di v1, Vertex2Di v2, Vertex2Di v3, const Image& texture, AddressMode addressMode, std::optional<BlendMode> _blendMode )
{
    Image* dstImage = state.colorTarget;

    if ( !dstImage )
        return;

    // Check culling for both triangles of the quad.
    int area1 = orient2D( v0.position, v1.position, v2.position );
    int area2 = orient2D( v2.position, v3.position, v0.position );

    // Check for degenerate cases.
    if ( area1 == 0 && area2 == 0 )
        return;

    bool ccw1 = area1 > 0;
    bool ccw2 = area2 > 0;

    bool isFront1 = state.frontCounterClockwise ? ccw1 : !ccw1;
    bool isFront2 = state.frontCounterClockwise ? ccw2 : !ccw2;

    switch ( state.cullMode )
    {
    case CullMode::Front:
        if ( isFront1 && isFront2 )
            return;
        break;
    case CullMode::Back:
        if ( !( isFront1 && isFront2 ) )
            return;
        break;
    case CullMode::None:
        break;
    }

    // Swap vertices if triangles are not counter-clockwise.
    if ( !ccw1 )
        std::swap( v0, v1 );

    if ( !ccw2 )
        std::swap( v2, v3 );

    BlendMode blendMode = _blendMode.value_or( state.blendMode );
    AABB      dstAABB   = dstImage->getAABB().clamped( AABB::fromViewport( state.viewport ) );

    // Compute the AABB over the quad vertices.
    AABB srcAABB = AABB {
        v0.position, v1.position, v2.position, v3.position
    };

    // If the sprite AABB doesn't overlap with the destination AABB.
    if ( !srcAABB.intersect( dstAABB ) )
        return;

    // Clamp the dstAABB by the srcAABB.
    dstAABB.clamp( srcAABB );

    const int minX = static_cast<int>( dstAABB.min.x );
    const int minY = static_cast<int>( dstAABB.min.y );
    const int maxX = static_cast<int>( dstAABB.max.x );
    const int maxY = static_cast<int>( dstAABB.max.y );

    glm::ivec2 p { minX, minY };

    Edge2D e[] {
        Edge2D { v0.position, v1.position, v2.position, p },
        Edge2D { v2.position, v3.position, v0.position, p }
    };

    const Vertex2Di verts[] = {
        v0, v1, v2, v3
    };

    const uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Compute valid texture coordinate bounds from vertices to prevent bleeding into tile margins
    // Vertices have texture coords offset by -0.5 for pixel center sampling
    const glm::vec2 minVertexTC = glm::min( glm::min( v0.texCoord, v1.texCoord ), glm::min( v2.texCoord, v3.texCoord ) );
    const glm::vec2 maxVertexTC = glm::max( glm::max( v0.texCoord, v1.texCoord ), glm::max( v2.texCoord, v3.texCoord ) );
    // Convert from vertex coords (offset by -0.5) to valid sampling range
    const glm::vec2 minTexCoord = glm::floor( minVertexTC + 0.5f );
    const glm::vec2 maxTexCoord = glm::ceil( maxVertexTC - 0.5f );

    for ( p.y = minY; p.y <= maxY; ++p.y )
    {
        for ( p.x = minX; p.x <= maxX; ++p.x )
        {
            for ( uint32_t i = 0; i < 2; ++i )
            {
                const Edge2D& edge = e[i];
                if ( edge.inside() )
                {
                    const uint32_t i0 = indices[i * 3 + 0];
                    const uint32_t i1 = indices[i * 3 + 1];
                    const uint32_t i2 = indices[i * 3 + 2];

                    const Vertex2Di& a = verts[i0];
                    const Vertex2Di& b = verts[i1];
                    const Vertex2Di& c = verts[i2];

                    const glm::vec3 bc             = edge.barycentric();
                    const glm::vec2 interpTexCoord = sr::math::interpolate( a.texCoord, b.texCoord, c.texCoord, bc );

                    // Use SIMD-optimized round and clamp
                    glm::ivec2 texCoord;
                    simd_round_clamp_vec2( &interpTexCoord.x, &minTexCoord.x, &maxTexCoord.x, &texCoord.x );

                    const Color color    = interpolate( a.color, b.color, c.color, bc );
                    const Color srcColor = texture.sample( texCoord.x, texCoord.y, addressMode ) * color;
                    dstImage->plot<false>( p.x, p.y, srcColor, blendMode );
                }
            }

            e[0].stepX();
            e[1].stepX();
        }

        e[0].stepY();
        e[1].stepY();
    }
}

void Rasterizer::drawQuad( const Vertex2D& _v0, const Vertex2D& _v1, const Vertex2D& _v2, const Vertex2D& _v3, const Image& texture, AddressMode addressMode, std::optional<BlendMode> blendMode )
{
    // Convert to integer texture coordinates.
    float w = static_cast<float>( texture.getWidth() - 1 );
    float h = static_cast<float>( texture.getHeight() - 1 );

    const Vertex2Di v0 { _v0.position, { _v0.texCoord.x * w, _v0.texCoord.y * h }, _v0.color };
    const Vertex2Di v1 { _v1.position, { _v1.texCoord.x * w, _v1.texCoord.y * h }, _v1.color };
    const Vertex2Di v2 { _v2.position, { _v2.texCoord.x * w, _v2.texCoord.y * h }, _v2.color };
    const Vertex2Di v3 { _v3.position, { _v3.texCoord.x * w, _v3.texCoord.y * h }, _v3.color };

    drawQuad( v0, v1, v2, v3, texture, addressMode, blendMode );
}

void Rasterizer::drawAABB( math::AABB aabb )
{
    Image*   image    = state.colorTarget;
    Viewport viewport = state.viewport;

    if ( !image )
        return;

    AABB imageAABB = image->getAABB();
    imageAABB.clamp( AABB::fromViewport( viewport ) );

    if ( !aabb.intersect( imageAABB ) )
        return;

    switch ( state.fillMode )
    {
    case FillMode::WireFrame:
        drawLine( aabb.min.x, aabb.min.y, aabb.max.x, aabb.min.y );
        drawLine( aabb.max.x, aabb.min.y, aabb.max.x, aabb.max.y );
        drawLine( aabb.max.x, aabb.max.y, aabb.min.x, aabb.max.y );
        drawLine( aabb.min.x, aabb.max.y, aabb.min.x, aabb.min.y );
        break;
    case FillMode::Solid:
        aabb.clamp( imageAABB );
        for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
        {
            for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
            {
                image->plot<false>( x, y, state.color, state.blendMode );
            }
        }
        break;
    }
}

void Rasterizer::drawImage( const Image& srcImage, int x, int y )
{
    Image* dstImage = state.colorTarget;
    if ( !dstImage )
        return;

    int srcW = srcImage.getWidth();
    int srcH = srcImage.getHeight();
    int dstW = dstImage->getWidth();

    // Clamp destination rectangle to viewport and image bounds
    AABB dstAABB    = dstImage->getAABB().clamped( AABB::fromViewport( state.viewport ) );
    int  clipLeft   = std::max( static_cast<int>( dstAABB.min.x ), x );
    int  clipTop    = std::max( static_cast<int>( dstAABB.min.y ), y );
    int  clipRight  = std::min( static_cast<int>( dstAABB.max.x ), x + srcW - 1 );
    int  clipBottom = std::min( static_cast<int>( dstAABB.max.y ), y + srcH - 1 );

    if ( clipLeft > clipRight || clipTop > clipBottom )
        return;

    const Color* src       = srcImage.data();
    Color*       dst       = dstImage->data();
    BlendMode    blendMode = state.blendMode;
    Color        color     = state.color;

    for ( int dy = clipTop; dy <= clipBottom; ++dy )
    {
        for ( int dx = clipLeft; dx <= clipRight; ++dx )
        {
            int sx = dx - x;
            int sy = dy - y;

            Color sC = src[sy * srcW + sx] * color;
            Color dC = dst[dy * dstW + dx];

            dst[dy * dstW + dx] = blendMode.Blend( sC, dC );
        }
    }
}

void Rasterizer::drawImage( const Image& srcImage, std::optional<sr::math::RectI> srcRect, std::optional<sr::math::RectI> dstRect )
{
    Image* dstImage = state.colorTarget;
    if ( !dstImage )
        return;

    // Get source rectangle
    int srcX = 0, srcY = 0, srcW = srcImage.getWidth(), srcH = srcImage.getHeight();
    if ( srcRect )
    {
        srcX = srcRect->left;
        srcY = srcRect->top;
        srcW = srcRect->width;
        srcH = srcRect->height;
    }

    // Get destination rectangle
    int dstX = 0, dstY = 0, dstW = srcW, dstH = srcH;
    if ( dstRect )
    {
        dstX = dstRect->left;
        dstY = dstRect->top;
        dstW = dstRect->width;
        dstH = dstRect->height;
    }

    // Clamp destination rectangle to viewport and image bounds
    AABB dstAABB    = dstImage->getAABB().clamped( AABB::fromViewport( state.viewport ) );
    int  clipLeft   = std::max( static_cast<int>( dstAABB.min.x ), dstX );
    int  clipTop    = std::max( static_cast<int>( dstAABB.min.y ), dstY );
    int  clipRight  = std::min( static_cast<int>( dstAABB.max.x ), dstX + dstW - 1 );
    int  clipBottom = std::min( static_cast<int>( dstAABB.max.y ), dstY + dstH - 1 );

    if ( clipLeft > clipRight || clipTop > clipBottom )
        return;

    // Calculate scaling factors if needed
    float scaleX = static_cast<float>( srcW ) / dstW;
    float scaleY = static_cast<float>( srcH ) / dstH;

    const Color* src = srcImage.data();
    Color*       dst = dstImage->data();
    int          sW  = srcImage.getWidth();
    int          dW  = dstImage->getWidth();

    BlendMode blendMode = state.blendMode;
    Color     color     = state.color;

    for ( int y = clipTop; y <= clipBottom; ++y )
    {
        for ( int x = clipLeft; x <= clipRight; ++x )
        {
            // Map destination pixel to source pixel
            int u = srcX + static_cast<int>( ( x - dstX ) * scaleX );
            int v = srcY + static_cast<int>( ( y - dstY ) * scaleY );

            // Clamp source coordinates
            u = std::clamp( u, 0, srcImage.getWidth() - 1 );
            v = std::clamp( v, 0, srcImage.getHeight() - 1 );

            Color sC = src[v * sW + u] * color;
            Color dC = dst[y * dW + x];

            dst[y * dW + x] = blendMode.Blend( sC, dC );
        }
    }
}

void Rasterizer::drawSprite( const Sprite& sprite, int _x, int _y )
{
    const Image* srcImage = sprite.getImage().get();
    Image*       dstImage = state.colorTarget;

    if ( !srcImage || !dstImage )
        return;

    const Color      color        = sprite.getColor() * state.color;
    const BlendMode  blendMode    = sprite.getBlendMode();
    const AABB       viewportAABB = AABB::fromViewport( state.viewport );
    const AABB       dstAABB      = dstImage->getAABB().clamped( viewportAABB );
    const glm::ivec2 size         = sprite.getSize();
    glm::ivec2       uv           = sprite.getUV();

    // Compute viewport clipping bounds.
    const int clipLeft   = std::max( static_cast<int>( dstAABB.min.x ), _x );
    const int clipTop    = std::max( static_cast<int>( dstAABB.min.y ), _y );
    const int clipRight  = std::min( static_cast<int>( dstAABB.max.x ), _x + size.x - 1 );
    const int clipBottom = std::min( static_cast<int>( dstAABB.max.y ), _y + size.y - 1 );

    // Check if the sprite is completely off-screen.
    if ( clipLeft >= clipRight || clipTop >= clipBottom )
        return;

    // Adjust sprite UV based on clipping.
    uv.x += clipLeft - _x;
    uv.y += clipTop - _y;

    const Color* src = srcImage->data();
    Color*       dst = dstImage->data();

    int sW = srcImage->getWidth();  // Source image width.
    int dW = dstImage->getWidth();  // Destination image width.

    for ( int y = clipTop; y <= clipBottom; ++y )
    {
        for ( int x = clipLeft; x <= clipRight; ++x )
        {
            // Compute clipped UV sprite texture coordinates.
            int u = uv.x + ( x - clipLeft );
            int v = uv.y + ( y - clipTop );

            Color sC = src[v * sW + u] * color;
            Color dC = dst[y * dW + x];

            dst[y * dW + x] = blendMode.Blend( sC, dC );
        }
    }
}

void Rasterizer::drawSprite( const Sprite& sprite, const glm::mat3& transform )
{
    const Image* srcImage = sprite.getImage().get();
    Image*       dstImage = state.colorTarget;

    if ( !srcImage || !dstImage )
        return;

    // If the top-left 2x2 area of the matrix is identity, then there is no
    // scale or rotation. In this case, just use the fast path to draw the sprite.
    if ( glm::isIdentity( glm::mat2 { transform }, 0.0001f ) )
    {
        const int x = static_cast<int>( transform[2][0] );
        const int y = static_cast<int>( transform[2][1] );

        drawSprite( sprite, x, y );

        return;
    }

    const Color      color = sprite.getColor() * state.color;
    const glm::ivec2 uv    = sprite.getUV();
    const glm::ivec2 size  = sprite.getSize();

    // With pixel center sampling, adjust vertex texture coordinates
    // Position 0.5 (first pixel center) maps to texture coordinate 0
    // Position 31.5 (last pixel center) maps to texture coordinate 31
    Vertex2Di verts[4] = {
        { { 0, 0 }, { uv.x - 0.5f, uv.y - 0.5f }, color },                              // Top-left.
        { { size.x, 0 }, { uv.x + size.x - 0.5f, uv.y - 0.5f }, color },                // Top-right.
        { { size.x, size.y }, { uv.x + size.x - 0.5f, uv.y + size.y - 0.5f }, color },  // Bottom-right.
        { { 0, size.y }, { uv.x - 0.5f, uv.y + size.y - 0.5f }, color },                // Bottom-left.
    };

    // Transform vertices
    for ( Vertex2Di& v: verts )
    {
        v.position = transform * glm::vec3 { v.position, 1.0f };
    }

    drawQuad( verts[0], verts[1], verts[2], verts[3], *srcImage, AddressMode::Clamp, sprite.getBlendMode() );
}

void Rasterizer::drawTileMap( const TileMap& tileMap, int x, int y )
{
    int tileX        = 0;
    int tileY        = 0;
    int rows         = static_cast<int>( tileMap.getRows() );
    int columns      = static_cast<int>( tileMap.getColumns() );
    int spriteWidth  = static_cast<int>( tileMap.getSpriteWidth() );
    int spriteHeight = static_cast<int>( tileMap.getSpriteHeight() );

    for ( int i = 0; i < rows; ++i )
    {
        tileX = 0;
        for ( int j = 0; j < columns; ++j )
        {
            int spriteId = tileMap[j, i];
            if ( spriteId >= 0 )
            {
                drawSprite( tileMap.getSprite( j, i ), x + tileX, y + tileY );
            }
            tileX += spriteWidth;
        }
        tileY += spriteHeight;
    }
}

void Rasterizer::drawTileMap( const TileMap& tileMap, const glm::mat3& transform )
{
    // If the top-left 2x2 area of the matrix is identity, then there is no
    // scale or rotation. In this case, just use the fast path to draw the sprite.
    if ( glm::isIdentity( glm::mat2 { transform }, 0.0001f ) )
    {
        const int x = static_cast<int>( transform[2][0] );
        const int y = static_cast<int>( transform[2][1] );

        drawTileMap( tileMap, x, y );

        return;
    }

    glm::mat3 tileOffset { 1 };
    uint32_t  rows         = tileMap.getRows();
    uint32_t  columns      = tileMap.getColumns();
    uint32_t  spriteWidth  = tileMap.getSpriteWidth();
    uint32_t  spriteHeight = tileMap.getSpriteHeight();

    for ( uint32_t y = 0; y < rows; ++y )
    {
        for ( uint32_t x = 0; x < columns; ++x )
        {
            int spriteId = tileMap[x, y];
            if ( spriteId >= 0 )
            {
                tileOffset[2][0] = static_cast<float>( x * spriteWidth );
                tileOffset[2][1] = static_cast<float>( y * spriteHeight );

                drawSprite( tileMap.getSprite( x, y ), transform * tileOffset );
            }
        }
    }
}