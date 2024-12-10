#include <iostream>
#include <graphics/Image.hpp>

#include <stb_image.h>

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

    // Convert ARGB
    unsigned char* p = data;
    for ( size_t i = 0; i < x * y; ++i )
    {
        unsigned char c = p[0];
        p[0]            = p[2];
        p[2]            = c;
        p += 4;
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