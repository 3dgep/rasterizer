#include <graphics/Rasterizer.hpp>

using namespace sr::graphics;

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

    for ( int x = x0; x <= x1; ++x)
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

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int D = 2 * dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; ++y)
    {
        image->plot<false>(x, y, state.color, blendMode);

        if (D > 0)
        {
            x += xi;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    Image*    image     = state.colorTarget;
    Viewport  viewport  = state.viewport;

    if (image == nullptr)
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

    assert( image != nullptr );

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

        // Bias the edge equations based on the top-left rule.
        int bias0 = isTopLeft( p1, p2 ) ? 0 : -1;
        int bias1 = isTopLeft( p2, p0 ) ? 0 : -1;
        int bias2 = isTopLeft( p0, p1 ) ? 0 : -1;

        aabb.clamp( triangleAABB );

        int minX = static_cast<int>( aabb.min.x );
        int minY = static_cast<int>( aabb.min.y );
        int maxX = static_cast<int>( aabb.max.x );
        int maxY = static_cast<int>( aabb.max.y );

        glm::ivec2 p { minX, minY };

        int w0_start = orient2D( p1, p2, p ) + bias0;
        int w1_start = orient2D( p2, p0, p ) + bias1;
        int w2_start = orient2D( p0, p1, p ) + bias2;

        // Compute X and Y edge deltas.
        int dX0 = p1.x - p0.x;
        int dX1 = p2.x - p1.x;
        int dX2 = p0.x - p2.x;
        int dY0 = p0.y - p1.y;
        int dY1 = p1.y - p2.y;
        int dY2 = p2.y - p0.y;

        for ( p.y = minY; p.y <= maxY; p.y++ )
        {
            int w0 = w0_start;
            int w1 = w1_start;
            int w2 = w2_start;

            for ( p.x = minX; p.x <= maxX; p.x++ )
            {

                if ( ( w0 | w1 | w2 ) >= 0 )
                    image->plot<false>( p.x, p.y, state.color, blendMode );

                w0 += dY1;
                w1 += dY2;
                w2 += dY0;
            }

            w0_start += dX1;
            w1_start += dX2;
            w2_start += dX0;
        }
    }
    break;
    }
}