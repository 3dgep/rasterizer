#pragma once

#include <cstdint>

namespace sr
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

    constexpr Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u ) noexcept;

    constexpr Color( const Color& ) noexcept   = default;
    constexpr Color( Color&& ) noexcept        = default;
    constexpr Color& operator=( const Color& ) = default;
    constexpr Color& operator=( Color&& )      = default;

    union
    {
        uint32_t rgba;

        struct
        {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
    };
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

}  // namespace sr