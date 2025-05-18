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

Image::Image( uint32_t width, uint32_t height )
{
    resize( width, height );
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
constexpr int fast_floor( float x ) noexcept
{
    return static_cast<int>( static_cast<double>( x ) + 1073741823.0 ) - 1073741823;
}

constexpr int fast_mod( int x, int y ) noexcept
{
    return x - y * fast_floor( static_cast<float>( x ) / static_cast<float>( y ) );
}
}  // namespace

const Color& Image::sample( int u, int v, AddressMode addressMode ) const noexcept
{
    assert( m_Surface != nullptr );

    const int w = m_Surface->w;
    const int h = m_Surface->h;

    switch ( addressMode )
    {
    case AddressMode::Wrap:
    {
        u = fast_mod( u, w );
        v = fast_mod( v, h );
    }
    break;
    case AddressMode::Mirror:
    {
        u = u / w % 2 == 0 ? fast_mod( u, w ) : ( w - 1 ) - fast_mod( u, w );
        v = v / h % 2 == 0 ? fast_mod( v, h ) : ( h - 1 ) - fast_mod( v, h );
    }
    break;
    case AddressMode::Clamp:
    {
        u = math::clamp( u, 0, w - 1 );
        v = math::clamp( v, 0, h - 1 );
    }
    break;
    }

    assert( u >= 0 && u < w );
    assert( v >= 0 && v < h );

    return *reinterpret_cast<const Color*>( static_cast<unsigned char*>( m_Surface->pixels ) + v * m_Surface->pitch + u * sizeof(Color) );
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

    std::fill_n( static_cast<Color*>( m_Surface->pixels ), m_Surface->pitch * m_Surface->h / sizeof(Color), color );
}

void Image::resize( uint32_t width, uint32_t height )
{
    if ( m_Surface && m_Surface->w == width && m_Surface->h == height )
        return;

    if ( m_Surface )
        SDL_DestroySurface( m_Surface );

    m_Surface = SDL_CreateSurface( width, height, SDL_PIXELFORMAT_RGBA32 );

    m_AABB = {
        { 0, 0, 0 },
        { m_Surface->w - 1, m_Surface->h - 1, 0 }
    };
}