#pragma once

#include <compare>
#include <cstdint>
#include <utility>

#include <glm/vec2.hpp>

namespace sr
{
inline namespace math
{
template<typename T>
struct Rect
{
    explicit Rect( T left = T( 0 ), T top = T( 0 ), T width = T( 0 ), T height = T( 0 ) )
    : left { left }
    , top { top }
    , width { width }
    , height { height }
    {}

    template<typename U>
    explicit Rect( U left = U( 0 ), U top = U( 0 ), U width = U( 0 ), U height = U( 0 ) )
    : left { static_cast<T>( left ) }
    , top { static_cast<T>( top ) }
    , width { static_cast<T>( width ) }
    , height { static_cast<T>( height ) }
    {}

    template<typename U>
    Rect( const Rect<U>& other )
    : left { static_cast<T>( other.left ) }
    , top { static_cast<T>( other.top ) }
    , width { static_cast<T>( other.width ) }
    , height { static_cast<T>( other.height ) }
    {}

    auto operator<=>( const Rect& ) const = default;

    T right() const noexcept
    {
        return left + width;
    }

    T bottom() const noexcept
    {
        return top + height;
    }

    glm::vec<2, T> center() const noexcept
    {
        return { left + width / T( 2 ), top + height / T( 2 ) };
    }

    glm::vec<2, T> topLeft() const noexcept
    {
        return { left, top };
    }

    glm::vec<2, T> topRight() const noexcept
    {
        return { right(), top };
    }

    glm::vec<2, T> bottomLeft() const noexcept
    {
        return { left, bottom() };
    }

    glm::vec<2, T> bottomRight() const noexcept
    {
        return { right(), bottom() };
    }

    /// <summary>
    /// The left (x) coordinate of the rectangle.
    /// </summary>
    T left = T( 0 );

    /// <summary>
    /// The top (y) coordinate of the rectangle.
    /// </summary>
    T top = T( 0 );

    /// <summary>
    /// The width of the rectangle.
    /// </summary>
    T width = T( 0 );

    /// <summary>
    /// The height of the rectangle.
    /// </summary>
    T height = T( 0 );
};

using RectI  = Rect<int32_t>;
using RectUI = Rect<uint32_t>;
using RectF  = Rect<float>;
using RectD  = Rect<double>;

}  // namespace math
}  // namespace sr