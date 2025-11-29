#pragma once

#include "BlendMode.hpp"
#include "Color.hpp"
#include "Enums.hpp"
#include "aligned_unique_ptr.hpp"

#include <math/AABB.hpp>

#include <filesystem>
#include <optional>
#include <utility>

namespace sr
{
inline namespace graphics
{
struct Image final
{
    /// <summary>
    /// Default construct an empty 0x0 image.
    /// </summary>
    Image();
    ~Image();

    /// <summary>
    /// Copy constructor for an image.
    /// </summary>
    /// <param name="copy">The image to copy to this one.</param>
    Image( const Image& copy );

    /// <summary>
    /// Move an image to this one.
    /// </summary>
    /// <param name="other">The image to move from.</param>
    Image( Image&& other ) noexcept;

    /// <summary>
    /// Load an image from a file.
    /// </summary>
    /// <param name="fileName">The image file to load.</param>
    explicit Image( const std::filesystem::path& fileName );

    /// <summary>
    /// Create an image with an initial width and height.
    /// </summary>
    /// <param name="width">The image width (in pixels).</param>
    /// <param name="height">The image height (in pixels).</param>
    /// <param name="color">Optional color to fill the texture with.</param>
    Image( uint32_t width, uint32_t height, std::optional<Color> color = {} );

    /// <summary>
    /// Copy another image to this one.
    /// </summary>
    /// <param name="copy">The image to copy from.</param>
    /// <returns>A reference to this image.</returns>
    Image& operator=( const Image& copy );

    /// <summary>
    /// Move another image to this one.
    /// </summary>
    /// <param name="other">The image to move from.</param>
    /// <returns>A reference to this image.</returns>
    Image& operator=( Image&& other ) noexcept;

    /// <summary>
    /// Access a pixel in the image by its linear index.
    /// </summary>
    /// <param name="i">The linear index of the pixel.</param>
    /// <returns>A constant reference to the color of the pixel at the given index.</returns>
    const Color& operator[]( size_t i ) const
    {
        assert( i < m_Width * m_Height );
        return m_Pixels[i];
    }

    /// <summary>
    /// Access a pixel in the image by its linear index.
    /// </summary>
    /// <param name="i">The linear index of the pixel.</param>
    /// <returns>A reference to the color of the pixel at the given index.</returns>
    Color& operator[]( size_t i )
    {
        assert( i < m_Width * m_Height );
        return m_Pixels[i];
    }

    const Color& operator[]( size_t x, size_t y ) const
    {
        assert( x < m_Width );
        assert( y < m_Height );

        return m_Pixels[y * m_Width + x];
    }

    Color& operator[]( size_t x, size_t y )
    {
        assert( x < m_Width );
        assert( y < m_Height );

        return m_Pixels[y * m_Width + x];
    }

    /// <summary>
    /// Access a pixel in the image by its 2D coordinates.
    /// </summary>
    /// <param name="x">The x-coordinate of the pixel.</param>
    /// <param name="y">The y-coordinate of the pixel.</param>
    /// <returns>A constant reference to the color of the pixel at the given coordinates.</returns>
    const Color& operator()( size_t x, size_t y ) const
    {
        assert( x < m_Width );
        assert( y < m_Height );

        return m_Pixels[y * m_Width + x];
    }

    /// <summary>
    /// Access a pixel in the image by its 2D coordinates.
    /// </summary>
    /// <param name="x">The x-coordinate of the pixel.</param>
    /// <param name="y">The y-coordinate of the pixel.</param>
    /// <returns>A reference to the color of the pixel at the given coordinates.</returns>
    Color& operator()( size_t x, size_t y )
    {
        assert( x < m_Width );
        assert( y < m_Height );

        return m_Pixels[y * m_Width + x];
    }

    /// <summary>
    /// Check if the image has been initialized and has a pixel buffer.
    /// </summary>
    explicit operator bool() const noexcept
    {
        return m_Pixels != nullptr;
    }

    /// <summary>
    /// Sample the image at integer coordinates.
    /// </summary>
    /// <param name="u">The U texture coordinate.</param>
    /// <param name="v">The V texture coordinate.</param>
    /// <param name="addressMode">Determines how to apply out-of-bounds texture coordinates.</param>
    /// <returns>The color of the texel at the given UV coordinates.</returns>
    const Color& sample( int u, int v, AddressMode addressMode = AddressMode::Wrap ) const noexcept;

    /// <summary>
    /// Sample the image at integer coordinates.
    /// </summary>
    /// <param name="uv">The texture coordinates.</param>
    /// <param name="addressMode">(Optional) The address mode to use during sampling. Default: AddressMode::Wrap.</param>
    /// <returns>The color of the texel at the given UV coordinates.</returns>
    const Color& sample( const glm::ivec2& uv, AddressMode addressMode = AddressMode::Wrap ) const noexcept
    {
        return sample( uv.x, uv.y, addressMode );
    }

    /// <summary>
    /// Sample the image using normalized texture coordinates (in the range from [0..1]).
    /// </summary>
    /// <param name="u">The normalized U texture coordinate.</param>
    /// <param name="v">The normalized V texture coordinate.</param>
    /// <param name="addressMode">(Optional) The addressing mode to use during sampling. Default: AddressMode::Wrap.</param>
    /// <returns>The color of the texel at the given UV texture coordinates.</returns>
    const Color& sample( float u, float v, AddressMode addressMode = AddressMode::Wrap ) const noexcept
    {
        return sample( static_cast<int>( u * static_cast<float>( m_Width - 1 ) + 0.5f ), static_cast<int>( v * static_cast<float>( m_Height - 1 ) + 0.5f ), addressMode );  // NOLINT(bugprone-incorrect-roundings)
    }

    /// <summary>
    /// Sample the image using normalized texture coordinates (in the range from [0..1]).
    /// </summary>
    /// <param name="uv">The normalized texture coordinates.</param>
    /// <param name="addressMode">The addressing mode to use during sampling.</param>
    /// <returns>The color of the texel at the given UV texture coordinates.</returns>
    const Color& sample( const glm::vec2& uv, AddressMode addressMode = AddressMode::Wrap ) const noexcept
    {
        return sample( uv.x, uv.y, addressMode );
    }

    /// <summary>
    /// Plot a single pixel to the image. Out-of-bounds coordinates are discarded.
    /// </summary>
    /// <param name="x">The x-coordinate to plot.</param>
    /// <param name="y">The y-coordinate to plot.</param>
    /// <param name="src">The source color of the pixel to plot.</param>
    /// <param name="blendMode">(Optional) The blend mode to apply. Default: No blending.</param>
    template<bool BoundsCheck = true, bool Blending = true>
    void plot( uint32_t x, uint32_t y, const Color& src, const BlendMode& blendMode = BlendMode {} ) noexcept
    {
        if constexpr ( BoundsCheck )
        {
            if ( std::cmp_greater_equal( x, m_Width ) || std::cmp_greater_equal( y, m_Height ) )
                return;
        }
        else
        {
            assert( std::cmp_less( x, m_Width ) );
            assert( std::cmp_less( y, m_Height ) );
        }

        Color& dst = m_Pixels[y * m_Width + x];
        if constexpr ( Blending )
        {
            dst = blendMode.Blend( src, dst );
        }
        else
        {
            dst = src;
        }
    }

    /// <summary>
    /// Save the image to disk.
    /// Supported file formats are:
    ///   * PNG
    ///   * BMP
    ///   * TGA
    ///   * JPEG
    /// </summary>
    /// <param name="file">The name of the file to save this image to.</param>
    void save( const std::filesystem::path& file ) const;

    /// <summary>
    /// Clear the image to a single color.
    /// </summary>
    /// <param name="color">The color to clear the image to.</param>
    void clear( const Color& color ) noexcept;

    /// <summary>
    /// Resize this image.
    /// Note: This function does nothing if the image is already the requested size.
    /// </summary>
    /// <param name="width">The new image width (in pixels).</param>
    /// <param name="height">The new image height (in pixels).</param>
    void resize( uint32_t width, uint32_t height );

    /// <summary>
    /// Get the width of the image (in pixels).
    /// </summary>
    /// <returns>The width of the image (in pixels).</returns>
    int getWidth() const noexcept
    {
        return m_Width;
    }

    /// <summary>
    /// Get the height of the image (in pixels).
    /// </summary>
    /// <returns>The height of the image (in pixels).</returns>
    int getHeight() const noexcept
    {
        return m_Height;
    }

    /// <summary>
    /// Get the distance in bytes between rows of pixels.
    /// </summary>
    /// <returns>The distance in bytes between the rows of pixels.</returns>
    int getPitch() const noexcept
    {
        return m_Width * static_cast<int>( sizeof( Color ) );
    }

    /// <summary>
    /// Get the AABB that covers the entire image.
    /// </summary>
    /// <returns>The AABB of the image.</returns>
    const AABB& getAABB() const noexcept
    {
        return m_AABB;
    }

    /// <summary>
    /// Get a pointer to the pixel buffer.
    /// </summary>
    /// <returns>A pointer to the pixel buffer of the image.</returns>
    Color* data() noexcept
    {
        return m_Pixels.get();
    }

    /// <summary>
    /// Get a read-only pointer to the pixel buffer.
    /// </summary>
    /// <returns>A read-only pointer to the pixel buffer of the image.</returns>
    const Color* data() const noexcept
    {
        return m_Pixels.get();
    }

private:
    // Precompute power-of-2 check results to avoid repeated computation
    struct AddressingInfo
    {
        constexpr AddressingInfo( int s = 0 ) noexcept
        : size( s )
        , isPowerOf2( ( s & ( s - 1 ) ) == 0 )
        , mask( isPowerOf2 ? s - 1 : -1 )
        {}

        int  size       = 0;
        bool isPowerOf2 = true;
        int  mask       = 0;  // size - 1 if power of 2, otherwise -1
    };

    AddressingInfo widthInfo {};
    AddressingInfo heightInfo {};

    /// <summary>
    /// Axis-aligned bounding box (used for clipping).
    /// </summary>
    AABB m_AABB;

    /// <summary>
    /// The image width (in pixels).
    /// </summary>
    int m_Width = 0;

    /// <summary>
    /// The image height (in pixels).
    /// </summary>
    int m_Height = 0;

    /// <summary>
    /// The pixel buffer.
    /// </summary>
    aligned_unique_ptr<Color[]> m_Pixels;
};
}  // namespace graphics
}  // namespace sr
