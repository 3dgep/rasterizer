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
    /// Sprites can be added to the spritesheet using the addSprite function.
    /// Note: When using this method, the rows & column counts will stay 0.
    /// </summary>
    SpriteSheet()  = default;

    /// <summary>
    /// Constructs a sprite sheet by loading an image from a file and dividing it into a grid of sprites.
    /// </summary>
    /// <param name="filePath">Path to the image file containing the sprites.</param>
    /// <param name="spriteWidth">The width (in pixels) of a sprite in the sprite sheet (Default: Image width) .</param>
    /// <param name="spriteHeight">The height (in pixels) of a sprite in the sprite sheet (Default: Image height).</param>
    /// <param name="padding">Space in pixels between adjacent sprites (Default: 0).</param>
    /// <param name="margin">Space in pixels around the edge of the image (Default: 0).</param>
    /// <param name="blendMode">Blend mode to apply when rendering sprites from this sheet (Default: Blending disabled).</param>
    explicit SpriteSheet( const std::filesystem::path& filePath, std::optional<int> spriteWidth = {}, std::optional<int> spriteHeight = {}, int padding = 0, int margin = 0, const BlendMode& blendMode = BlendMode {} );

    /// <summary>
    /// Constructs a sprite sheet from an existing image and divides it into a grid of sprites.
    /// </summary>
    /// <param name="image">Shared pointer to the image containing the sprites.</param>
    /// <param name="spriteWidth">The width (in pixels) of a sprite in the sprite sheet (Default: Image width) .</param>
    /// <param name="spriteHeight">The height (in pixels) of a sprite in the sprite sheet (Default: Image height).</param>
    /// <param name="padding">Space in pixels between adjacent sprites.</param>
    /// <param name="margin">Space in pixels around the edge of the image.</param>
    /// <param name="blendMode">Blend mode to apply when rendering sprites from this sheet.</param>
    explicit SpriteSheet( const std::shared_ptr<Image>& image, std::optional<int> spriteWidth = {}, std::optional<int> spriteHeight = {}, int padding = 0, int margin = 0, const BlendMode& blendMode = BlendMode {} );

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

    /// <summary>
    /// Returns the number of rows.
    /// </summary>
    /// <returns>The number of rows in the spritesheet.</returns>
    int getRows() const noexcept
    {
        return m_Rows;
    }

    /// <summary>
    /// Returns the number of columns.
    /// </summary>
    /// <returns>The number of columns in the spritesheet.</returns>
    int getColumns() const noexcept
    {
        return m_Columns;
    }

    /// <summary>
    /// Returns the number of sprites currently stored.
    /// </summary>
    /// <returns>The number of sprites in the spritesheet.</returns>
    size_t getNumSprites() const noexcept
    {
        return m_Sprites.size();
    }

    /// <summary>
    /// Returns the width of the specified sprite.
    /// </summary>
    /// <param name="spriteId">The index of the sprite whose width is to be retrieved. Defaults to 0.</param>
    /// <returns>The width of the sprite at the given index, or 0 if the index is out of range.</returns>
    int getSpriteWidth( size_t spriteId = 0 ) const noexcept
    {
        if ( spriteId < m_Sprites.size() )
            return m_Sprites[spriteId].getWidth();

        return 0;
    }

    /// <summary>
    /// Returns the height of the specified sprite.
    /// </summary>
    /// <param name="spriteId">The index of the sprite whose height is to be retrieved. Defaults to 0.</param>
    /// <returns>The height of the sprite at the given index, or 0 if the index is out of bounds.</returns>
    int getSpriteHeight( size_t spriteId = 0) const noexcept
    {
        if ( spriteId < m_Sprites.size() )
            return m_Sprites[spriteId].getHeight();

        return 0;
    }

    /// <summary>
    /// Adds a sprite to the current collection or scene.
    /// </summary>
    /// <param name="sprite">The Sprite object to be added.</param>
    void addSprite( const Sprite& sprite );

    /// <summary>
    /// Retrieves the sprite at the specified index.
    /// </summary>
    /// <param name="index">The index of the sprite to retrieve.</param>
    /// <returns>A reference to the Sprite at the given index.</returns>
    const Sprite& getSprite( size_t index ) const noexcept;

    /// <summary>
    /// Provides read-only access to a Sprite at the specified index.
    /// </summary>
    /// <param name="index">The position of the Sprite to access.</param>
    /// <returns>A reference to the Sprite at the given index.</returns>
    const Sprite& operator[](size_t index) const noexcept;

    /// <summary>
    /// Accesses the sprite at the specified row and column indices.
    /// </summary>
    /// <param name="i">The row index of the sprite.</param>
    /// <param name="j">The column index of the sprite.</param>
    /// <returns>A reference to the sprite at the given indices.</returns>
    const Sprite& operator[]( size_t i, size_t j ) const noexcept;

private:
    int m_Columns = 0;  // The number of sprites in the x-axis of the image.
    int m_Rows    = 0;  // The number of sprites in the y-axis of the image.

    std::vector<Sprite> m_Sprites;
};
}  // namespace graphics
}  // namespace sr