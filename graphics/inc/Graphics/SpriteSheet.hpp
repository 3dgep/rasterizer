#pragma once

#include "Sprite.hpp"

#include "math/Rect.hpp"

#include <span>
#include <vector>

namespace sr
{
inline namespace graphics
{
class SpriteSheet
{
public:
    /// <summary>
    /// Default constructor. Creates an empty sprite sheet.
    /// </summary>
    SpriteSheet()  = default;

    /// <summary>
    /// Constructs a sprite sheet by loading an image from a file and dividing it into a grid of sprites.
    /// </summary>
    /// <param name="filePath">Path to the image file containing the sprites.</param>
    /// <param name="rows">Number of sprite rows in the sheet.</param>
    /// <param name="columns">Number of sprite columns in the sheet.</param>
    /// <param name="padding">Space in pixels between adjacent sprites.</param>
    /// <param name="margin">Space in pixels around the edge of the image.</param>
    /// <param name="blendMode">Blend mode to apply when rendering sprites from this sheet.</param>
    explicit SpriteSheet( const std::filesystem::path& filePath, uint32_t rows = 1, uint32_t columns = 1, uint32_t padding = 0, uint32_t margin = 0, const BlendMode& blendMode = BlendMode {} );

    /// <summary>
    /// Constructs a sprite sheet from an existing image and divides it into a grid of sprites.
    /// </summary>
    /// <param name="image">Shared pointer to the image containing the sprites.</param>
    /// <param name="rows">Number of sprite rows in the sheet.</param>
    /// <param name="columns">Number of sprite columns in the sheet.</param>
    /// <param name="padding">Space in pixels between adjacent sprites.</param>
    /// <param name="margin">Space in pixels around the edge of the image.</param>
    /// <param name="blendMode">Blend mode to apply when rendering sprites from this sheet.</param>
    explicit SpriteSheet( const std::shared_ptr<Image>& image, uint32_t rows = 1, uint32_t columns = 1, uint32_t padding = 0, uint32_t margin = 0, const BlendMode& blendMode = BlendMode {} );

    /// <summary>
    /// Constructs a sprite sheet from an image file using explicit rectangle definitions for each sprite.
    /// </summary>
    /// <param name="filePath">Path to the image file containing the sprites.</param>
    /// <param name="rects">Collection of rectangles defining the position and size of each sprite in the image.</param>
    /// <param name="blendMode">Blend mode to apply when rendering sprites from this sheet.</param>
    SpriteSheet( const std::filesystem::path& filePath, std::span<const math::RectI> rects, const BlendMode& blendMode = BlendMode {} );


    /// <summary>
    /// Constructs a sprite sheet from an existing image using explicit rectangle definitions for each sprite.
    /// </summary>
    /// <param name="image">Shared pointer to the image containing the sprites.</param>
    /// <param name="rects">Collection of rectangles defining the position and size of each sprite in the image.</param>
    /// <param name="blendMode">Blend mode to apply when rendering sprites from this sheet.</param>
    SpriteSheet( const std::shared_ptr<Image>& image, std::span<const math::RectI> rects, const BlendMode& blendMode = BlendMode {} );

    uint32_t getRows() const noexcept
    {
        return m_Rows;
    }

    uint32_t getColumns() const noexcept
    {
        return m_Columns;
    }

    size_t getNumSprites() const noexcept
    {
        return m_Sprites.size();
    }

    int getSpriteWidth( size_t spriteId = 0 ) const noexcept
    {
        if ( spriteId < m_Sprites.size() )
            return m_Sprites[spriteId].getWidth();

        return 0;
    }

    int getSpriteHeight( size_t spriteId = 0) const noexcept
    {
        if ( spriteId < m_Sprites.size() )
            return m_Sprites[spriteId].getHeight();

        return 0;
    }

    const Sprite& getSprite( size_t index ) const noexcept;

    const Sprite& operator[](size_t index) const noexcept;

    const Sprite& operator[]( size_t i, size_t j ) const noexcept;


private:
    uint32_t m_Rows    = 0;  // The number of sprites in the x-axis of the image.
    uint32_t m_Columns = 0;  // The number of sprites in the y-axis of the image.

    std::vector<Sprite> m_Sprites;
};
}  // namespace graphics
}  // namespace sr