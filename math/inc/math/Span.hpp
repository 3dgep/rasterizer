#pragma once

#include "Math.hpp"

namespace sr
{
inline namespace math
{
/// <summary>
/// A horizontal span of pixels on a single scanline, described by an inclusive
/// range of x-coordinates [x0, x1]. A span where x1 &lt; x0 is empty.
/// </summary>
struct Span
{
    /// <summary>
    /// Construct an empty span.
    /// </summary>
    Span() = default;

    /// <summary>
    /// Construct a span from an inclusive range of x-coordinates.
    /// </summary>
    /// <param name="x0">The first x-coordinate of the span (inclusive).</param>
    /// <param name="x1">The last x-coordinate of the span (inclusive).</param>
    Span( int x0, int x1 ) noexcept
    : x0 { x0 }
    , x1 { x1 }
    {}

    /// <summary>
    /// Check whether the span contains no pixels.
    /// </summary>
    /// <returns><c>true</c> if x1 &lt; x0, otherwise <c>false</c>.</returns>
    bool isEmpty() const noexcept
    {
        return x1 < x0;
    }

    /// <summary>
    /// Get the number of pixels covered by the span.
    /// </summary>
    /// <returns>The number of pixels in the span, or 0 if the span is empty.</returns>
    int count() const noexcept
    {
        return x1 < x0 ? 0 : x1 - x0 + 1;
    }

    /// <summary>
    /// Clamp this span to another span (the intersection of both spans).
    /// </summary>
    /// <param name="other">The span to clamp to.</param>
    /// <returns>
    /// The overlapping region of both spans. The result is empty if the spans do not overlap.
    /// </returns>
    Span clamped( const Span& other ) const noexcept
    {
        return { math::max( x0, other.x0 ), math::min( x1, other.x1 ) };
    }

    /// <summary>
    /// The first x-coordinate of the span (inclusive).
    /// </summary>
    int x0 = 0;

    /// <summary>
    /// The last x-coordinate of the span (inclusive).
    /// x1 &lt; x0 means the span is empty.
    /// </summary>
    int x1 = -1;
};
}  // namespace math
}  // namespace sr
