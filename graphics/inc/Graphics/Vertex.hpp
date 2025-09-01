#pragma once

#include "Color.hpp"

#include <glm/vec2.hpp>

namespace sr
{
inline namespace graphics
{
// Vertex with normalized texture coordinates.
struct Vertex2D
{
    glm::vec2 position { 0 };
    glm::vec2 texCoord { 0 };
    Color     color { Color::White };

    Vertex2D( const glm::vec2& position = glm::vec2 { 0 }, const glm::vec2& texCoord = glm::vec2 { 0 }, const Color& color = Color::White )
    : position { position }
    , texCoord { texCoord }
    , color { color }
    {}
};


// Vertex with integer texture coordinates.
struct Vertex2Di
{
    glm::vec2 position { 0 };
    glm::vec2 texCoord { 0 };
    Color     color { Color::White };

    Vertex2Di( const glm::vec2& position = glm::vec2 { 0 }, const glm::vec2& texCoord = glm::vec2 { 0 }, const Color& color = Color::White )
    : position { position }
    , texCoord { texCoord }
    , color { color }
    {}
};
}  // namespace graphics
}  // namespace sr