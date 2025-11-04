#pragma once

#include <math/Intrinsics.hpp>
#include <math/Math.hpp>

#include <compare>
#include <string_view>

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

    Color  operator+( const Color& rhs ) const noexcept;
    Color& operator+=( const Color& rhs ) noexcept;
    Color  operator-( const Color& rhs ) const noexcept;
    Color& operator-=( const Color& rhs ) noexcept;
    Color  operator*( const Color& rhs ) const noexcept;
    Color& operator*=( const Color& rhs ) noexcept;
    Color  operator*( float rhs ) const noexcept;
    Color& operator*=( float rhs ) noexcept;
    Color  operator/( float rhs ) const noexcept;
    Color& operator/=( float rhs ) noexcept;

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
    /// Parses an HTML color string and returns the corresponding Color object.
    /// </summary>
    /// <param name="html">A string view containing the HTML color code (e.g., "#FF0000" or "red").</param>
    /// <returns>A Color object representing the parsed color.</returns>
    static Color fromHTML( std::string_view html );

    /// <summary>
    /// Construct a color from Hue, Saturation, and Brightness values.
    /// See: <a href="https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB">HSL and HSV on Wikipedia</a>.
    /// </summary>
    /// <param name="H">Hue in the range [0, 360).</param>
    /// <param name="S">Saturation in the range [0, 1].</param>
    /// <param name="V">Value in the range [0, 1].</param>
    /// <returns>The color constructed from the given HSV values.</returns>
    static inline Color fromHSV( float H, float S = 1.0f, float V = 1.0f ) noexcept;

#pragma warning( push )
#pragma warning( disable : 4201 ) // warning C4201: nonstandard extension used: nameless struct/union
    union
    {
        uint32_t rgba;
        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };
    };
#pragma warning( pop )

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

    static const Color AliceBlue;
    static const Color AntiqueWhite;
    static const Color Aqua;
    static const Color AquaMarine;
    static const Color Azure;
    static const Color Beige;
    static const Color Bisque;
    static const Color Black;
    static const Color BlanchedAlmond;
    static const Color Blue;
    static const Color BlueViolet;
    static const Color Brown;
    static const Color BurlyWood;
    static const Color CadetBlue;
    static const Color Chartreuse;
    static const Color Chocolate;
    static const Color Coral;
    static const Color CornflowerBlue;
    static const Color Cornsilk;
    static const Color Crimson;
    static const Color Cyan;
    static const Color DarkBlue;
    static const Color DarkCyan;
    static const Color DarkGoldenRod;
    static const Color DarkGray;
    static const Color DarkGrey;
    static const Color DarkGreen;
    static const Color DarkKhaki;
    static const Color DarkMagenta;
    static const Color DarkOliveGreen;
    static const Color DarkOrange;
    static const Color DarkOrchid;
    static const Color DarkRed;
    static const Color DarkSalmon;
    static const Color DarkSeaGreen;
    static const Color DarkSlateBlue;
    static const Color DarkSlateGray;
    static const Color DarkSlateGrey;
    static const Color DarkTurquoise;
    static const Color DarkViolet;
    static const Color DeepPink;
    static const Color DeepSkyBlue;
    static const Color DimGray;
    static const Color DimGrey;
    static const Color DodgerBlue;
    static const Color FireBrick;
    static const Color FloralWhite;
    static const Color ForestGreen;
    static const Color Fuchsia;
    static const Color Gainsboro;
    static const Color GhostWhite;
    static const Color Gold;
    static const Color GoldenRod;
    static const Color Gray;
    static const Color Grey;
    static const Color Green;
    static const Color GreenYellow;
    static const Color HoneyDew;
    static const Color HotPink;
    static const Color IndianRed;
    static const Color Indigo;
    static const Color Ivory;
    static const Color Khaki;
    static const Color Lavender;
    static const Color LavenderBlush;
    static const Color LawnGreen;
    static const Color LemonChiffon;
    static const Color LightBlue;
    static const Color LightCoral;
    static const Color LightCyan;
    static const Color LightGoldenRodYellow;
    static const Color LightGray;
    static const Color LightGrey;
    static const Color LightGreen;
    static const Color LightPink;
    static const Color LightSalmon;
    static const Color LightSeaGreen;
    static const Color LightSkyBlue;
    static const Color LightSlateGray;
    static const Color LightSlateGrey;
    static const Color LightSteelBlue;
    static const Color LightYellow;
    static const Color Lime;
    static const Color LimeGreen;
    static const Color Linen;
    static const Color Magenta;
    static const Color Maroon;
    static const Color MediumAquaMarine;
    static const Color MediumBlue;
    static const Color MediumOrchid;
    static const Color MediumPurple;
    static const Color MediumSeaGreen;
    static const Color MediumSlateBlue;
    static const Color MediumSpringGreen;
    static const Color MediumTurquoise;
    static const Color MediumVioletRed;
    static const Color MidnightBlue;
    static const Color MintCream;
    static const Color MistyRose;
    static const Color Moccasin;
    static const Color NavajoWhite;
    static const Color Navy;
    static const Color OldLace;
    static const Color Olive;
    static const Color OliveDrab;
    static const Color Orange;
    static const Color OrangeRed;
    static const Color Orchid;
    static const Color PaleGoldenRod;
    static const Color PaleGreen;
    static const Color PaleTurquoise;
    static const Color PaleVioletRed;
    static const Color PapayaWhip;
    static const Color PeachPuff;
    static const Color Peru;
    static const Color Pink;
    static const Color Plum;
    static const Color PowderBlue;
    static const Color Purple;
    static const Color RebeccaPurple;
    static const Color Red;
    static const Color RosyBrown;
    static const Color RoyalBlue;
    static const Color SaddleBrown;
    static const Color Salmon;
    static const Color SandyBrown;
    static const Color SeaGreen;
    static const Color Seashell;
    static const Color Sienna;
    static const Color Silver;
    static const Color SkyBlue;
    static const Color SlateBlue;
    static const Color SlateGray;
    static const Color SlateGrey;
    static const Color Snow;
    static const Color SpringGreen;
    static const Color SteelBlue;
    static const Color Tan;
    static const Color Teal;
    static const Color Thistle;
    static const Color Tomato;
    static const Color Turquoise;
    static const Color Violet;
    static const Color Wheat;
    static const Color White;
    static const Color WhiteSmoke;
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

inline Color Color::operator+( const Color& rhs ) const noexcept
{
#if defined( SR_SIMD_SSE2 )
    // Note: static_cast is safe here - we're just transferring bit patterns
    __m128i this_vec = _mm_cvtsi32_si128( static_cast<int>( rgba ) );
    __m128i rhs_vec  = _mm_cvtsi32_si128( static_cast<int>( rhs.rgba ) );

    __m128i result = simd_add_saturated_u8( this_vec, rhs_vec );

    return Color( static_cast<uint32_t>( _mm_cvtsi128_si32( result ) ) );
#elif defined( SR_SIMD_NEON )
    uint32x2_t this_vec = vdup_n_u32( rgba );
    uint32x2_t rhs_vec  = vdup_n_u32( rhs.rgba );

    uint8x8_t this_u8 = vreinterpret_u8_u32( this_vec );
    uint8x8_t rhs_u8  = vreinterpret_u8_u32( rhs_vec );

    uint8x8_t result = vqadd_u8( this_u8, rhs_u8 );

    return Color( vget_lane_u32( vreinterpret_u32_u8( result ), 0 ) );
#else
    // Fallback scalar implementation
    return {
        static_cast<uint8_t>( math::min( r + rhs.r, 255 ) ),
        static_cast<uint8_t>( math::min( g + rhs.g, 255 ) ),
        static_cast<uint8_t>( math::min( b + rhs.b, 255 ) ),
        static_cast<uint8_t>( math::min( a + rhs.a, 255 ) ),
    };
#endif
}

inline Color& Color::operator+=( const Color& rhs ) noexcept
{
    *this = *this + rhs;
    return *this;
}

inline Color Color::operator-( const Color& rhs ) const noexcept
{
#if defined( SR_SIMD_SSE2 )
    __m128i this_vec = _mm_cvtsi32_si128( static_cast<int>( rgba ) );
    __m128i rhs_vec  = _mm_cvtsi32_si128( static_cast<int>( rhs.rgba ) );

    __m128i result = simd_sub_saturated_u8( this_vec, rhs_vec );

    return Color( static_cast<uint32_t>( _mm_cvtsi128_si32( result ) ) );
#elif defined( SR_SIMD_NEON )
    uint32x2_t this_vec = vdup_n_u32( rgba );
    uint32x2_t rhs_vec  = vdup_n_u32( rhs.rgba );

    uint8x8_t this_u8 = vreinterpret_u8_u32( this_vec );
    uint8x8_t rhs_u8  = vreinterpret_u8_u32( rhs_vec );

    uint8x8_t result = vqsub_u8( this_u8, rhs_u8 );

    return Color( vget_lane_u32( vreinterpret_u32_u8( result ), 0 ) );
#else
    // Fallback scalar implementation
    return {
        static_cast<uint8_t>( math::max( r - rhs.r, 0 ) ),
        static_cast<uint8_t>( math::max( g - rhs.g, 0 ) ),
        static_cast<uint8_t>( math::max( b - rhs.b, 0 ) ),
        static_cast<uint8_t>( math::max( a - rhs.a, 0 ) ),
    };
#endif
}

inline Color& Color::operator-=( const Color& rhs ) noexcept
{
    *this = *this - rhs;
    return *this;
}

inline Color Color::operator*( const Color& rhs ) const noexcept
{
#if defined( SR_SIMD_SSE2 )
    __m128i this_vec = _mm_cvtsi32_si128( static_cast<int>( rgba ) );
    __m128i rhs_vec  = _mm_cvtsi32_si128( static_cast<int>( rhs.rgba ) );

    __m128i result = simd_multiply_u8( this_vec, rhs_vec );

    return Color( static_cast<uint32_t>( _mm_cvtsi128_si32( result ) ) );
#elif defined( SR_SIMD_NEON )
    uint32x2_t this_vec = vdup_n_u32( rgba );
    uint32x2_t rhs_vec  = vdup_n_u32( rhs.rgba );

    uint8x8_t this_u8 = vreinterpret_u8_u32( this_vec );
    uint8x8_t rhs_u8  = vreinterpret_u8_u32( rhs_vec );

    uint16x4_t this_u16 = vget_low_u16( vmovl_u8( this_u8 ) );
    uint16x4_t rhs_u16  = vget_low_u16( vmovl_u8( rhs_u8 ) );

    uint16x4_t result = vmul_u16( this_u16, rhs_u16 );
    result            = vshr_n_u16( vadd_u16( result, vshr_n_u16( result, 8 ) ), 8 );

    uint8x8_t result_u8 = vmovn_u16( vcombine_u16( result, result ) );

    return Color( vget_lane_u32( vreinterpret_u32_u8( result_u8 ), 0 ) );
#else
    // Fallback scalar implementation
    const auto red   = static_cast<uint8_t>( r * rhs.r / 255 );
    const auto green = static_cast<uint8_t>( g * rhs.g / 255 );
    const auto blue  = static_cast<uint8_t>( b * rhs.b / 255 );
    const auto alpha = static_cast<uint8_t>( a * rhs.a / 255 );

    return { red, green, blue, alpha };
#endif
}

inline Color& Color::operator*=( const Color& rhs ) noexcept
{
    *this = *this * rhs;
    return *this;
}

inline Color Color::operator*( float rhs ) const noexcept
{
#if defined( SR_SIMD_SSE2 )
    // Load the 4 uint8_t components into a 128-bit register
    __m128i rgba_i = _mm_cvtsi32_si128( static_cast<int>( rgba ) );

    // Convert to 32-bit floats
    __m128i rgba_i32 = _mm_unpacklo_epi8( rgba_i, _mm_setzero_si128() );
    rgba_i32         = _mm_unpacklo_epi16( rgba_i32, _mm_setzero_si128() );
    __m128 rgba_f    = _mm_cvtepi32_ps( rgba_i32 );

    // Multiply by scalar
    __m128 rhs_vec  = _mm_set1_ps( rhs );
    __m128 result_f = _mm_mul_ps( rgba_f, rhs_vec );

    // Clamp to [0, 255]
    result_f = _mm_max_ps( result_f, _mm_setzero_ps() );
    result_f = _mm_min_ps( result_f, _mm_set1_ps( 255.0f ) );

    // Convert back to integers
    __m128i result_i32 = _mm_cvtps_epi32( result_f );
    __m128i result_i16 = _mm_packs_epi32( result_i32, result_i32 );
    __m128i result_i8  = _mm_packus_epi16( result_i16, result_i16 );

    return Color( static_cast<uint32_t>( _mm_cvtsi128_si32( result_i8 ) ) );

#elif defined( SR_SIMD_NEON )
    // Load into NEON registers
    uint32x2_t rgba_u32 = vdup_n_u32( rgba );
    uint8x8_t  rgba_u8  = vreinterpret_u8_u32( rgba_u32 );

    // Convert first 4 bytes to 32-bit integers, then to floats
    uint16x4_t  rgba_u16      = vget_low_u16( vmovl_u8( rgba_u8 ) );
    uint32x4_t  rgba_u32_wide = vmovl_u16( rgba_u16 );
    float32x4_t rgba_f        = vcvtq_f32_u32( rgba_u32_wide );

    // Multiply by scalar
    float32x4_t rhs_vec  = vdupq_n_f32( rhs );
    float32x4_t result_f = vmulq_f32( rgba_f, rhs_vec );

    // Clamp to [0, 255]
    result_f = vmaxq_f32( result_f, vdupq_n_f32( 0.0f ) );
    result_f = vminq_f32( result_f, vdupq_n_f32( 255.0f ) );

    // Convert back to uint8
    uint32x4_t result_u32 = vcvtq_u32_f32( result_f );
    uint16x4_t result_u16 = vqmovn_u32( result_u32 );
    uint8x8_t  result_u8  = vqmovn_u16( vcombine_u16( result_u16, result_u16 ) );

    return Color( vget_lane_u32( vreinterpret_u32_u8( result_u8 ), 0 ) );

#else
    // Fallback scalar implementation
    const auto red   = static_cast<uint8_t>( math::clamp( static_cast<float>( r ) * rhs, 0.0f, 255.0f ) );
    const auto green = static_cast<uint8_t>( math::clamp( static_cast<float>( g ) * rhs, 0.0f, 255.0f ) );
    const auto blue  = static_cast<uint8_t>( math::clamp( static_cast<float>( b ) * rhs, 0.0f, 255.0f ) );
    const auto alpha = static_cast<uint8_t>( math::clamp( static_cast<float>( a ) * rhs, 0.0f, 255.0f ) );

    return { red, green, blue, alpha };
#endif
}

inline Color& Color::operator*=( float rhs ) noexcept
{
    *this = *this * rhs;
    return *this;
}

inline Color Color::operator/( float rhs ) const noexcept
{
    assert( rhs != 0.0f );

    rhs = 1.0f / rhs;

    return operator*( rhs );
}

inline Color& Color::operator/=( float rhs ) noexcept
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

inline Color operator*( float lhs, const Color& rhs )
{
    return rhs * lhs;
}

/// <summary>
/// Returns the component-wise minimum of two colors.
/// </summary>
/// <param name="c1">The first color.</param>
/// <param name="c2">The second color.</param>
/// <returns>The component-wise minimum of the two colors.</returns>
inline Color min( const Color& c1, const Color& c2 )
{
#if defined( SR_SIMD_SSE2 )
    __m128i c1_vec = _mm_cvtsi32_si128( static_cast<int>( c1.rgba ) );
    __m128i c2_vec = _mm_cvtsi32_si128( static_cast<int>( c2.rgba ) );

    __m128i result = simd_min_u8( c1_vec, c2_vec );

    return Color( static_cast<uint32_t>( _mm_cvtsi128_si32( result ) ) );
#elif defined( SR_SIMD_NEON )
    uint32x2_t c1_vec = vdup_n_u32( c1.rgba );
    uint32x2_t c2_vec = vdup_n_u32( c2.rgba );

    uint8x8_t c1_u8 = vreinterpret_u8_u32( c1_vec );
    uint8x8_t c2_u8 = vreinterpret_u8_u32( c2_vec );

    uint8x8_t result = vmin_u8( c1_u8, c2_u8 );

    return Color( vget_lane_u32( vreinterpret_u32_u8( result ), 0 ) );
#else
    // Fallback scalar implementation
    const auto r = math::min( c1.r, c2.r );
    const auto g = math::min( c1.g, c2.g );
    const auto b = math::min( c1.b, c2.b );
    const auto a = math::min( c1.a, c2.a );

    return { r, g, b, a };
#endif
}

/// <summary>
/// Returns the component-wise maximum of two colors.
/// </summary>
/// <param name="c1">The first color.</param>
/// <param name="c2">The second color.</param>
/// <returns>The component-wise maximum of the two colors.</returns>
inline Color max( const Color& c1, const Color& c2 )
{
#if defined( SR_SIMD_SSE2 )
    __m128i c1_vec = _mm_cvtsi32_si128( static_cast<int>( c1.rgba ) );
    __m128i c2_vec = _mm_cvtsi32_si128( static_cast<int>( c2.rgba ) );

    __m128i result = simd_max_u8( c1_vec, c2_vec );

    return Color( static_cast<uint32_t>( _mm_cvtsi128_si32( result ) ) );
#elif defined( SR_SIMD_NEON )
    uint32x2_t c1_vec = vdup_n_u32( c1.rgba );
    uint32x2_t c2_vec = vdup_n_u32( c2.rgba );

    uint8x8_t c1_u8 = vreinterpret_u8_u32( c1_vec );
    uint8x8_t c2_u8 = vreinterpret_u8_u32( c2_vec );

    uint8x8_t result = vmax_u8( c1_u8, c2_u8 );

    return Color( vget_lane_u32( vreinterpret_u32_u8( result ), 0 ) );
#else
    // Fallback scalar implementation
    const auto r = math::max( c1.r, c2.r );
    const auto g = math::max( c1.g, c2.g );
    const auto b = math::max( c1.b, c2.b );
    const auto a = math::max( c1.a, c2.a );

    return { r, g, b, a };
#endif
}

inline Color interpolate( const Color& c0, const Color& c1, const Color& c2, const glm::vec3& bc )
{
    // c = c0 * bc.x
    float r = static_cast<float>( c0.r ) * bc.x;
    float g = static_cast<float>( c0.g ) * bc.x;
    float b = static_cast<float>( c0.b ) * bc.x;
    float a = static_cast<float>( c0.a ) * bc.x;

    // c += c1 * bc.y
    r = std::fma<float>( c1.r, bc.y, r );
    g = std::fma<float>( c1.g, bc.y, g );
    b = std::fma<float>( c1.b, bc.y, b );
    a = std::fma<float>( c1.a, bc.y, a );

    // c += c2 * bc.z
    r = std::fma<float>( c2.r, bc.z, r );
    g = std::fma<float>( c2.g, bc.z, g );
    b = std::fma<float>( c2.b, bc.z, b );
    a = std::fma<float>( c2.a, bc.z, a );

    return {
        static_cast<uint8_t>(r),
        static_cast<uint8_t>(g),
        static_cast<uint8_t>(b),
        static_cast<uint8_t>(a)
    };
}

}  // namespace graphics
}  // namespace sr
