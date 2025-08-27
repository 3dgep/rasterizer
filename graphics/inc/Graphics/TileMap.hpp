#pragma once

#include "SpriteSheet.hpp"

namespace sr
{
inline namespace graphics
{
/// <summary>
/// Represents a 2D grid of tiles, where each tile is a sprite from a sprite sheet.
/// TileMap provides a way to organize and access sprites in a grid-like structure.
/// </summary>
class TileMap
{
public:
    /// <summary>
    /// Default constructor. Creates an empty tile map with no sprites.
    /// </summary>
    TileMap() = default;

    /// <summary>
    /// Constructs a tile map with the specified sprite sheet and dimensions.
    /// </summary>
    /// <param name="spriteSheet">The sprite sheet containing the sprites to use in the tile map.</param>
    /// <param name="columns">The number of columns in the tile map.</param>
    /// <param name="rows">The number of rows in the tile map.</param>
    TileMap( std::shared_ptr<SpriteSheet> spriteSheet, uint32_t columns, uint32_t rows );

    /// <summary>
    /// Accesses a tile in the tile map by its coordinates (read-only).
    /// </summary>
    /// <param name="x">The x-coordinate (column) of the tile.</param>
    /// <param name="y">The y-coordinate (row) of the tile.</param>
    /// <returns>The sprite index at the specified coordinates.</returns>
    int operator[]( size_t x, size_t y ) const noexcept;

    /// <summary>
    /// Accesses a tile in the tile map by its coordinates (mutable).
    /// </summary>
    /// <param name="x">The x-coordinate (column) of the tile.</param>
    /// <param name="y">The y-coordinate (row) of the tile.</param>
    /// <returns>A reference to the sprite index at the specified coordinates.</returns>
    int& operator[]( size_t x, size_t y ) noexcept;

    /// <summary>
    /// Clears all tiles in the tile map.
    /// </summary>
    void clear();

    /// <summary>
    /// Gets the number of columns in the tile map.
    /// </summary>
    /// <returns>The number of columns in the tile map.</returns>
    uint32_t getColumns() const noexcept
    {
        return m_Columns;
    }

    /// <summary>
    /// Gets the number of rows in the tile map.
    /// </summary>
    /// <returns>The number of rows in the tile map.</returns>
    uint32_t getRows() const noexcept
    {
        return m_Rows;
    }

    /// <summary>
    /// Gets the sprite sheet used by this tile map.
    /// </summary>
    /// <returns>A shared pointer to the sprite sheet.</returns>
    std::shared_ptr<SpriteSheet> getSpriteSheet() const noexcept
    {
        return m_SpriteSheet;
    }

    /// <summary>
    /// Gets the grid of sprite indices.
    /// </summary>
    /// <returns>A constant reference to the vector of sprite indices.</returns>
    const std::vector<int>& getSpriteGrid() const noexcept
    {
        return m_SpriteGrid;
    }

    /// <summary>
    /// Gets the sprite at the specified coordinates.
    /// </summary>
    /// <param name="x">The x-coordinate (column) of the tile.</param>
    /// <param name="y">The y-coordinate (row) of the tile.</param>
    /// <returns>A constant reference to the sprite at the specified coordinates.</returns>
    const Sprite& getSprite( size_t x, size_t y ) const;

    /// <summary>
    /// Sets the entire sprite grid using a span of integers.
    /// </summary>
    /// <param name="spriteGrid">A span containing the sprite indices to use for the grid.</param>
    void setSpriteGrid( std::span<const int> spriteGrid );

    /// <summary>
    /// Gets the width of sprites used in this tile map.
    /// </summary>
    /// <returns>The width of each sprite in pixels.</returns>
    uint32_t getSpriteWidth() const noexcept;

    /// <summary>
    /// Gets the height of sprites used in this tile map.
    /// </summary>
    /// <returns>The height of each sprite in pixels.</returns>
    uint32_t getSpriteHeight() const noexcept;

    /// <summary>
    /// Gets the total width of the tile map in pixels.
    /// Calculated by multiplying the number of columns by the sprite width.
    /// </summary>
    /// <returns>The total width of the tile map in pixels.</returns>
    uint32_t getWidth() const noexcept
    {
        return m_Columns * getSpriteWidth();
    }

    /// <summary>
    /// Gets the total height of the tile map in pixels.
    /// Calculated by multiplying the number of rows by the sprite height.
    /// </summary>
    /// <returns>The total height of the tile map in pixels.</returns>
    uint32_t getHeight() const noexcept
    {
        return m_Rows * getSpriteHeight();
    }

private:
    uint32_t m_Columns = 0u;
    uint32_t m_Rows    = 0u;

    std::shared_ptr<SpriteSheet> m_SpriteSheet;
    std::vector<int>             m_SpriteGrid;
};
}  // namespace graphics
}  // namespace sr