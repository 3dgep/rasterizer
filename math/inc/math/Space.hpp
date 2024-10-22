#pragma once

namespace sr
{
inline namespace math
{
enum class Space
{
    Local,  ///< Translation is applied in the camera's local space.
    World,  ///< Translation is applied in world coordinates.
};
}   // namespace math
}  // namespace sr