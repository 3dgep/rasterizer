#pragma once

#include "Line.hpp"

namespace sr
{
namespace math
{
struct Sphere
{
    Sphere() = default;
    constexpr Sphere( const glm::vec3& center, float radius )
    : center { center }
    , radius { radius }
    {}

    /// <summary>
    /// Returns a new Sphere translated by the given vector.
    /// </summary>
    /// <param name="rhs">The vector by which to translate the sphere's center.</param>
    /// <returns>A Sphere whose center is offset by rhs and whose radius remains unchanged.</returns>
    Sphere operator+( const glm::vec3& rhs ) const noexcept
    {
        return { center + rhs, radius };
    }

    /// <summary>
    /// Returns a new Sphere translated by the negation of the given vector.
    /// </summary>
    /// <param name="rhs">The vector to subtract from the sphere's center.</param>
    /// <returns>A new Sphere with its center moved by subtracting rhs, and the same radius.</returns>
    Sphere operator-=( const glm::vec3& rhs ) const noexcept
    {
        return { center - rhs, radius };
    }

    /// <summary>
    /// Adds a vector to the center of the sphere and returns the updated sphere.
    /// </summary>
    /// <param name="rhs">The vector to add to the sphere's center.</param>
    /// <returns>A reference to the updated sphere after addition.</returns>
    Sphere& operator+=( const glm::vec3& rhs ) noexcept
    {
        center += rhs;
        return *this;
    }

    /// <summary>
    /// Subtracts a 3D vector from the sphere's center and returns the modified sphere.
    /// </summary>
    /// <param name="rhs">The 3D vector to subtract from the sphere's center.</param>
    /// <returns>A reference to the modified sphere after subtraction.</returns>
    Sphere& operator-=( const glm::vec3& rhs ) noexcept
    {
        center -= rhs;
        return *this;
    }

    /// <summary>
    /// Get the diameter of the sphere.
    /// </summary>
    /// <returns>The sphere diameter.</returns>
    constexpr float diameter() const noexcept
    {
        return 2.0f * radius;
    }

    /// <summary>
    /// The sphere min point.
    /// </summary>
    /// <returns>The minimum point of the sphere.</returns>
    constexpr glm::vec3 min() const noexcept
    {
        return center - glm::vec3 { radius };
    }

    /// <summary>
    /// The sphere max point.
    /// </summary>
    /// <returns>The maximum point of the sphere.</returns>
    constexpr glm::vec3 max() const noexcept
    {
        return center + glm::vec3 { radius };
    }
    /// <summary>
    /// Point-sphere intersection test.
    /// </summary>
    /// <param name="p">The point to test for intersection with the sphere.</param>
    /// <returns>`true` if the point lies inside the sphere, `false` otherwise.</returns>
    bool intersect( const glm::vec3& p ) const noexcept
    {
        float dx = center.x - p.x;
        float dy = center.y - p.y;
        float dz = center.z - p.z;
        float d  = dx * dx + dy * dy + dz * dz;

        return d < radius * radius;
    }

    /// <summary>
    /// Line/sphere intersection test.
    /// </summary>
    /// <param name="p0">The beginning of the line.</param>
    /// <param name="p1">The end of the line.</param>
    /// <returns>`true` if the line intersects with this sphere, `false` otherwise.</returns>
    bool intersect( const glm::vec3& p0, const glm::vec3& p1 ) const
    {
        return intersect( Line { p0, p1 } );
    }

    /// <summary>
    /// Line/sphere intersection test.
    /// </summary>
    /// <param name="line">The line to test for intersection.</param>
    /// <returns>`true` if the line intersects with this sphere, `false` otherwise.</returns>
    bool intersect( const Line& line ) const
    {
        return line.squareDistance( center ) < radius * radius;
    }

    /// <summary>
    /// Sphere-sphere intersection test.
    /// </summary>
    /// <param name="other">The other sphere to test for intersection with.</param>
    /// <returns>`true` if the circles are overlapping, `false` otherwise.</returns>
    bool intersect( const Sphere& other ) const noexcept
    {
        float dx = center.x - other.center.x;
        float dy = center.y - other.center.y;
        float dz = center.z - other.center.z;
        float d  = dx * dx + dy * dy + dz * dz;
        float r  = radius + other.radius;

        return d < r * r;
    }

    glm::vec3 center { 0.0f };
    float     radius { 0.0f };
};
}  // namespace math
}  // namespace sr