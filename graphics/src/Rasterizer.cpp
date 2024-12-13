#include <graphics/Rasterizer.hpp>

using namespace sr::graphics;

void Rasterizer::drawLine( int x0, int y0, int x1, int y1 )
{
    Image*   image    = state.colorTargets[0];
    Viewport viewport = state.viewports[0];
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

void Rasterizer::drawTriangle( const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2 )
{
    Image*   image    = state.colorTargets[0];
    Viewport viewport = state.viewports[0];
    BlendMode blendMode = state.blendModes[0];

    assert( image != nullptr );

    auto aabb = image->aabb();
    aabb.clamp( AABB::fromViewport( viewport ) );

    auto triangleAABB = AABB::fromTriangle( p0, p1, p2 );

    // Check to see if any part of the triangle intersects the AABB of the screen (after the viewport is applied)
    if ( !aabb.intersect( triangleAABB ) )
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
        // Sort the vertices by y-coordinate.
        glm::vec2 v0 = p0;
        glm::vec2 v1 = p1;
        glm::vec2 v2 = p2;
        if ( v0.y > v1.y )
            std::swap( v0, v1 );
        if ( v0.y > v2.y )
            std::swap( v0, v2 );
        if ( v1.y > v2.y )
            std::swap( v1, v2 );
        const float totalHeight = v2.y - v0.y;
        for ( int i = 0; i < totalHeight; i++ )
        {
            const bool  secondHalf    = i > v1.y - v0.y || v1.y == v0.y;
            const int   segmentHeight = secondHalf ? v2.y - v1.y : v1.y - v0.y;
            const float alpha         = static_cast<float>( i ) / totalHeight;
            const float beta          = static_cast<float>( i - ( secondHalf ? v1.y - v0.y : 0 ) ) / segmentHeight;
            glm::vec2   A             = v0 + ( v2 - v0 ) * alpha;
            glm::vec2   B             = secondHalf ? v1 + ( v2 - v1 ) * beta : v0 + ( v1 - v0 ) * beta;
            if ( A.x > B.x )
                std::swap( A, B );
            for ( int j = A.x; j <= B.x; j++ )
                image->plot<false>( j, v0.y + i, state.color, blendMode );
        }
    }
    break;
    }
}