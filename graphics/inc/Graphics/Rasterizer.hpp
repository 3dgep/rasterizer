#pragma once

#include "BlendMode.hpp"
#include "Color.hpp"
#include "Image.hpp"
#include "RenderTarget.hpp"

#include <glm/vec2.hpp>
#include <math/AABB.hpp>

#include <array>

namespace sr
{
inline namespace graphics
{

constexpr size_t MAX_COLOR_TARGETS = 8;

struct Rasterizer
{
    Rasterizer() = default;

    /// <summary>
    /// Don't forget to configure the state of the rasterizer before calling any draw functions!
    /// </summary>
    struct State
    {
        Color                                    color;
        FillMode                                 fillMode = FillMode::Solid;
        std::array<BlendMode, MAX_COLOR_TARGETS> blendModes;
        std::array<Image*, MAX_COLOR_TARGETS>    colorTargets = {};
        std::array<Viewport, MAX_COLOR_TARGETS>  viewports;
    } state;

    /// <summary>
    /// Draws a line from (x0, y0) to (x1, y1) using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - blendMode
    /// - Image bound to renderTarget.color[0]
    /// - Viewport at viewports[0]
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
    /// - Image bound to renderTarget.color[0]
    /// - Viewport at viewports[0]
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
    /// - Image bound to renderTarget.color[0]
    /// - Viewport at viewports[0]
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
    /// - Image bound to renderTarget.color[0]
    /// - Viewport at viewports[0]
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
    /// - Image bound to renderTarget.color[0]
    /// - Viewport at viewports[0]
    /// </summary>
    /// <param name="line">The line to draw.</param>
    void drawLine( const math::Line& line )
    {
        drawLine( line.p0.x, line.p0.y, line.p1.x, line.p1.y );
    }

    /// <summary>
    /// Draw a triangle using the current rasterizer state.<br>
    /// Required state:
    /// - color
    /// - fillMode
    /// - blendMode (if fill mode is Solid)
    /// - Image bound to renderTarget.color[0]
    /// - Viewport at viewports[0].
    /// </summary>
    /// <param name="p0"></param>
    /// <param name="p1"></param>
    /// <param name="p2"></param>
    void drawTriangle( const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2 );
};
}  // namespace graphics
}  // namespace sr