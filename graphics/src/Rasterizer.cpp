#include <graphics/Rasterizer.hpp>

using namespace sr::graphics;

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    Image*   image    = state.renderTarget.color[0];
    Viewport viewport = state.viewports[0];

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
        image->plot<false>( x0, y0, state.color, state.blendMode );

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