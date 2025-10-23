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

// Round 2D vector to nearest integer
inline void simd_round_vec2( const float* value, float* result )
{
    // Load two floats (x, y) into lower part of XMM register
    __m128 v = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( value ) ) );

    // Round to nearest integer
    __m128 rounded = _mm_round_ps( v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC );

    // Store two floats
    _mm_store_sd( reinterpret_cast<double*>( result ), _mm_castps_pd( rounded ) );
}

// Clamp 2D vector
inline void simd_clamp_vec2( const float* value, const float* minVal, const float* maxVal, float* result )
{
    // Load two floats
    __m128 v = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( value ) ) );
    __m128 minV = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( minVal ) ) );
    __m128 maxV = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( maxVal ) ) );

    // Clamp: max(min, min(max, value))
    __m128 clamped = _mm_min_ps( maxV, _mm_max_ps( minV, v ) );

    // Store two floats
    _mm_store_sd( reinterpret_cast<double*>( result ), _mm_castps_pd( clamped ) );
}

// Round and clamp 2D vector (optimized for texture coordinate clamping)
inline void simd_round_clamp_vec2( const float* value, const float* minVal, const float* maxVal, int* result )
{
    // Load two floats (x, y) into lower part of XMM register
    __m128 v = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( value ) ) );
    __m128 minV = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( minVal ) ) );
    __m128 maxV = _mm_castpd_ps( _mm_load_sd( reinterpret_cast<const double*>( maxVal ) ) );

    // Round to nearest integer
    __m128 rounded = _mm_round_ps( v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC );

    // Clamp: max(min, min(max, rounded))
    __m128 clamped = _mm_min_ps( maxV, _mm_max_ps( minV, rounded ) );

    // Convert to integers
    __m128i resultInt = _mm_cvtps_epi32( clamped );

    // Store two integers
    _mm_storel_epi64( reinterpret_cast<__m128i*>( result ), resultInt );
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

// Round 2D vector to nearest integer
inline void simd_round_vec2( const float* value, float* result )
{
    // Load two floats
    float32x2_t v = vld1_f32( value );

    // Round to nearest integer
    float32x2_t rounded = vrndn_f32( v );

    // Store two floats
    vst1_f32( result, rounded );
}

// Clamp 2D vector
inline void simd_clamp_vec2( const float* value, const float* minVal, const float* maxVal, float* result )
{
    // Load two floats
    float32x2_t v = vld1_f32( value );
    float32x2_t minV = vld1_f32( minVal );
    float32x2_t maxV = vld1_f32( maxVal );

    // Clamp: max(min, min(max, value))
    float32x2_t clamped = vmin_f32( maxV, vmax_f32( minV, v ) );

    // Store two floats
    vst1_f32( result, clamped );
}

// Round and clamp 2D vector (optimized for texture coordinate clamping)
inline void simd_round_clamp_vec2( const float* value, const float* minVal, const float* maxVal, int* result )
{
    // Load two floats
    float32x2_t v = vld1_f32( value );
    float32x2_t minV = vld1_f32( minVal );
    float32x2_t maxV = vld1_f32( maxVal );

    // Round to nearest integer
    float32x2_t rounded = vrndn_f32( v );

    // Clamp: max(min, min(max, rounded))
    float32x2_t clamped = vmin_f32( maxV, vmax_f32( minV, rounded ) );

    // Convert to integers
    int32x2_t resultInt = vcvtn_s32_f32( clamped );

    // Store two integers
    vst1_s32( result, resultInt );
}

#else
// Scalar fallbacks for platforms without SIMD

// Round 2D vector to nearest integer
inline void simd_round_vec2( const float* value, float* result )
{
    result[0] = std::round( value[0] );
    result[1] = std::round( value[1] );
}

// Clamp 2D vector
inline void simd_clamp_vec2( const float* value, const float* minVal, const float* maxVal, float* result )
{
    result[0] = std::min( maxVal[0], std::max( minVal[0], value[0] ) );
    result[1] = std::min( maxVal[1], std::max( minVal[1], value[1] ) );
}

// Round and clamp 2D vector (optimized for texture coordinate clamping)
inline void simd_round_clamp_vec2( const float* value, const float* minVal, const float* maxVal, int* result )
{
    for ( int i = 0; i < 2; ++i )
    {
        float rounded = std::round( value[i] );
        float clamped = std::min( maxVal[i], std::max( minVal[i], rounded ) );
        result[i] = static_cast<int>( clamped );
    }
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