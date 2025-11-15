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

    m_VertexBufferDirty = true;
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

const std::vector<Vertex2Di>& TileMap::getVertexBuffer() const
{
    if ( m_VertexBufferDirty )
    {
        m_VertexBuffer.clear();

        uint32_t sW = getSpriteWidth();
        uint32_t sH = getSpriteHeight();

        //  Position and UV offsets for each vertex of the quad.
        glm::ivec2 posOffset[] = {
            { 0, 0 },
            { sW, 0 },
            { sW, sH },
            { 0, sH }
        };
        glm::ivec2 uvOffset[] = {
            { 0, 0 },
            { sW - 1, 0 },
            { sW - 1, sH - 1 },
            { 0, sH - 1 }
        };

        for ( uint32_t i = 0; i < m_Rows; ++i )
        {
            for ( uint32_t j = 0; j < m_Columns; ++j )
            {
                int spriteId = m_SpriteGrid[i * m_Columns + j];
                if ( spriteId >= 0 )
                {
                    auto& sprite = m_SpriteSheet->getSprite( spriteId );
                    auto  uv     = sprite.getUV();
                    auto  c      = sprite.getColor();
                    for ( uint32_t k = 0; k < 4; ++k )
                        m_VertexBuffer.emplace_back( glm::ivec2 { j * sW, i * sH } + posOffset[k], uv + uvOffset[k], c );
                }
            }
        }

        m_VertexBufferDirty = false;
    }

    return m_VertexBuffer;
}

std::shared_ptr<Image> TileMap::getImage() const noexcept
{
    if ( m_SpriteSheet )
    {
        return m_SpriteSheet->getSprite( 0 ).getImage();
    }

    return nullptr;
}

const BlendMode& TileMap::getBlendMode() const noexcept
{
    if ( m_SpriteSheet )
    {
        return m_SpriteSheet->getSprite( 0 ).getBlendMode();
    }

    return BlendMode::Disable;
}

int TileMap::getSpriteId( size_t x, size_t y ) const
{
    assert( x < m_Columns );
    assert( y < m_Rows );

    return m_SpriteGrid[y * m_Columns + x];
}

const Sprite& TileMap::getSprite( size_t x, size_t y ) const
{
    int spriteId = getSpriteId( x, y );
    if ( spriteId >= 0 )
        return m_SpriteSheet->getSprite( spriteId );

    static const Sprite emptySprite;
    return emptySprite;
}

void TileMap::setSpriteGrid( std::span<const int> spriteGrid )
{
    m_SpriteGrid        = std::vector( spriteGrid.begin(), spriteGrid.end() );
    m_VertexBufferDirty = true;
}
