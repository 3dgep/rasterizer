#pragma once

#include <glm/vec2.hpp>

namespace sr
{
inline namespace math
{
/// <summary>
/// Represents a 2D circle defined by a center point and a radius.
/// Provides utility methods for geometric queries such as edge positions,
/// bounding box, and intersection tests with points and other circles.
/// </summary>
struct Circle
{
    /// <summary>
    /// Default constructor. Initializes the circle at the origin with zero radius.
    /// </summary>
    Circle() = default;

    /// <summary>
    /// Constructs a circle with the specified center and radius.
    /// </summary>
    /// <param name="center">The center point of the circle.</param>
    /// <param name="radius">The radius of the circle.</param>
    Circle( const glm::vec2& center, float radius )
    : center { center }
    , radius { radius }
    {}

    /// <summary>
    /// Returns a new Circle translated by the given vector.
    /// </summary>
    /// <param name="rhs">The 2D vector by which to translate the circle's center.</param>
    /// <returns>A new Circle whose center is offset by rhs and whose radius remains unchanged.</returns>
    Circle operator+( const glm::vec2& rhs ) const noexcept
    {
        return { center + rhs, radius };
    }

    /// <summary>
    /// Adds a 2D vector to the center of the circle and returns a reference to the modified circle.
    /// </summary>
    /// <param name="rhs">The 2D vector to add to the circle's center.</param>
    /// <returns>A reference to the modified Circle object.</returns>
    Circle& operator+=( const glm::vec2& rhs ) noexcept
    {
        center += rhs;
        return *this;
    }

    /// <summary>
    /// Get the diameter of the circle.
    /// </summary>
    /// <returns>The diameter of the circle.</returns>
    float diameter() const noexcept
    {
        return radius * 2.0f;
    }

    /// <summary>
    /// Get the top edge of the circle.
    /// </summary>
    /// <returns>The top edge of the circle.</returns>
    float top() const noexcept
    {
        return center.y - radius;
    }

    /// <summary>
    /// Get the left edge of the circle.
    /// </summary>
    /// <returns>The left edge of the circle.</returns>
    float left() const noexcept
    {
        return center.x - radius;
    }

    /// <summary>
    /// Get the bottom edge of the circle.
    /// </summary>
    /// <returns>The bottom edge of the circle.</returns>
    float bottom() const noexcept
    {
        return center.y + radius;
    }

    /// <summary>
    /// Get the right edge of the circle.
    /// </summary>
    /// <returns>The right edge of the circle.</returns>
    float right() const noexcept
    {
        return center.x + radius;
    }

    /// <summary>
    /// Get the minimum point (top-left corner) of the circle's bounding box.
    /// </summary>
    /// <returns>The minimum point of the bounding box.</returns>
    glm::vec2 min() const noexcept
    {
        return { center.x - radius, center.y - radius };
    }

    /// <summary>
    /// Get the maximum point (bottom-right corner) of the circle's bounding box.
    /// </summary>
    /// <returns>The maximum point of the bounding box.</returns>
    glm::vec2 max() const noexcept
    {
        return { center.x + radius, center.y + radius };
    }

    /// <summary>
    /// Circle-circle intersection test.
    /// </summary>
    /// <param name="other">The other circle to test for intersection with.</param>
    /// <returns>`true` if the circles are overlapping, `false` otherwise.</returns>
    bool intersect( const Circle& other ) const noexcept
    {
        float dx = center.x - other.center.x;
        float dy = center.y - other.center.y;
        float d  = dx * dx + dy * dy;
        float r  = radius + other.radius;

        return d < r * r;
    }

    /// <summary>
    /// Point-circle intersection test.
    /// </summary>
    /// <param name="p">The point to test for intersection with the circle.</param>
    /// <returns>`true` if the point lies inside the circle, `false` otherwise.</returns>
    bool intersect( const glm::vec2& p ) const noexcept
    {
        float dx = center.x - p.x;
        float dy = center.y - p.y;
        float d  = dx * dx + dy * dy;
        return d < radius * radius;
    }

    /// <summary>
    /// The center point of the circle.
    /// </summary>
    glm::vec2 center { 0 };

    /// <summary>
    /// The radius of the circle.
    /// </summary>
    float radius { 0 };
};
}  // namespace math
}  // namespace sr