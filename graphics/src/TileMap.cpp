#include <graphics/TileMap.hpp>

#include <algorithm>  // for std::ranges::fill
#include <cassert>

using namespace sr::graphics;

TileMap::TileMap( std::shared_ptr<SpriteSheet> spriteSheet, uint32_t columns, uint32_t rows )
: m_Columns { columns }
, m_Rows { rows }
, m_SpriteSheet( std::move( spriteSheet ) )
, m_SpriteGrid( static_cast<size_t>( m_Columns ) * m_Rows, -1 )
{}

int TileMap::operator[]( size_t x, size_t y ) const noexcept
{
    if ( x < m_Columns && y < m_Rows )
        return m_SpriteGrid[y * m_Columns + x];

    return -1;
}

int& TileMap::operator[]( size_t x, size_t y ) noexcept
{
    assert( x < m_Columns );
    assert( y < m_Rows );

    return m_SpriteGrid[y * m_Columns + x];
}

void TileMap::clear()
{
    std::ranges::fill( m_SpriteGrid, -1 );
}

uint32_t TileMap::getSpriteWidth() const noexcept
{
    if ( m_SpriteSheet )
        return m_SpriteSheet->getSpriteWidth();

    return 0u;
}

uint32_t TileMap::getSpriteHeight() const noexcept
{
    if ( m_SpriteSheet )
        return m_SpriteSheet->getSpriteHeight();

    return 0u;
}

const Sprite& TileMap::getSprite( size_t x, size_t y ) const
{
    assert( x < m_Columns );
    assert( y < m_Rows );

    int spriteId = m_SpriteGrid[y * m_Columns + x];
    if ( spriteId >= 0 )
        return m_SpriteSheet->getSprite( spriteId );

    static const Sprite emptySprite;
    return emptySprite;
}

void TileMap::setSpriteGrid( std::span<const int> spriteGrid )
{
    m_SpriteGrid = std::vector( spriteGrid.begin(), spriteGrid.end() );
}
