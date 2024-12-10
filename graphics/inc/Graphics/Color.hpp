#pragma once

#include <math/Math.hpp>

#include <compare>
#include <cstdint>

namespace sr
{
inline namespace graphics
{
struct Color
{
    /// <summary>
    /// Construct a default (black) color.
    /// </summary>
    constexpr Color() noexcept;

    /// <summary>
    /// Construct a color from a 32-bit integer.
    /// </summary>
    /// <param name="rgba">The color value as a 32-bit integer (0xRRGGBBAA).</param>
    constexpr explicit Color( uint32_t rgba ) noexcept;

    /// <summary>
    /// Construct a color from red, green, and blue color primaries.
    /// </summary>
    /// <param name="r">The red component.</param>
    /// <param name="g">The green component.</param>
    /// <param name="b">The blue component.</param>
    /// <param name="a">The alpha component.</param>
    constexpr Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u ) noexcept;

    ~Color() noexcept                          = default;
    constexpr Color( const Color& ) noexcept   = default;
    constexpr Color( Color&& ) noexcept        = default;
    constexpr Color& operator=( const Color& ) = default;
    constexpr Color& operator=( Color&& )      = default;

    constexpr auto operator<=>( const Color& rhs ) const noexcept;
    constexpr bool operator==( const Color& rhs ) const noexcept;

    constexpr Color  operator+( const Color& rhs ) const noexcept;
    constexpr Color& operator+=( const Color& rhs ) noexcept;
    constexpr Color  operator-( const Color& rhs ) const noexcept;
    constexpr Color& operator-=( const Color& rhs ) noexcept;
    constexpr Color  operator*( const Color& rhs ) const noexcept;
    constexpr Color& operator*=( const Color& rhs ) noexcept;
    constexpr Color  operator*( float rhs ) const noexcept;
    constexpr Color& operator*=( float rhs ) noexcept;
    constexpr Color  operator/( float rhs ) const noexcept;
    constexpr Color& operator/=( float rhs ) noexcept;

    union
    {
        uint32_t rgba;
        struct  // TODO: Check for endianness.
        {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
    };

    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
};

constexpr Color::Color() noexcept
: a { 255 }
, b { 0 }
, g { 0 }
, r { 0 }
{}

constexpr Color::Color( uint32_t rgba ) noexcept
: rgba { rgba }
{}

constexpr Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept
: a { a }
, b { b }
, g { g }
, r { r }
{}

constexpr bool Color::operator==( const Color& rhs ) const noexcept
{
    return a == rhs.a && b == rhs.b && g == rhs.g && r == rhs.r;
}

constexpr auto Color::operator<=>( const Color& rhs ) const noexcept
{
    if ( const auto cmp = a <=> rhs.a; cmp != 0 )
        return cmp;
    if ( const auto cmp = b <=> rhs.b; cmp != 0 )
        return cmp;
    if ( const auto cmp = g <=> rhs.g; cmp != 0 )
        return cmp;

    return r <=> rhs.r;
}

constexpr Color Color::operator+( const Color& _rhs ) const noexcept
{
    return {
        static_cast<uint8_t>( math::min<uint32_t>( r + _rhs.r, 255u ) ),
        static_cast<uint8_t>( math::min<uint32_t>( g + _rhs.g, 255u ) ),
        static_cast<uint8_t>( math::min<uint32_t>( b + _rhs.b, 255u ) ),
        static_cast<uint8_t>( math::min<uint32_t>( a + _rhs.a, 255u ) ),
    };
}

}  // namespace graphics
}  // namespace sr