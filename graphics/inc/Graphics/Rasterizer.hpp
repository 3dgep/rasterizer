#pragma once

#include "BlendMode.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "Image.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "TileMap.hpp"
#include "Vertex.hpp"

#include <math/AABB.hpp>
#include <math/Transform2D.hpp>
#include <math/Viewport.hpp>

struct TTF_TextEngine;

namespace sr
{
inline namespace graphics
{

class Rasterizer
{
public:
    /// <summary>
    /// Don't forget to configure the state of the rasterizer before calling any draw functions!
    /// </summary>
    struct State
    {
        Color     color    = Color::White;
        FillMode  fillMode = FillMode::Solid;
        BlendMode blendMode;
        Image*    colorTarget = nullptr;
        Viewport  viewport;
    } state;

    /// <summary>
    /// Draws a line from (x0, y0) to (x1, y1) using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - blendMode
    /// - colorTarget
    /// - viewport
    /// </summary>
    /// <param name="x0">The x-coordinate of the starting point.</param>
    /// <param name="y0">The y-coordinate of the starting point.</param>
    /// <param name="x1">The x-coordinate of the ending point.</param>
    /// <param name="y1">The y-coordinate of the ending point.</param>
    void drawLine( int x0, int y0, int x1, int y1 );

    /// <summary>
    /// Draws a line from (x0, y0) to (x1, y1) using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - blendMode
    /// - colorTarget
    /// - viewport
    /// </summary>
    /// <param name="x0">The x-coordinate of the starting point.</param>
    /// <param name="y0">The y-coordinate of the starting point.</param>
    /// <param name="x1">The x-coordinate of the ending point.</param>
    /// <param name="y1">The y-coordinate of the ending point.</param>
    void drawLine( float x0, float y0, float x1, float y1 )
    {
        drawLine( static_cast<int>( x0 ), static_cast<int>( y0 ), static_cast<int>( x1 ), static_cast<int>( y1 ) );
    }

    /// <summary>
    /// Draws a line from p0 to p1 using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - blendMode
    /// - colorTarget
    /// - viewport
    /// </summary>
    /// <param name="p0">The starting point of the line.</param>
    /// <param name="p1">The endpoint of the line.</param>
    void drawLine( const glm::vec2& p0, const glm::vec2& p1 )
    {
        drawLine( p0.x, p0.y, p1.x, p1.y );
    }

    /// <summary>
    /// Draws a line from p0 to p1 using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - blendMode
    /// - colorTarget
    /// - viewport
    /// </summary>
    /// <param name="p0">The starting point of the line.</param>
    /// <param name="p1">The endpoint of the line.</param>
    void drawLine( const glm::ivec2& p0, const glm::ivec2& p1 )
    {
        drawLine( p0.x, p0.y, p1.x, p1.y );
    }

    /// <summary>
    /// Draws a line current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - blendMode
    /// - colorTarget
    /// - viewport
    /// </summary>
    /// <param name="line">The line to draw.</param>
    void drawLine( const math::Line& line )
    {
        drawLine( line.p0.x, line.p0.y, line.p1.x, line.p1.y );
    }

    void drawCircle( int x, int y, int r );

    void drawCircle( const glm::ivec2& center, int radius )
    {
        drawCircle( center.x, center.y, radius );
    }

    void drawCircle( float x, float y, float r )
    {
        drawCircle( static_cast<int>( x ), static_cast<int>( y ), static_cast<int>( r ) );
    }

    void drawCircle( const Circle& circle )
    {
        drawCircle( circle.center.x, circle.center.y, circle.radius );
    }

    /// <summary>
    /// Draw a triangle using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - fillMode
    /// - blendMode (solid fill mode)
    /// - colorTarget
    /// - viewport
    /// </summary>
    /// <param name="p0">The first triangle coordinate (in screen coordinates).</param>
    /// <param name="p1">The second triangle coordinate (in screen coordinates).</param>
    /// <param name="p2">The third triangle coordinate (in screen coordinates).</param>
    void drawTriangle( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2 );

    void drawQuad( glm::ivec2 p0, glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3 );

    void drawTexturedQuad( const Vertex2D& v0, const Vertex2D& v1, const Vertex2D& v2, const Vertex2D& v3, const Image& texture, std::optional<BlendMode> blendMode = {});

    /// <summary>
    /// Draws an axis-aligned bounding box (AABB).
    /// Required state:
    /// - color
    /// - fillMode
    /// - blendMode (solid fill mode)
    /// - viewport
    /// </summary>
    /// <param name="aabb">The axis-aligned bounding box to draw.</param>
    void drawAABB( math::AABB aabb );

    void drawSprite( const Sprite& sprite, int x, int y );

    void drawSprite( const Sprite& sprite, const glm::mat3& transform );

    void drawSprite( const Sprite& sprite, const math::Transform2D& transform )
    {
        drawSprite( sprite, transform.getMatrix() );
    }

    void drawTileMap( const TileMap& tileMap, int x, int y );

    void drawTileMap( const TileMap& tileMap, const glm::mat3& transform );

    void drawTileMap( const TileMap& tileMap, const math::Transform2D& transform )
    {
        drawTileMap( tileMap, transform.getMatrix() );
    }

    /// <summary>
    /// Draws the specified text at the given coordinates using the provided font.
    /// </summary>
    /// <param name="font">The font to use for rendering the text.</param>
    /// <param name="text">The text string to be drawn.</param>
    /// <param name="x">The x-coordinate where the text will be drawn.</param>
    /// <param name="y">The y-coordinate where the text will be drawn.</param>
    void drawText( const Font& font, std::string_view text, int x, int y );

    /// <summary>
    /// Draws the specified text at the given coordinates, if provided.
    /// </summary>
    /// <param name="text">The text object to be drawn.</param>
    /// <param name="x">The x-coordinate where the text should be drawn. If not provided, the position of the Text object is used.</param>
    /// <param name="y">The y-coordinate where the text should be drawn. If not provided, the position of the Text object is used.</param>
    void drawText( const Text& text, int x = 0, int y = 0 );

private:
    /// <summary>
    /// Draws a line between two points using an algorithm optimized for lines with a shallow slope (|dy| < |dx|).
    /// </summary>
    /// <param name="x0">The x-coordinate of the starting point.</param>
    /// <param name="y0">The y-coordinate of the starting point.</param>
    /// <param name="x1">The x-coordinate of the ending point.</param>
    /// <param name="y1">The y-coordinate of the ending point.</param>
    void drawLineLow( int x0, int y0, int x1, int y1 );

    /// <summary>
    /// Draws a line between two points using an algorithm optimized for lines with a steep slope (|dy| > |dx|).
    /// </summary>
    /// <param name="x0">The x-coordinate of the starting point.</param>
    /// <param name="y0">The y-coordinate of the starting point.</param>
    /// <param name="x1">The x-coordinate of the ending point.</param>
    /// <param name="y1">The y-coordinate of the ending point.</param>
    void drawLineHigh( int x0, int y0, int x1, int y1 );
};
}  // namespace graphics
}  // namespace sr