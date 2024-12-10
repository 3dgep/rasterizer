#pragma once

#include "Color.hpp"
#include "../aligned_unique_ptr.hpp"

#include <math/AABB.hpp>

#include <cstdint>
#include <filesystem>

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
    Image( uint32_t width, uint32_t height );

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
    uint32_t width() const noexcept
    {
        return m_Width;
    }

    /// <summary>
    /// Get the height of the image (in pixels).
    /// </summary>
    /// <returns>The height of the image (in pixels).</returns>
    uint32_t height() const noexcept
    {
        return m_Height;
    }

    /// <summary>
    /// Get the AABB that covers the entire image.
    /// </summary>
    /// <returns>The AABB of the image.</returns>
    const AABB& aabb() const noexcept
    {
        return m_AABB;
    }

    /// <summary>
    /// Get a pointer to the pixel buffer.
    /// </summary>
    /// <returns>A pointer to the pixel buffer of the image.</returns>
    Color* data() noexcept
    {
        return m_Data.get();
    }

    /// <summary>
    /// Get a read-only pointer to the pixel buffer.
    /// </summary>
    /// <returns>A read-only pointer to the pixel buffer of the image.</returns>
    const Color* data() const noexcept
    {
        return m_Data.get();
    }

private:
    uint32_t  m_Width  = 0;
    uint32_t  m_Height = 0;

    /// <summary>
    /// Axis-aligned bounding box (used for clipping).
    /// </summary>
    AABB m_AABB;

    aligned_unique_ptr<Color[]> m_Data;
};
}
}
