#include <graphics/Rasterizer.hpp>

using namespace sr::graphics;
using namespace sr::math;

// 2D Edge functions for triangle rasterization.
struct Edge2D
{
    glm::ivec3 dX;  // X deltas.
    glm::ivec3 dY;  // Y deltas.
    glm::ivec3 w0;  // Starting weights per row.
    glm::ivec3 w;   // Current weight.

    Edge2D( const glm::ivec2& p0, const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p )
    : dX { p1.x - p0.x, p2.x - p1.x, p0.x - p2.x }
    , dY { p0.y - p1.y, p1.y - p2.y, p2.y - p0.y }
    {
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
    Image*    image     = state.colorTarget;
    Viewport  viewport  = state.viewport;
    BlendMode blendMode = state.blendMode;

    if ( !image )
        return;

    auto aabb = image->aabb();
    aabb.clamp( AABB::fromViewport( viewport ) );

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
                    image->plot<false>( p.x, p.y, state.color, blendMode );

                e.stepX();
            }

            e.stepY();
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