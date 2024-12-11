#pragma once

#include "BlendMode.hpp"
#include "Color.hpp"
#include "Image.hpp"
#include "RenderTarget.hpp"
#include <math/AABB.hpp>

#include <array>

namespace sr
{
inline namespace graphics
{
struct Rasterizer
{
    Rasterizer() = default;

    /// <summary>
    /// Don't forget to configure the state of the rasterizer before calling any draw functions!
    /// </summary>
    struct State
    {
        Color                   color;
        BlendMode               blendMode;
        FillMode                fillMode;
        RenderTarget            renderTarget;
        std::array<Viewport, 8> viewports;
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
};
}  // namespace graphics
}  // namespace sr