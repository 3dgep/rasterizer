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
constexpr int getSpriteSize( int imageSize, int numSprites, int padding, int margin )
{
    return ( imageSize - 2 * margin - ( numSprites - 1 ) * padding ) / numSprites;
}

/// <summary>
/// Helper function to compute the number of sprites in a specific dimension of the sprite sheet.
/// </summary>
/// <param name="imageSize">The size of the sprite sheet image.</param>
/// <param name="spriteSize">The size of a single sprite.</param>
/// <param name="padding">The padding between each sprite in the image.</param>
/// <param name="margin">The spacing around the sprite atlas.</param>
/// <returns></returns>
constexpr int getNumSprites( int imageSize, int spriteSize, int padding, int margin )
{
    return ( imageSize + padding - 2 * margin ) / ( padding + spriteSize );
}

}  // namespace

SpriteSheet::SpriteSheet( const std::filesystem::path& filePath, std::optional<int> spriteWidth, std::optional<int> spriteHeight, int padding, int margin, const BlendMode& blendMode )
: SpriteSheet( ResourceManager::loadImage( filePath ), spriteWidth, spriteHeight, padding, margin, blendMode )
{}

SpriteSheet::SpriteSheet( const std::shared_ptr<Image>& image, std::optional<int> _spriteWidth, std::optional<int> _spriteHeight, int padding, int margin, const BlendMode& blendMode )
{
    if ( !image )
        return;

    int imageWidth   = image->getWidth();
    int imageHeight  = image->getHeight();
    int spriteWidth  = _spriteWidth.value_or( imageWidth - 2 * margin );
    int spriteHeight = _spriteHeight.value_or( imageHeight - 2 * margin );

    m_Columns = ::getNumSprites( imageWidth, spriteWidth, padding, margin );
    m_Rows    = ::getNumSprites( imageHeight, spriteHeight, padding, margin );

    int u = margin;
    int v = margin;

    for ( int i = 0; i < m_Rows; ++i )
    {
        u = margin;
        for ( int j = 0; j < m_Columns; ++j )
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

void SpriteSheet::addSprite( const Sprite& sprite )
{
    m_Sprites.push_back( sprite );
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