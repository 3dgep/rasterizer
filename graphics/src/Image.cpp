#include <graphics/Image.hpp>
#include <iostream>

#include <stb_image.h>
#include <stb_image_write.h>

#include <climits>  // For INT_MAX
#include <cstring>  // For std::memcpy

using namespace sr::graphics;

Image::Image()  = default;
Image::~Image() = default;

Image::Image( const Image& copy )
{
    if ( copy.m_Pixels )
    {
        resize( copy.m_Width, copy.m_Height );
        std::memcpy( m_Pixels.get(), copy.m_Pixels.get(), static_cast<size_t>( m_Width ) * m_Height * sizeof( Color ) );
    }
}

Image::Image( Image&& other ) noexcept
: widthInfo( std::exchange( other.widthInfo, {} ) )
, heightInfo( std::exchange( other.heightInfo, {} ) )
, m_AABB( std::exchange( other.m_AABB, {} ) )
, m_Width( std::exchange( other.m_Width, 0 ) )
, m_Height( std::exchange( other.m_Height, 0 ) )
, m_Pixels( std::move( other.m_Pixels ) )
{}

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
    std::memcpy( m_Pixels.get(), data, static_cast<size_t>( m_Width ) * m_Height * sizeof( Color ) );

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

    if ( copy.m_Pixels )
    {
        resize( copy.m_Width, copy.m_Height );
        std::memcpy( m_Pixels.get(), copy.m_Pixels.get(), static_cast<size_t>( copy.m_Width ) * copy.m_Height * sizeof( Color ) );
    }

    return *this;
}

Image& Image::operator=( Image&& other ) noexcept
{
    if ( this == &other )
        return *this;

    widthInfo  = std::exchange( other.widthInfo, {} );
    heightInfo = std::exchange( other.heightInfo, {} );
    m_AABB     = std::exchange( other.m_AABB, {} );
    m_Width    = std::exchange( other.m_Width, 0 );
    m_Height   = std::exchange( other.m_Height, 0 );
    m_Pixels   = std::move( other.m_Pixels );

    return *this;
}

const Color& Image::sample( int u, int v, AddressMode addressMode ) const noexcept
{
    const int w = m_Width;
    const int h = m_Height;

    switch ( addressMode )
    {
    case AddressMode::Wrap:
        // Optimized wrap using bitwise operations for power-of-2, fast mod otherwise
        if ( widthInfo.isPowerOf2 )
            u = u & widthInfo.mask;  // Extremely fast for power-of-2
        else
            u = fast_mod_signed( u, w );

        if ( heightInfo.isPowerOf2 )
            v = v & heightInfo.mask;
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

    return m_Pixels[v * m_Width + u];
}

void Image::save( const std::filesystem::path& file ) const
{
    const auto extension = file.extension();

    if ( extension == ".png" )
    {
        stbi_write_png( file.string().c_str(), m_Width, m_Height, 4, m_Pixels.get(), m_Width * static_cast<int>( sizeof( Color ) ) );
    }
    else if ( extension == ".bmp" )
    {
        stbi_write_bmp( file.string().c_str(), m_Width, m_Height, 4, m_Pixels.get() );
    }
    else if ( extension == ".tga" )
    {
        stbi_write_tga( file.string().c_str(), m_Width, m_Height, 4, m_Pixels.get() );
    }
    else if ( extension == ".jpg" )
    {
        stbi_write_jpg( file.string().c_str(), m_Width, m_Height, 4, m_Pixels.get(), 10 );
    }
    else
    {
        std::cerr << "Invalid file type: " << file << std::endl;
    }
}

void Image::clear( const Color& color ) noexcept
{
    std::fill_n( m_Pixels.get(), m_Width * m_Height, color );
}

void Image::resize( uint32_t width, uint32_t height )
{
    assert( width < INT_MAX );
    assert( height < INT_MAX );

    if ( m_Pixels && std::cmp_equal( m_Width, width ) && std::cmp_equal( m_Height, height ) )
        return;

    m_Width  = static_cast<int>( width );
    m_Height = static_cast<int>( height );

    m_Pixels = make_aligned_unique<Color[], 64>( static_cast<size_t>( width ) * height );

    widthInfo  = AddressingInfo { m_Width };
    heightInfo = AddressingInfo { m_Height };

    m_AABB = {
        { 0, 0, 0 },
        { m_Width - 1, m_Height - 1, 0 }
    };
}