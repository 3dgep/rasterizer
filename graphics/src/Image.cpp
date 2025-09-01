#include <graphics/Image.hpp>
#include <iostream>

#include <stb_image.h>
#include <stb_image_write.h>

using namespace sr::graphics;

Image::Image() = default;

Image::~Image()
{
    SDL_DestroySurface( m_Surface );
}

Image::Image( const Image& copy )
{
    if ( copy.m_Surface )
    {
        resize( copy.m_Surface->w, copy.m_Surface->h );
        std::memcpy( m_Surface->pixels, copy.m_Surface->pixels, m_Surface->pitch * m_Surface->h );
    }
}

Image::Image( Image&& other ) noexcept
: m_AABB { other.m_AABB }
{
    m_Surface       = other.m_Surface;
    other.m_Surface = nullptr;
    other.m_AABB    = {};
}

Image::Image( const std::filesystem::path& fileName )
{
    int            w, h, n;
    unsigned char* data = stbi_load( fileName.string().c_str(), &w, &h, &n, STBI_rgb_alpha );
    if ( !data )
    {
        std::cerr << "ERROR: Could not load: " << fileName.string() << std::endl;
        return;
    }

    resize( static_cast<uint32_t>( w ), static_cast<uint32_t>( h ) );
    std::memcpy( m_Surface->pixels, data, m_Surface->pitch * m_Surface->h );

    stbi_image_free( data );
}

Image::Image( uint32_t width, uint32_t height, std::optional<Color> color )
{
    resize( width, height );
    if ( color )
    {
        clear( *color );
    }
}

Image& Image::operator=( const Image& copy )
{
    if ( this == &copy )
        return *this;

    if ( copy.m_Surface )
    {
        resize( copy.m_Surface->w, copy.m_Surface->h );
        std::memcpy( m_Surface->pixels, copy.m_Surface->pixels, m_Surface->pitch * m_Surface->h );
    }

    return *this;
}

Image& Image::operator=( Image&& other ) noexcept
{
    if ( this == &other )
        return *this;

    m_AABB    = other.m_AABB;
    m_Surface = other.m_Surface;

    other.m_Surface = nullptr;
    other.m_AABB    = {};

    return *this;
}

namespace
{
// Precompute power-of-2 check results to avoid repeated computation
struct AddressingInfo
{
    int  size;
    int  mask;  // size - 1 if power of 2, otherwise -1
    bool isPowerOf2;

    constexpr AddressingInfo( int s ) noexcept
    : size( s )
    , mask( ( s & ( s - 1 ) ) == 0 ? s - 1 : -1 )
    , isPowerOf2( ( s & ( s - 1 ) ) == 0 )
    {}
};
}  // namespace

const Color& Image::sample( int u, int v, AddressMode addressMode ) const noexcept
{
    assert( m_Surface != nullptr );

    const int w = m_Surface->w;
    const int h = m_Surface->h;

    // Precompute addressing info to optimize repeated operations
    static thread_local int            cached_w = -1, cached_h = -1;
    static thread_local AddressingInfo w_info { 1 }, h_info { 1 };

    if ( cached_w != w )
    {
        cached_w = w;
        w_info   = AddressingInfo { w };
    }
    if ( cached_h != h )
    {
        cached_h = h;
        h_info   = AddressingInfo { h };
    }

    switch ( addressMode )
    {
    case AddressMode::Wrap:
        // Optimized wrap using bitwise operations for power-of-2, fast mod otherwise
        if ( w_info.isPowerOf2 )
            u = u & w_info.mask;  // Extremely fast for power-of-2
        else
            u = fast_mod_signed( u, w );

        if ( h_info.isPowerOf2 )
            v = v & h_info.mask;
        else
            v = fast_mod_signed( v, h );
        break;
    case AddressMode::Mirror:
        u = mirror_coord( u, w );
        v = mirror_coord( v, h );
    break;
    case AddressMode::Clamp:
        // Branchless clamping - often faster than std::clamp due to avoided branches
        u = u < 0 ? 0 : ( u >= w ? w - 1 : u );
        v = v < 0 ? 0 : ( v >= h ? h - 1 : v );
        break;
    }

    assert( u >= 0 && u < w );
    assert( v >= 0 && v < h );

    return *reinterpret_cast<const Color*>( static_cast<unsigned char*>( m_Surface->pixels ) + v * m_Surface->pitch + u * sizeof( Color ) );
}

void Image::save( const std::filesystem::path& file ) const
{
    assert( m_Surface != nullptr );

    const auto extension = file.extension();

    if ( extension == ".png" )
    {
        stbi_write_png( file.string().c_str(), m_Surface->w, m_Surface->h, 4, m_Surface->pixels, m_Surface->pitch );
    }
    else if ( extension == ".bmp" )
    {
        stbi_write_bmp( file.string().c_str(), m_Surface->w, m_Surface->h, 4, m_Surface->pixels );
    }
    else if ( extension == ".tga" )
    {
        stbi_write_tga( file.string().c_str(), m_Surface->w, m_Surface->h, 4, m_Surface->pixels );
    }
    else if ( extension == ".jpg" )
    {
        stbi_write_jpg( file.string().c_str(), m_Surface->w, m_Surface->h, 4, m_Surface->pixels, 10 );
    }
    else
    {
        std::cerr << "Invalid file type: " << file << std::endl;
    }
}

void Image::clear( const Color& color ) noexcept
{
    assert( m_Surface != nullptr );

    std::fill_n( static_cast<Color*>( m_Surface->pixels ), m_Surface->pitch * m_Surface->h / sizeof( Color ), color );
}

void Image::resize( uint32_t width, uint32_t height )
{
    assert( width < INT_MAX );
    assert( height < INT_MAX );

    if ( m_Surface && std::cmp_equal( m_Surface->w, width ) && std::cmp_equal( m_Surface->h, height ) )
        return;

    if ( m_Surface )
        SDL_DestroySurface( m_Surface );

    m_Surface = SDL_CreateSurface( width, height, SDL_PIXELFORMAT_RGBA32 );

    m_AABB = {
        { 0, 0, 0 },
        { m_Surface->w - 1, m_Surface->h - 1, 0 }
    };
}