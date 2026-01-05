#include "graphics/Rasterizer.hpp"

#include <Background.hpp>

#include <graphics/ResourceManager.hpp>

#include <algorithm>
#include <execution>
#include <ranges>

using namespace sr;

Background::Background( const std::filesystem::path& filePath, const glm::vec2& scrollDirection, float scrollSpeed )
: backgroundImage { ResourceManager::loadImage( filePath ) }
, scrollDirection { scrollDirection }
, scrollSpeed { scrollSpeed }
{}

void Background::update( const Timer& timer )
{
    textureOffset -= scrollDirection * scrollSpeed * static_cast<float>( timer.elapsedSeconds() );
}

void Background::draw( Rasterizer& rasterizer ) const
{
    auto dst = rasterizer.state.colorTarget;

    if ( !dst || !backgroundImage )
        return;

    // Used parallel tiled rendering for the background.
    const size_t tileSize    = 32;  // 32x32 tiles.
    const size_t tileRows    = std::ceil( dst->getHeight() / tileSize );
    const size_t tileColumns = std::ceil( dst->getWidth() / tileSize );

    // Compute position offsets for each tile.
    const glm::vec2 posOffset[] = {
        { 0, 0 },
        { tileSize, 0 },
        { tileSize, tileSize },
        { 0, tileSize }
    };

    // Build the tile vertices over the screen
    std::vector<Vertex2D> vertices;
    vertices.reserve( tileRows * tileColumns * 4 );  // 4 vertices per tile.

    for ( size_t i = 0; i < tileRows; ++i )
    {
        for ( size_t j = 0; j < tileColumns; ++j )
        {
            for ( size_t k = 0; k < 4; ++k )
            {
                const glm::vec2 p = glm::vec2 { j * tileSize, i * tileSize } + posOffset[k];
                vertices.emplace_back( p, p + textureOffset);
            }
        }
    }

    auto range = std::views::iota( 0, static_cast<int>( tileRows * tileColumns ) );

    // Now render the quads in parallel.
    std::for_each( std::execution::par_unseq, range.begin(), range.end(), [this, &rasterizer, &vertices]( int i ) {
        const auto& v0 = vertices[i * 4 + 0];
        const auto& v1 = vertices[i * 4 + 1];
        const auto& v2 = vertices[i * 4 + 2];
        const auto& v3 = vertices[i * 4 + 3];

        rasterizer.drawQuad( v0, v1, v2, v3, *backgroundImage );
    } );
}
