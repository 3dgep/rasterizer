#include <graphics/ResourceManager.hpp>
#include <graphics/SpriteSheet.hpp>
#include <utility>

using namespace sr::graphics;

namespace
{
/// <summary>
/// Calculates the size of a single sprite in a sprite sheet.
/// </summary>
/// <param name="imageSize">The total width or height of the sprite sheet image in pixels.</param>
/// <param name="numSprites">The number of sprites along that dimension (rows or columns).</param>
/// <param name="padding">The space between adjacent sprites in pixels.</param>
/// <param name="margin">The space between the edge of the image and the first/last sprite in pixels.</param>
/// <returns>The calculated size (width or height) of a single sprite in pixels.</returns>
/// <remarks>
/// This function accounts for the margins on both edges of the image and the padding between sprites.
/// </remarks>
constexpr uint32_t getSpriteSize( uint32_t imageSize, uint32_t numSprites, uint32_t padding, uint32_t margin )
{
    return ( imageSize - 2 * margin - ( numSprites - 1 ) * padding ) / numSprites;
}
}  // namespace

SpriteSheet::SpriteSheet( const std::filesystem::path& filePath, uint32_t rows, uint32_t columns, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
: SpriteSheet( ResourceManager::loadImage( filePath ), rows, columns, padding, margin, blendMode )
{}

SpriteSheet::SpriteSheet( const std::shared_ptr<Image>& image, uint32_t rows, uint32_t columns, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
: m_Rows { rows }
, m_Columns { columns }
{
    if ( !image )
        return;

    int imageWidth  = image->getWidth();
    int imageHeight = image->getHeight();

    int spriteWidth  = getSpriteSize( imageWidth, columns, padding, margin );
    int spriteHeight = getSpriteSize( imageHeight, rows, padding, margin );

    int u = margin;
    int v = margin;

    for ( uint32_t i = 0; i < rows; ++i )
    {
        for ( uint32_t j = 0; j < columns; ++j )
        {
            const RectI spriteRect {
                u, v, spriteWidth, spriteHeight
            };
            m_Sprites.emplace_back( image, spriteRect, blendMode );

            u += spriteWidth + padding;
        }

        v += spriteHeight + padding;
    }
}

SpriteSheet::SpriteSheet( const std::filesystem::path& filePath, std::span<const math::RectI> rects, const BlendMode& blendMode )
: SpriteSheet( ResourceManager::loadImage( filePath ), rects, blendMode )
{}

SpriteSheet::SpriteSheet( const std::shared_ptr<Image>& image, std::span<const math::RectI> rects, const BlendMode& blendMode )
{
    for ( auto& rect: rects )
    {
        m_Sprites.emplace_back( image, rect, blendMode );
    }
}

const Sprite& SpriteSheet::getSprite( size_t index ) const noexcept
{
    if ( index < m_Sprites.size() )
        return m_Sprites[index];

    static const Sprite emptySprite;
    return emptySprite;
}

const Sprite& SpriteSheet::operator[]( size_t index ) const noexcept
{
    return getSprite( index );
}

const Sprite& SpriteSheet::operator[]( size_t i, size_t j ) const noexcept
{
    return getSprite( i * m_Columns + j );
}