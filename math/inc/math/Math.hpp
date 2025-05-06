#pragma once

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sr
{
inline namespace math
{
/// <summary>
/// Returns either a, or b, whichever is less.
/// A faster implementation of min than what is provided by STL.
/// </summary>
/// <typeparam name="T">The value type.</typeparam>
/// <param name="a">The first value.</param>
/// <param name="b">The second value.</param>
/// <returns>Either a, or b, whichever is less.</returns>
template<typename T>
constexpr T min( T a, T b ) noexcept
{
    return a < b ? a : b;
}

/// <summary>
/// Returns either a or b, whichever is greater.
/// This should be a faster implementation than what is provided by STL.
/// </summary>
/// <typeparam name="T">The value type.</typeparam>
/// <param name="a">The first value.</param>
/// <param name="b">The second value.</param>
/// <returns>Either a, or b, whichever is greater.</returns>
template<typename T>
constexpr T max( T a, T b ) noexcept
{
    return a > b ? a : b;
}

/// <summary>
/// Clamp the value `v` between `minVal` and `maxVal`.
/// This should be a faster implementation of clamp than what is provided by STL.
/// </summary>
/// <typeparam name="T">The value type to clamp</typeparam>
/// <param name="v">The value to clamp.</param>
/// <param name="minVal">The minimum allowed value.</param>
/// <param name="maxVal">The maximum allowed value.</param>
/// <returns>The result of clamping `v` between `min`, and `max`.</returns>
template<typename T>
constexpr T clamp( T v, T minVal, T maxVal ) noexcept
{
    return min( max( v, minVal ), maxVal );
}

/// <summary>
/// Compute the area of a triangle in 2D.
/// Source: "Real-Time Collision Detection" (Chapter 3.4), Christer Ericson, 2005, Elsevier Inc.
/// </summary>
/// <param name="ax">The x-coordinate of the first point.</param>
/// <param name="ay">The y-coordinate of the first point.</param>
/// <param name="bx">The x-coordinate of the second point.</param>
/// <param name="by">The y-coordinate of the second point.</param>
/// <param name="cx">The x-coordinate of the third point.</param>
/// <param name="cy">The y-coordinate of the third point.</param>
/// <returns></returns>
constexpr float triangleArea2D( float ax, float ay, float bx, float by, float cx, float cy ) noexcept
{
    return ( ax - bx ) * ( by - cy ) - ( bx - cx ) * ( ay - by );
}

/// <summary>
/// Determines the orientation of the 2D triangle (ax, ay), (bx, by), (cx, cy).
/// The function returns:
/// - a positive value if the triangle is oriented counter-clockwise,
/// - a negative value if the triangle is oriented clockwise,
/// - zero if the points are collinear.
/// Source: https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
/// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-practical-implementation.html
/// </summary>
/// <param name="ax">The x-coordinate of the first point.</param>
/// <param name="ay">The y-coordinate of the first point.</param>
/// <param name="bx">The x-coordinate of the second point.</param>
/// <param name="by">The y-coordinate of the second point.</param>
/// <param name="cx">The x-coordinate of the third point.</param>
/// <param name="cy">The y-coordinate of the third point.</param>
/// <returns>An integer representing the orientation of the triangle.</returns>
constexpr int orient2D( int ax, int ay, int bx, int by, int cx, int cy ) noexcept
{
    return ( bx - ax ) * ( cy - ay ) - ( by - ay ) * ( cx - ax );
}

/// <summary>
/// Determines the orientation of the 2D triangle (a, b, c).<br>
/// The function returns:
/// - a positive value if the triangle is oriented counter-clockwise,
/// - a negative value if the triangle is oriented clockwise,
/// - zero if the points are collinear.
/// Source: https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
/// Source: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-practical-implementation.html
/// </summary>
/// <param name="a">The first triangle point.</param>
/// <param name="b">The second triangle point.</param>
/// <param name="c">The third triangle point.</param>
/// <returns>The (signed) area of the triangle.</returns>
constexpr int orient2D( const glm::ivec2& a, const glm::ivec2& b, const glm::ivec2& c ) noexcept
{
    return orient2D( a.x, a.y, b.x, b.y, c.x, c.y );
}

/// <summary>
/// Check to see if a line (in screen space) from <code>(ax, ay)</code> to <code>(bx, by)</code> is a top-left edge.
/// A line is a top-left edge if the first point is above and to the left of the second point.
/// Note: In screen-space, <code>a</code> is above <code>b</code> if <code>ay < by</code>. And <code>a</code> is to the left of <code>b</code> if <code>ax < bx</code>.
/// </summary>
/// <param name="ax">The x-coordinate of the start of the line.</param>
/// <param name="ay">The y-coordinate of the start of the line.</param>
/// <param name="bx">The x-coordinate of the end of the line.</param>
/// <param name="by">The y-coordinate of the end of the line.</param>
/// <returns>true if this is a top-left edge, false otherwise.</returns>
constexpr bool isTopLeft( int ax, int ay, int bx, int by ) noexcept
{
    return ( ay < by ) || ( ay == by && ax < bx );
}

/// <summary>
/// Check to see if a line (in screen space) from a to b is a top-left edge.
/// A line is a top-left edge if the first point is above and to the left of the second point.
/// Note: In screen-space, a is above b if a.y < b.y. And a is to the left of b if a.x < b.x.
/// </summary>
/// <param name="a">The start of the line.</param>
/// <param name="b">The end of the line.</param>
/// <returns>true if this is a top-left edge, false otherwise.</returns>
constexpr bool isTopLeft( const glm::ivec2& a, const glm::ivec2& b ) noexcept
{
    return isTopLeft( a.x, a.y, b.x, b.y );
}

/// <summary>
/// Compute the area of a triangle in 2D.
/// Source: "Real-Time Collision Detection" (Chapter 3.4), Christer Ericson, 2005, Elsevier Inc.
/// </summary>
/// <param name="a">The first triangle point.</param>
/// <param name="b">The second triangle point.</param>
/// <param name="c">The third triangle point.</param>
/// <returns>The area of the 2D triangle.</returns>
constexpr float triangleArea2D( const glm::vec2& a, const glm::vec2& b, const glm::vec2& c ) noexcept
{
    return triangleArea2D( a.x, a.y, b.x, b.y, c.x, c.y );
}

/// <summary>
/// Compute the barycentric coordinates for a point p
/// with respect to a triangle (a, b, c).
/// Source: "Real-Time Collision Detection" (Chapter 3.4), Christer Ericson, 2005, Elsevier Inc.
/// </summary>
/// <param name="a">The first triangle coordinate.</param>
/// <param name="b">The second triangle coordinate.</param>
/// <param name="c">The third triangle coordinate.</param>
/// <param name="p">The point to compute the barycentric coordinates for.</param>
/// <returns>The barycentric coordinates for the point with respect to a triangle.</returns>
inline glm::vec3 barycentric( const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p ) noexcept
{
    const glm::vec3 m = glm::cross( b - a, c - a );

    const float x = std::abs( m.x );
    const float y = std::abs( m.y );
    const float z = std::abs( m.z );

    // Nominators and one-over-denominator for u, and v ratios.
    float nu, nv, odd;

    // Compute the areas in the plan of the largest projection.
    if ( x >= y && x >= z )
    {
        // x is largest, project to the yz plane.
        nu  = triangleArea2D( p.y, p.z, b.y, b.z, c.y, c.z );
        nv  = triangleArea2D( p.y, p.z, c.y, c.z, a.y, a.z );
        odd = 1.0f / m.x;
    }
    else if ( y >= x && y >= z )
    {
        // y is largest, project to the xz plane.
        nu  = triangleArea2D( p.x, p.z, b.x, b.z, c.x, c.z );
        nv  = triangleArea2D( p.x, p.z, c.x, c.z, a.x, a.z );
        odd = 1.0f / -m.y;
    }
    else
    {
        // Z is largest, project to the xy plane.
        nu  = triangleArea2D( p.x, p.y, b.x, b.y, c.x, c.y );
        nv  = triangleArea2D( p.x, p.y, c.x, c.y, a.x, a.y );
        odd = 1.0f / m.z;
    }

    float u = nu * odd;
    float v = nv * odd;
    float w = 1.0f - u - v;

    return { u, v, w };
}

/// <summary>
/// Compute the barycentric coordinates of a 2D triangle.
/// </summary>
/// <param name="a">The first triangle vertex.</param>
/// <param name="b">The second triangle vertex.</param>
/// <param name="c">The third triangle vertex.</param>
/// <param name="p">The point to compute the barycentric coordinates for.</param>
/// <returns>The barycentric coordinates for triangle (`a`, `b`, `c`) at point `p`.</returns>
inline glm::vec3 barycentric( const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p )
{
    glm::vec3 u = cross( glm::vec3 { c.x - a.x, b.x - a.x, a.x - p.x }, glm::vec3 { c.y - a.y, b.y - a.y, a.y - p.y } );

    if ( glm::abs( u.z ) < 1.0f )
        return { -1, -1, -1 };

    return { 1.0f - ( u.x + u.y ) / u.z, u.y / u.z, u.x / u.z };
}

/// <summary>
/// Check to see if the barycentric coordinates are inside or outside the triangle.
/// </summary>
/// <param name="barycentric">The barycentric coordinates to check.</param>
/// <returns>true if the barycentric coordinates are inside the triangle, false otherwise.</returns>
inline bool barycentricInside( const glm::vec3& barycentric )
{
    return barycentric.y >= 0.0f && barycentric.z >= 0.0f && ( barycentric.y + barycentric.z ) <= 1.0f;
}

/// <summary>
/// Test to see if a point p is contained in a triangle (a, b, c).
/// </summary>
/// <param name="p">The point to test for containment.</param>
/// <param name="a">The first triangle point.</param>
/// <param name="b">The second triangle point.</param>
/// <param name="c">The third triangle point.</param>
/// <returns>true if the point is contained in the triangle, false otherwise.</returns>
inline bool pointInsideTriangle( const glm::vec3& p, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c ) noexcept
{
    const glm::vec3 bc = barycentric( a, b, c, p );
    return barycentricInside( bc );
}

/// <summary>
/// Test to see if a point p is contained in a triangle (a, b, c).
/// </summary>
/// <param name="p">The point to test for containment.</param>
/// <param name="a">The first triangle point.</param>
/// <param name="b">The second triangle point.</param>
/// <param name="c">The third triangle point.</param>
/// <returns>true if the point is contained in the triangle, false otherwise.</returns>
inline bool pointInsideTriangle( const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c ) noexcept
{
    const glm::vec3 bc = barycentric( a, b, c, p );
    return barycentricInside( bc );
}
}  // namespace math
}  // namespace sr
