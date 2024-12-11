#pragma once

#include <array>

namespace sr
{
inline namespace graphics
{
struct Image;

struct RenderTarget
{
    // A maximum of 8 color targets.
    std::array<Image*, 8> color = {};
};
}  // namespace graphics
}  // namespace sr
