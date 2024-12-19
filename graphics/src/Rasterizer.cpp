#include <graphics/Rasterizer.hpp>

using namespace sr::graphics;

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    Image*    image     = state.colorTargets[0];
    Viewport  viewport  = state.viewports[0];
    BlendMode blendMode = state.blendModes[0];

    assert( image != nullptr );

    auto aabb = image->aabb();
    aabb.clamp( AABB::fromViewport( viewport ) );

    if ( !aabb.clip( x0, y0, x1, y1 ) )
        return;

    const int dx = std::abs( x1 - x0 );
    const int dy = -std::abs( y1 - y0 );
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while ( true )
    {
        image->plot<false>( x0, y0, state.color, blendMode );

        const int e2 = err * 2;

        if ( e2 >= dy )
        {
            if ( x0 == x1 )
                break;

            err += dy;
            x0 += sx;
        }
        if ( e2 <= dx )
        {
            if ( y0 == y1 )
                break;

            err += dx;
            y0 += sy;
        }
    }
}

void Rasterizer::drawTriangle( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2 )
{
    Image*    image     = state.colorTargets[0];
    Viewport  viewport  = state.viewports[0];
    BlendMode blendMode = state.blendModes[0];

    assert( image != nullptr );

    auto aabb = image->aabb();
    aabb.clamp( AABB::fromViewport( viewport ) );
    aabb.clamp( AABB::fromTriangle( p0, p1, p2 ) );

    // if ( !aabb.isValid() )
    //     return;

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

        // Compute X and Y edge deltas.
        int dX0 = p1.x - p0.x;
        int dX1 = p2.x - p1.x;
        int dX2 = p0.x - p2.x;
        int dY0 = p0.y - p1.y;
        int dY1 = p1.y - p2.y;
        int dY2 = p2.y - p0.y;

        // Bias the edge equations based on the top-left rule.
        int bias0 = isTopLeft( p1, p2 ) ? 0 : -1;
        int bias1 = isTopLeft( p2, p0 ) ? 0 : -1;
        int bias2 = isTopLeft( p0, p1 ) ? 0 : -1;

        glm::ivec2 p = aabb.min;

        int w0_start = orient2D( p1, p2, p ) + bias0;
        int w1_start = orient2D( p2, p0, p ) + bias1;
        int w2_start = orient2D( p0, p1, p ) + bias2;

        Color* c = image->data();
        int    w = image->width();

        for ( p.y = aabb.min.y; p.y <= aabb.max.y; p.y++ )
        {
            int w0 = w0_start;
            int w1 = w1_start;
            int w2 = w2_start;

            for ( p.x = aabb.min.x; p.x <= aabb.max.x; p.x++ )
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