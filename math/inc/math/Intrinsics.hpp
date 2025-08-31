#pragma once

// Source: Claude Sonnet 4 (Aug 31, 2025) "I want to apply SIMD optimizations to this Color class. But I also want to maintain platform independence, so SIMD should only be used on the platforms that support it."

// Platform-specific includes for bit operations
#ifdef _MSC_VER
    #include <intrin.h>
#endif

// SIMD detection and includes
#if defined( __SSE2__ ) || defined( _M_X64 ) || ( defined( _M_IX86_FP ) && _M_IX86_FP >= 2 )
    #define SR_SIMD_SSE2 1
    #include <emmintrin.h>
#endif

#if defined( __SSSE3__ ) || ( defined( _MSC_VER ) && defined( __AVX__ ) )
    #define SR_SIMD_SSSE3 1
    #include <tmmintrin.h>
#endif

#if defined( __ARM_NEON ) || defined( __ARM_NEON__ )
    #define SR_SIMD_NEON 1
    #include <arm_neon.h>
#endif

namespace sr
{
inline namespace math
{
// SIMD helper functions for different platforms

#if defined( SR_SIMD_SSE2 )
// SSE2 implementation
inline __m128i simd_add_saturated_u8( __m128i a, __m128i b )
{
    return _mm_adds_epu8( a, b );
}

inline __m128i simd_sub_saturated_u8( __m128i a, __m128i b )
{
    return _mm_subs_epu8( a, b );
}

inline __m128i simd_multiply_u8( __m128i a, __m128i b )
{
    // Unpack to 16-bit, multiply, then pack back
    __m128i a_lo = _mm_unpacklo_epi8( a, _mm_setzero_si128() );
    __m128i a_hi = _mm_unpackhi_epi8( a, _mm_setzero_si128() );
    __m128i b_lo = _mm_unpacklo_epi8( b, _mm_setzero_si128() );
    __m128i b_hi = _mm_unpackhi_epi8( b, _mm_setzero_si128() );

    __m128i result_lo = _mm_mullo_epi16( a_lo, b_lo );
    __m128i result_hi = _mm_mullo_epi16( a_hi, b_hi );

    __m128i temp_lo = _mm_add_epi16( result_lo, _mm_set1_epi16( 1 ) );
    __m128i temp_hi = _mm_add_epi16( result_hi, _mm_set1_epi16( 1 ) );

    temp_lo = _mm_add_epi16( temp_lo, _mm_srli_epi16( result_lo, 8 ) );
    temp_hi = _mm_add_epi16( temp_hi, _mm_srli_epi16( result_hi, 8 ) );

    result_lo = _mm_srli_epi16( temp_lo, 8 );
    result_hi = _mm_srli_epi16( temp_hi, 8 );

    return _mm_packus_epi16( result_lo, result_hi );
}

inline __m128i simd_min_u8( __m128i a, __m128i b )
{
    return _mm_min_epu8( a, b );
}

inline __m128i simd_max_u8( __m128i a, __m128i b )
{
    return _mm_max_epu8( a, b );
}

#elif defined( SR_SIMD_NEON )
// NEON implementation
inline uint8x16_t simd_add_saturated_u8( uint8x16_t a, uint8x16_t b )
{
    return vqaddq_u8( a, b );
}

inline uint8x16_t simd_sub_saturated_u8( uint8x16_t a, uint8x16_t b )
{
    return vqsubq_u8( a, b );
}

inline uint8x16_t simd_multiply_u8( uint8x16_t a, uint8x16_t b )
{
    uint16x8_t a_lo = vmovl_u8( vget_low_u8( a ) );
    uint16x8_t a_hi = vmovl_u8( vget_high_u8( a ) );
    uint16x8_t b_lo = vmovl_u8( vget_low_u8( b ) );
    uint16x8_t b_hi = vmovl_u8( vget_high_u8( b ) );

    uint16x8_t result_lo = vmulq_u16( a_lo, b_lo );
    uint16x8_t result_hi = vmulq_u16( a_hi, b_hi );

    result_lo = vaddq_u16( vaddq_u16( result_lo, vdupq_n_u16( 1 ) ), vshrq_n_u16( result_lo, 8 ) );
    result_hi = vaddq_u16( vaddq_u16( result_hi, vdupq_n_u16( 1 ) ), vshrq_n_u16( result_hi, 8 ) );

    result_lo = vshrq_n_u16( result_lo, 8 );
    result_hi = vshrq_n_u16( result_hi, 8 );

    return vcombine_u8( vqmovn_u16( result_lo ), vqmovn_u16( result_hi ) );
}

inline uint8x16_t simd_min_u8( uint8x16_t a, uint8x16_t b )
{
    return vminq_u8( a, b );
}

inline uint8x16_t simd_max_u8( uint8x16_t a, uint8x16_t b )
{
    return vmaxq_u8( a, b );
}
#endif

// Cross-platform count trailing zeros
inline int count_trailing_zeros( int x ) noexcept
{
    if ( x == 0 )
        return 32;

#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward( &index, static_cast<unsigned long>( x ) );
    return static_cast<int>( index );
#elif defined( __GNUC__ ) || defined( __clang__ )
    return __builtin_ctz( static_cast<unsigned int>( x ) );
#else
    // Fallback implementation using lookup table
    static constexpr int trailing_zeros_table[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
    };
    return trailing_zeros_table[( ( static_cast<unsigned>( x ) & -static_cast<unsigned>( x ) ) * 0x077CB531U ) >> 27];
#endif
}

}  // namespace math
}  // namespace sr