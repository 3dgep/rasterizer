#pragma once

#include "Color.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sr
{
inline namespace graphics
{
struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    Color     color;

    Vertex( const glm::vec3& position = glm::vec3 { 0 }, const glm::vec2& texCoord = glm::vec2 { 0 }, const Color& color = Color::White )
    : position { position }
    , texCoord { texCoord }
    , color { color }
    {}
};
}  // namespace graphics
}  // namespace sr