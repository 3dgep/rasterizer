#include <graphics/Image.hpp>
#include <iostream>

#include <stb_image.h>
#include <stb_image_write.h>

using namespace sr::graphics;

Image::Image()  = default;
Image::~Image() = default;

Image::Image( const Image& copy )
{
    resize( copy.m_Width, copy.m_Height );
    std::memcpy( m_Data.get(), copy.m_Data.get(), static_cast<size_t>( m_Width ) * m_Height * sizeof( Color ) );
}

Image::Image( Image&& other ) noexcept
: m_Width { other.m_Width }
, m_Height { other.m_Height }
, m_AABB { other.m_AABB }
, m_Data { std::move( other.m_Data ) }
{
    other.m_Width  = 0;
    other.m_Height = 0;
    other.m_AABB   = {};
}

Image::Image( const std::filesystem::path& fileName )
{
    int            x, y, n;
    unsigned char* data = stbi_load( fileName.string().c_str(), &x, &y, &n, STBI_rgb_alpha );
    if ( !data )
    {
        std::cerr << "ERROR: Could not load: " << fileName.string() << std::endl;
        return;
    }

    resize( static_cast<uint32_t>( x ), static_cast<uint32_t>( y ) );

    std::memcpy( m_Data.get(), data, static_cast<std::size_t>( m_Width ) * m_Height * sizeof( Color ) );

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

    resize( copy.m_Width, copy.m_Height );
    std::memcpy( m_Data.get(), copy.m_Data.get(), static_cast<size_t>( m_Width ) * m_Height * sizeof( Color ) );

    return *this;
}

Image& Image::operator=( Image&& other ) noexcept
{
    if ( this == &other )
        return *this;

    m_Width  = other.m_Width;
    m_Height = other.m_Height;
    m_AABB   = other.m_AABB;
    m_Data   = std::move( other.m_Data );

    other.m_Width  = 0;
    other.m_Height = 0;
    other.m_AABB   = {};

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
    const int w = static_cast<int>( m_Width );
    const int h = static_cast<int>( m_Height );

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

    return m_Data[static_cast<uint64_t>( v ) * m_Width + u];
}

void Image::save( const std::filesystem::path& file ) const
{
    const auto extension = file.extension();

    if ( extension == ".png" )
    {
        stbi_write_png( file.string().c_str(), static_cast<int>( m_Width ), static_cast<int>( m_Height ), 4, m_Data.get(), static_cast<int>( m_Width * sizeof( Color ) ) );
    }
    else if ( extension == ".bmp" )
    {
        stbi_write_bmp( file.string().c_str(), static_cast<int>( m_Width ), static_cast<int>( m_Height ), 4, m_Data.get() );
    }
    else if ( extension == ".tga" )
    {
        stbi_write_tga( file.string().c_str(), static_cast<int>( m_Width ), static_cast<int>( m_Height ), 4, m_Data.get() );
    }
    else if ( extension == ".jpg" )
    {
        stbi_write_jpg( file.string().c_str(), static_cast<int>( m_Width ), static_cast<int>( m_Height ), 4, m_Data.get(), 10 );
    }
    else
    {
        std::cerr << "Invalid file type: " << file << std::endl;
    }
}

void Image::clear( const Color& color ) noexcept
{
    std::fill_n( m_Data.get(), m_Width * m_Height, color );
}

void Image::resize( uint32_t width, uint32_t height )
{
    if ( m_Width == width && m_Height == height )
        return;

    m_Width  = width;
    m_Height = height;

    m_AABB = {
        { 0, 0, 0 },
        { m_Width - 1, m_Height - 1, 0 }
    };

    m_Data = make_aligned_unique<Color[], 64>( static_cast<size_t>( width ) * height );
}