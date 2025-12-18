#pragma once

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

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

// Fast modulo for positive numbers - much faster than division-based approach
constexpr int fast_positive_mod( int x, int divisor ) noexcept
{
    // For power-of-2 divisors, use bitwise AND (extremely fast)
    if ( ( divisor & ( divisor - 1 ) ) == 0 )
    {
        return x & ( divisor - 1 );
    }

    // For non-power-of-2, use optimized modulo
    // This avoids the expensive division in the original fast_mod
    return x >= 0 ? x % divisor : ( divisor + ( x % divisor ) ) % divisor;
}

// Optimized floor that works well for typical coordinate ranges
constexpr int fast_floor_int( float x ) noexcept
{
    const int i = static_cast<int>( x );
    return i - ( x < i );  // Branchless: subtract 1 if x was negative and truncated up
}

// Fast modulo that handles negative numbers correctly
constexpr int fast_mod_signed( int x, int divisor ) noexcept
{
    if ( x >= 0 )
    {
        return fast_positive_mod( x, divisor );
    }
    else
    {
        // Handle negative case: make it positive, then mod
        const int pos_mod = fast_positive_mod( -x, divisor );
        return pos_mod == 0 ? 0 : divisor - pos_mod;
    }
}

// Floor division that works correctly with negative numbers
constexpr int floor_div( int x, int divisor ) noexcept
{
    return ( x / divisor ) - ( x % divisor != 0 && ( ( x < 0 ) ^ ( divisor < 0 ) ) );
}

// Mirror coordinate calculation that works for both positive and negative coords
constexpr int mirror_coord( int coord, int size ) noexcept
{
    // Get the tile number (which reflection we're in)
    const int tile = floor_div( coord, size );

    // Get position within the tile [0, size-1]
    int pos = coord - tile * size;  // This is equivalent to proper floor modulo

    // If we're in an odd tile, flip the coordinate
    if ( tile & 1 )
    {
        pos = size - 1 - pos;
    }

    return pos;
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
/// Note: In screen-space, <code>a</code> is above <code>b</code> if <code>ay > by</code>. And <code>a</code> is to the left of <code>b</code> if <code>ax < bx</code>.
/// </summary>
/// <param name="ax">The x-coordinate of the start of the line.</param>
/// <param name="ay">The y-coordinate of the start of the line.</param>
/// <param name="bx">The x-coordinate of the end of the line.</param>
/// <param name="by">The y-coordinate of the end of the line.</param>
/// <returns>true if this is a top-left edge, false otherwise.</returns>
constexpr bool isTopLeft( int ax, int ay, int bx, int by ) noexcept
{
    return ( ay > by ) || ( ay == by && ax < bx );  // TODO: Check correctness of top-left rule.
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
    float nu, nv, ood;

    // Compute the areas in the plan of the largest projection.
    if ( x >= y && x >= z )
    {
        // x is largest, project to the yz plane.
        nu  = triangleArea2D( p.y, p.z, b.y, b.z, c.y, c.z );
        nv  = triangleArea2D( p.y, p.z, c.y, c.z, a.y, a.z );
        ood = 1.0f / m.x;
    }
    else if ( y >= x && y >= z )
    {
        // y is largest, project to the xz plane.
        nu  = triangleArea2D( p.x, p.z, b.x, b.z, c.x, c.z );
        nv  = triangleArea2D( p.x, p.z, c.x, c.z, a.x, a.z );
        ood = 1.0f / -m.y;
    }
    else
    {
        // Z is largest, project to the xy plane.
        nu  = triangleArea2D( p.x, p.y, b.x, b.y, c.x, c.y );
        nv  = triangleArea2D( p.x, p.y, c.x, c.y, a.x, a.y );
        ood = 1.0f / m.z;
    }

    float u = nu * ood;
    float v = nv * ood;
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

/// <summary>
/// Interpolates between three 2D vectors using barycentric coordinates.
/// </summary>
/// <param name="v0">The first 2D vector to interpolate.</param>
/// <param name="v1">The second 2D vector to interpolate.</param>
/// <param name="v2">The third 2D vector to interpolate.</param>
/// <param name="bc">The barycentric coordinates (as a 3D vector) used for interpolation.</param>
/// <returns>The interpolated 2D vector computed as v0 * bc.x + v1 * bc.y + v2 * bc.z.</returns>
inline glm::vec2 interpolate( const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2, const glm::vec3& bc )
{
    glm::vec2 i;

    // i = v0 * bc.x;
    i.x = v0.x * bc.x;
    i.y = v0.y * bc.x;

    // i += v1 * bc.y;
    i.x = std::fma( v1.x, bc.y, i.x );
    i.y = std::fma( v1.y, bc.y, i.y );

    // i += v2 * bc.z;
    i.x = std::fma( v2.x, bc.z, i.x );
    i.y = std::fma( v2.y, bc.z, i.y );

    return i;
}

/// <summary>
/// Interpolates between three 3D vectors using barycentric coordinates.
/// </summary>
/// <param name="v0">The first 3D vector (corresponding to the first vertex of the triangle).</param>
/// <param name="v1">The second 3D vector (corresponding to the second vertex of the triangle).</param>
/// <param name="v2">The third 3D vector (corresponding to the third vertex of the triangle).</param>
/// <param name="bc">The barycentric coordinates used for interpolation.</param>
/// <returns>A 3D vector representing the interpolated value based on the input vectors and barycentric coordinates.</returns>
inline glm::vec3 interpolate( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& bc )
{
    glm::vec3 i;

    // i = v0 * bc.x;
    i.x = v0.x * bc.x;
    i.y = v0.y * bc.x;
    i.z = v0.z * bc.x;

    // i += v1 * bc.y;
    i.x = std::fma( v1.x, bc.y, i.x );
    i.y = std::fma( v1.y, bc.y, i.y );
    i.z = std::fma( v1.z, bc.y, i.z );

    // i += v2 * bc.z;
    i.x = std::fma( v2.x, bc.z, i.x );
    i.y = std::fma( v2.y, bc.z, i.y );
    i.z = std::fma( v2.z, bc.z, i.z );

    return i;
}

inline glm::vec4 interpolate( const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2, const glm::vec3& bc )
{
    glm::vec4 i;

    // i = v0 * bc.x;
    i.x = v0.x * bc.x;
    i.y = v0.y * bc.x;
    i.z = v0.z * bc.x;
    i.w = v0.w * bc.x;

    // i += v1 * bc.y;
    i.x = std::fma( v1.x, bc.y, i.x );
    i.y = std::fma( v1.y, bc.y, i.y );
    i.z = std::fma( v1.z, bc.y, i.z );
    i.w = std::fma( v1.w, bc.y, i.w );

    // i += v2 * bc.z;
    i.x = std::fma( v2.x, bc.z, i.x );
    i.y = std::fma( v2.y, bc.z, i.y );
    i.z = std::fma( v2.z, bc.z, i.z );
    i.w = std::fma( v2.w, bc.z, i.w );

    return i;
}

// Source: https://grok.com (March 8th, 2025). "What is the best way to implement a character controller for a 2D platforming game?"
constexpr float sign( float v )
{
    return v > 0.0f ? 1.0f : v < 0.0f ? -1.0f : 0.0f;
}

// std::abs but constexpr
constexpr float absolute( float v )
{
    return v < 0.0f ? -v : v;
}

// Source: https://grok.com (March 8th, 2025). "What is the best way to implement a character controller for a 2D platforming game?"
// Move the current value towards the target value with a maximum delta.
constexpr float moveToward( float current, float target, float maxDelta )
{
    // Calculate the difference between current and target.
    float diff = target - current;

    // If the difference is smaller than maxDelta, just return the target.
    if ( absolute( diff ) < maxDelta )
        return target;

    // Otherwise, move by maxDelta in the direction of the target.
    return current + sign( diff ) * maxDelta;
}

// Source: https://grok.com (March 17th, 2025) "I want to implement a 2D camera for my platforming game. I want to include variables like x, and y damping and look-ahead."
// Smooth damping function.
constexpr float smoothDamp( float current, float target, float damping, float deltaTime )
{
    float speed = damping * deltaTime;
    return moveToward( current, target, speed * absolute( target - current ) );
}

}  // namespace math
}  // namespace sr
