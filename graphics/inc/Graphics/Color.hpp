#pragma once

#include <math/Math.hpp>

#include <SDL3/SDL_endian.h>

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
    /// <param name="rgba">The color value as a 32-bit integer (0xAABBGGRR).</param>
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

    /// <summary>
    /// Return this color with a specified alpha value.
    /// </summary>
    /// <param name="alpha">The alpha value for the new color.</param>
    /// <returns>This color with a give alpha value.</returns>
    constexpr Color withAlpha( uint8_t alpha ) const noexcept;

    /// <summary>
    /// Return this color with a specified alpha value.
    /// </summary>
    /// <param name="alpha">The alpha value for the new color.</param>
    /// <returns>This color with a give alpha value.</returns>
    constexpr Color withAlpha( float alpha ) const noexcept;

    /// <summary>
    /// Construct a color using floating-point values in the range [0 ... 1].
    /// </summary>
    /// <param name="r">The red color component.</param>
    /// <param name="g">The green color component.</param>
    /// <param name="b">The blue color component.</param>
    /// <param name="a">(optional) The alpha component. Default: 1.0f</param>
    /// <returns>The color.</returns>
    static constexpr Color fromFloats( float r, float g, float b, float a = 1.0f ) noexcept;

    /// <summary>
    /// Construct a color from a 32-bit unsigned integer (usually expressed as a hexadecimal value).
    /// The color is expressed as <code>0xAABBGGRR</code>. The alpha channel (<code>AA</code>) can be omitted from the
    /// hexadecimal value, but the default alpha will be 0.<br>
    /// Transparent red is <code>0xFF</code>.<br>
    /// Transparent green is <code>0xFF00</code>.<br>
    /// Transparent blue is <code>0xFF0000</code>.<br>
    /// Opaque black is <code>0xFF000000</code>.<br>
    ///
    /// </summary>
    /// <param name="color">The color as a 32-bit unsigned integer.</param>
    /// <returns>The RGBA color.</returns>
    static constexpr Color fromHex( uint32_t color ) noexcept;

    /// <summary>
    /// Construct a color from Hue, Saturation, and Brightness values.
    /// See: <a href="https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB">HSL and HSV on Wikipedia</a>.
    /// </summary>
    /// <param name="H">Hue in the range [0, 360).</param>
    /// <param name="S">Saturation in the range [0, 1].</param>
    /// <param name="V">Value in the range [0, 1].</param>
    /// <returns>The color constructed from the given HSV values.</returns>
    static inline Color fromHSV( float H, float S = 1.0f, float V = 1.0f ) noexcept;

    union
    {
        uint32_t rgba;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        struct
        {
            uint8_t a;
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
#else
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
#endif
    };

    /// <summary>
    /// Masks for each of the color channels.
    /// </summary>
    static constexpr uint32_t RedMask   = 0x000000FF;
    static constexpr uint32_t GreenMask = 0x0000FF00;
    static constexpr uint32_t BlueMask  = 0x00FF0000;
    static constexpr uint32_t AlphaMask = 0xFF000000;

    /// <summary>
    /// Bit shifts for each of the color channels.
    /// </summary>
    static constexpr uint32_t RedShift   = 0;
    static constexpr uint32_t GreenShift = 8;
    static constexpr uint32_t BlueShift  = 16;
    static constexpr uint32_t AlphaShift = 24;

    static const Color Aqua;
    static const Color AquaMarine;
    static const Color Black;
    static const Color Blue;
    static const Color Brown;
    static const Color CadetBlue;
    static const Color ChartReuse;
    static const Color Coral;
    static const Color CornFlowerBlue;
    static const Color Crimson;
    static const Color Cyan;
    static const Color DarkBlue;
    static const Color DarkCyan;
    static const Color DarkGoldenRod;
    static const Color DarkGreen;
    static const Color DarkKhaki;
    static const Color DarkOliveGreen;
    static const Color DarkOrange;
    static const Color DarkRed;
    static const Color DarkSalmon;
    static const Color DarkSeaGreen;
    static const Color DarkSlateGray;
    static const Color DarkTurquoise;
    static const Color DeepSkyBlue;
    static const Color DodgerBlue;
    static const Color Firebrick;
    static const Color ForestGreen;
    static const Color Gold;
    static const Color GoldenRod;
    static const Color Green;
    static const Color GreenYellow;
    static const Color IndianRed;
    static const Color Khaki;
    static const Color LawnGreen;
    static const Color LightBlue;
    static const Color LightCoral;
    static const Color LightCyan;
    static const Color LightGreen;
    static const Color LightSalmon;
    static const Color LightSeaGreen;
    static const Color LightSkyBlue;
    static const Color Lime;
    static const Color LimeGreen;
    static const Color Maroon;
    static const Color MediumAquaMarine;
    static const Color MediumBlue;
    static const Color MediumSeaGreen;
    static const Color MediumSpringGreen;
    static const Color MediumTurquoise;
    static const Color MidnightBlue;
    static const Color Navy;
    static const Color Olive;
    static const Color OliveDrab;
    static const Color Orange;
    static const Color OrangeRed;
    static const Color PaleGoldenRod;
    static const Color PaleGreen;
    static const Color PaleTurquoise;
    static const Color PowderBlue;
    static const Color Red;
    static const Color Salmon;
    static const Color SeaGreen;
    static const Color SkyBlue;
    static const Color SpringGreen;
    static const Color SteelBlue;
    static const Color Teal;
    static const Color Tomato;
    static const Color Turquoise;
    static const Color White;
    static const Color Yellow;
    static const Color YellowGreen;
};

constexpr Color::Color() noexcept
: r { 0 }
, g { 0 }
, b { 0 }
, a { 255 }
{}

constexpr Color::Color( uint32_t rgba ) noexcept
: rgba { rgba }
{}

constexpr Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept
: r { r }
, g { g }
, b { b }
, a { a }
{}

constexpr bool Color::operator==( const Color& rhs ) const noexcept
{
    return rgba == rhs.rgba;
}

constexpr auto Color::operator<=>( const Color& rhs ) const noexcept
{
    if ( const auto cmp = r <=> rhs.r; cmp != 0 )
        return cmp;
    if ( const auto cmp = g <=> rhs.g; cmp != 0 )
        return cmp;
    if ( const auto cmp = b <=> rhs.b; cmp != 0 )
        return cmp;

    return a <=> rhs.a;
}

constexpr Color Color::operator+( const Color& rhs ) const noexcept
{
    return {
        static_cast<uint8_t>( math::min( r + rhs.r, 255 ) ),
        static_cast<uint8_t>( math::min( g + rhs.g, 255 ) ),
        static_cast<uint8_t>( math::min( b + rhs.b, 255 ) ),
        static_cast<uint8_t>( math::min( a + rhs.a, 255 ) ),
    };
}

constexpr Color& Color::operator+=( const Color& rhs ) noexcept
{
    r = static_cast<uint8_t>( math::min( r + rhs.r, 255 ) );
    g = static_cast<uint8_t>( math::min( g + rhs.g, 255 ) );
    b = static_cast<uint8_t>( math::min( b + rhs.b, 255 ) );
    a = static_cast<uint8_t>( math::min( a + rhs.a, 255 ) );

    return *this;
}

constexpr Color Color::operator-( const Color& rhs ) const noexcept
{
    return {
        static_cast<uint8_t>( math::max( r - rhs.r, 0 ) ),
        static_cast<uint8_t>( math::max( g - rhs.g, 0 ) ),
        static_cast<uint8_t>( math::max( b - rhs.b, 0 ) ),
        static_cast<uint8_t>( math::max( a - rhs.a, 0 ) ),
    };
}

constexpr Color& Color::operator-=( const Color& rhs ) noexcept
{
    r = static_cast<uint8_t>( math::max( r - rhs.r, 0 ) );
    g = static_cast<uint8_t>( math::max( g - rhs.g, 0 ) );
    b = static_cast<uint8_t>( math::max( b - rhs.b, 0 ) );
    a = static_cast<uint8_t>( math::max( a - rhs.a, 0 ) );

    return *this;
}

constexpr Color Color::operator*( const Color& rhs ) const noexcept
{
    const auto red   = static_cast<uint8_t>( r * rhs.r / 255 );
    const auto green = static_cast<uint8_t>( g * rhs.g / 255 );
    const auto blue  = static_cast<uint8_t>( b * rhs.b / 255 );
    const auto alpha = static_cast<uint8_t>( a * rhs.a / 255 );

    return { red, green, blue, alpha };
}

constexpr Color& Color::operator*=( const Color& rhs ) noexcept
{
    r = static_cast<uint8_t>( r * rhs.r / 255 );
    g = static_cast<uint8_t>( g * rhs.g / 255 );
    b = static_cast<uint8_t>( b * rhs.b / 255 );
    a = static_cast<uint8_t>( a * rhs.a / 255 );

    return *this;
}

constexpr Color Color::operator*( float rhs ) const noexcept
{
    const auto red   = static_cast<uint8_t>( math::clamp( static_cast<float>( r ) * rhs, 0.0f, 255.0f ) );
    const auto green = static_cast<uint8_t>( math::clamp( static_cast<float>( g ) * rhs, 0.0f, 255.0f ) );
    const auto blue  = static_cast<uint8_t>( math::clamp( static_cast<float>( b ) * rhs, 0.0f, 255.0f ) );
    const auto alpha = static_cast<uint8_t>( math::clamp( static_cast<float>( a ) * rhs, 0.0f, 255.0f ) );

    return { red, green, blue, alpha };
}

constexpr Color& Color::operator*=( float rhs ) noexcept
{
    r = static_cast<uint8_t>( math::clamp( static_cast<float>( r ) * rhs, 0.0f, 255.0f ) );
    g = static_cast<uint8_t>( math::clamp( static_cast<float>( g ) * rhs, 0.0f, 255.0f ) );
    b = static_cast<uint8_t>( math::clamp( static_cast<float>( b ) * rhs, 0.0f, 255.0f ) );
    a = static_cast<uint8_t>( math::clamp( static_cast<float>( a ) * rhs, 0.0f, 255.0f ) );

    return *this;
}

constexpr Color Color::operator/( float rhs ) const noexcept
{
    assert( rhs != 0.0f );

    rhs = 1.0f / rhs;

    return operator*( rhs );
}

constexpr Color& Color::operator/=( float rhs ) noexcept
{
    assert( rhs != 0.0f );

    rhs = 1.0f / rhs;

    return operator*=( rhs );
}

constexpr Color Color::withAlpha( uint8_t alpha ) const noexcept
{
    return { r, g, b, alpha };
}

constexpr Color Color::withAlpha( float alpha ) const noexcept
{
    return withAlpha( static_cast<uint8_t>( alpha * 255.0f ) );
}

constexpr Color Color::fromFloats( float r, float g, float b, float a ) noexcept
{
    const auto red   = static_cast<uint8_t>( r * 255.0f );
    const auto green = static_cast<uint8_t>( g * 255.0f );
    const auto blue  = static_cast<uint8_t>( b * 255.0f );
    const auto alpha = static_cast<uint8_t>( a * 255.0f );

    return { red, green, blue, alpha };
}

constexpr Color Color::fromHex( uint32_t color ) noexcept
{
    const auto r = static_cast<uint8_t>( ( color & RedMask ) >> RedShift );
    const auto g = static_cast<uint8_t>( ( color & GreenMask ) >> GreenShift );
    const auto b = static_cast<uint8_t>( ( color & BlueMask ) >> BlueShift );
    const auto a = static_cast<uint8_t>( ( color & AlphaMask ) >> AlphaShift );

    return { r, g, b, a };
}

Color Color::fromHSV( float H, float S, float V ) noexcept
{
    // Ensure H is within [0, 360)
    H = fmodf( H, 360.0f );
    if ( H < 0 )
        H += 360.0f;

    S = math::clamp( S, 0.0f, 1.0f );
    V = math::clamp( V, 0.0f, 1.0f );

    float C  = V * S;
    float m  = V - C;
    float H2 = H / 60.0f;
    float X  = C * ( 1.0f - fabsf( fmodf( H2, 2.0f ) - 1.0f ) );

    float r = 0, g = 0, b = 0;

    switch ( static_cast<int>( H2 ) )
    {
    case 0:
        r = C;
        g = X;
        b = 0;
        break;
    case 1:
        r = X;
        g = C;
        b = 0;
        break;
    case 2:
        r = 0;
        g = C;
        b = X;
        break;
    case 3:
        r = 0;
        g = X;
        b = C;
        break;
    case 4:
        r = X;
        g = 0;
        b = C;
        break;
    case 5:
        r = C;
        g = 0;
        b = X;
        break;
    default:
        r = 0;
        g = 0;
        b = 0;
        break;
    }

    r += m;
    g += m;
    b += m;

    return fromFloats( r, g, b );
}

constexpr Color operator*( float lhs, const Color& rhs )
{
    return rhs * lhs;
}

/// <summary>
/// Returns the component-wise minimum of two colors.
/// </summary>
/// <param name="c1">The first color.</param>
/// <param name="c2">The second color.</param>
/// <returns>The component-wise minimum of the two colors.</returns>
constexpr Color min( const Color& c1, const Color& c2 )
{
    const auto r = math::min( c1.r, c2.r );
    const auto g = math::min( c1.g, c2.g );
    const auto b = math::min( c1.b, c2.b );
    const auto a = math::min( c1.a, c2.a );

    return { r, g, b, a };
}

/// <summary>
/// Returns the component-wise maximum of two colors.
/// </summary>
/// <param name="c1">The first color.</param>
/// <param name="c2">The second color.</param>
/// <returns>The component-wise maximum of the two colors.</returns>
constexpr Color max( const Color& c1, const Color& c2 )
{
    const auto r = math::max( c1.r, c2.r );
    const auto g = math::max( c1.g, c2.g );
    const auto b = math::max( c1.b, c2.b );
    const auto a = math::max( c1.a, c2.a );

    return { r, g, b, a };
}

}  // namespace graphics
}  // namespace sr