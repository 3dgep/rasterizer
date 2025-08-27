#include <graphics/Rasterizer.hpp>
#include <graphics/Vertex.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_query.hpp>  // glm::isIdentity.

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
    : dX { p1.x - p0.x, p2.x - p1.x, p0.x - p2.x }
    , dY { p0.y - p1.y, p1.y - p2.y, p2.y - p0.y }
    {
        area = static_cast<float>( orient2D( p0, p1, p2 ) );

        int bias0 = isTopLeft( p1, p2 ) ? 0 : -1;
        int bias1 = isTopLeft( p2, p0 ) ? 0 : -1;
        int bias2 = isTopLeft( p0, p1 ) ? 0 : -1;

        w0.x = orient2D( p1, p2, p ) + bias0;
        w0.y = orient2D( p2, p0, p ) + bias1;
        w0.z = orient2D( p0, p1, p ) + bias2;

        w = w0;
    }

    bool inside() const
    {
        return ( w.x | w.y | w.z ) >= 0;  // Check if all weights are non-negative.
    }

    void stepX()
    {
        w[0] += dY[1];
        w[1] += dY[2];
        w[2] += dY[0];
    }

    void stepY()
    {
        w0[0] += dX[1];
        w0[1] += dX[2];
        w0[2] += dX[0];

        w = w0;
    }

    glm::vec3 barycentric() const
    {
        float u = static_cast<float>( w.x ) / area;
        float v = static_cast<float>( w.y ) / area;

        return { u, v, 1.0f - ( u + v ) };
    }
};

// Source: Claud Sonnet 4 "Create a 2D Software Rasterizer in 2D in C++"
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

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    Image*   image    = state.colorTarget;
    Viewport viewport = state.viewport;

    if ( !image )
        return;

    auto aabb = image->aabb();
    aabb.clamp( AABB::fromViewport( viewport ) );

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

void Rasterizer::drawTriangle( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2 )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    auto aabb         = image->aabb().clamped( AABB { state.viewport } );
    auto triangleAABB = AABB::fromTriangle( p0, p1, p2 );

    if ( !triangleAABB.intersect( aabb ) )
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
        int area = orient2D( p0, p1, p2 );

        if ( area < 0 )
        {
            // Swap vertices if they are in clockwise order.
            std::swap( p1, p2 );
            area = -area;
        }

        aabb.clamp( triangleAABB );

        int minX = static_cast<int>( aabb.min.x );
        int minY = static_cast<int>( aabb.min.y );
        int maxX = static_cast<int>( aabb.max.x );
        int maxY = static_cast<int>( aabb.max.y );

        glm::ivec2 p { minX, minY };

        // Edge setup.
        Edge2D e { p0, p1, p2, p };

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

void Rasterizer::drawQuad( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3 )
{
    Image* image = state.colorTarget;

    if ( !image )
        return;

    AABB dstAABB = image->aabb().clamped( AABB { state.viewport } );
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

void Rasterizer::drawAABB( math::AABB aabb )
{
    Image*   image    = state.colorTarget;
    Viewport viewport = state.viewport;

    if ( !image )
        return;

    AABB imageAABB = image->aabb();
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

void Rasterizer::drawSprite( const Sprite& sprite, int x, int y )
{
    const Image* srcImage = sprite.getImage().get();
    Image*       dstImage = state.colorTarget;

    if ( !srcImage || !dstImage )
        return;

    const Color      color        = sprite.getColor() * state.color;
    const BlendMode  blendMode    = sprite.getBlendMode();
    const AABB       viewportAABB = AABB::fromViewport( state.viewport );
    const AABB       dstAABB      = dstImage->aabb().clamped( viewportAABB );
    const glm::ivec2 size         = sprite.getSize();
    glm::ivec2       uv           = sprite.getUV();

    // Compute viewport clipping bounds.
    const int clipLeft   = std::max( static_cast<int>( dstAABB.min.x ), x );
    const int clipTop    = std::max( static_cast<int>( dstAABB.min.y ), y );
    const int clipRight  = std::min( static_cast<int>( dstAABB.max.x ), x + size.x );
    const int clipBottom = std::min( static_cast<int>( dstAABB.max.y ), y + size.y );

    // Check if the sprite is completely off-screen.
    if ( clipLeft >= clipRight || clipTop >= clipBottom )
        return;

    // Adjust sprite UV based on clipping.
    uv.x += clipLeft - x;
    uv.y += clipTop - y;

    const Color* src = srcImage->data();
    Color*       dst = dstImage->data();

    int sW = srcImage->getWidth();  // Source image width.
    int dW = dstImage->getWidth();  // Destination image width.

    for ( int y = clipTop; y < clipBottom; ++y )
    {
        for ( int x = clipLeft; x < clipRight; ++x )
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

    const Color      color        = sprite.getColor() * state.color;
    const BlendMode  blendMode    = sprite.getBlendMode();
    const AABB       viewportAABB = AABB::fromViewport( state.viewport );
    AABB             dstAABB      = dstImage->aabb().clamped( viewportAABB );
    const glm::ivec2 uv           = sprite.getUV();
    const glm::ivec2 size         = sprite.getSize();

    Vertex2D verts[4] = {
        { { 0, 0 }, { uv.x, uv.y }, color },                                      // Top-left.
        { { size.x, 0 }, { uv.x + size.y - 1, uv.y }, color },                    // Top-right.
        { { size.x, size.y }, { uv.x + size.x - 1, uv.y + size.y - 1 }, color },  // Bottom-right.
        { { 0, size.y }, { uv.x, uv.y + size.y - 1 }, color },                    // Bottom-left.
    };

    const uint32_t indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    // Transform vertices
    for ( Vertex2D& v: verts )
    {
        v.position = transform * glm::vec3 { v.position, 1.0f };
    }

    // Compute the AABB over the transformed sprite vertices.
    AABB srcAABB = AABB {
        verts[0].position, verts[1].position, verts[2].position, verts[3].position
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

    // Edge setup for the 2 triangles of the quad.
    Edge2D e[] {
        { verts[indices[0]].position, verts[indices[1]].position, verts[indices[2]].position, p },
        { verts[indices[3]].position, verts[indices[4]].position, verts[indices[5]].position, p }
    };

    for ( p.y = minY; p.y <= maxY; ++p.y )
    {
        for ( p.x = minX; p.x <= maxX; ++p.x )
        {
            for ( uint32_t i = 0; i < 2; ++i )
            {
                if ( e[i].inside() )
                {
                    const uint32_t i0 = indices[i * 3 + 0];
                    const uint32_t i1 = indices[i * 3 + 1];
                    const uint32_t i2 = indices[i * 3 + 2];

                    const glm::vec3  bc       = e[i].barycentric();
                    const glm::ivec2 texCoord = glm::round( verts[i0].texCoord * bc.x + verts[i1].texCoord * bc.y + verts[i2].texCoord * bc.z );
                    const Color      srcColor = srcImage->sample( texCoord.x, texCoord.y, AddressMode::Clamp ) * color;
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
        tileOffset[2][0] = 0.0f;

        for ( uint32_t x = 0; x < columns; ++x )
        {
            int spriteId = tileMap[x, y];
            if ( spriteId >= 0 )
            {
                drawSprite( tileMap.getSprite( x, y ), transform * tileOffset );
            }

            tileOffset[2][0] += static_cast<float>( spriteWidth );
        }

        tileOffset[2][1] += static_cast<float>( spriteHeight );
    }
}