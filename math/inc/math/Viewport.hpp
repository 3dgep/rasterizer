#pragma once

#include <limits>

namespace sr
{
inline namespace math
{
struct Viewport
{
    explicit Viewport( float x = 0.0f, float y = 0.0f, float width = std::numeric_limits<float>::max(), float height = std::numeric_limits<float>::max(), float minDepth = 0.0f, float maxDepth = 1.0f )
    : x { x }
    , y { y }
    , width { width }
    , height { height }
    , minDepth { minDepth }
    , maxDepth { maxDepth }
    {}

    float x        = 0.0f;
    float y        = 0.0f;
    float width    = std::numeric_limits<float>::max();
    float height   = std::numeric_limits<float>::max();
    float minDepth = 0.0f;
    float maxDepth = 1.0f;
};
}  // namespace math
}  // namespace sr