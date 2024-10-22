#pragma once

#include "bitmask_operators.hpp"

namespace sr
{
inline namespace math
{
// Outcodes for computing line intersections with the AABB.
// Source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
enum class OutCode
{
    Inside = 0b000000,  // Inside AABB.
    Left   = 0b000001,  // -x
    Right  = 0b000010,  // +x
    Bottom = 0b000100,  // -y
    Top    = 0b001000,  // +y
    Near   = 0b010000,  // -z
    Far    = 0b100000,  // +z
};

}  // namespace math
}  // namespace sr

// Enable bitmask operators for OutCode.
template<>
struct enable_bitmask_operators<sr::math::OutCode>
{
    static constexpr bool enable = true;
};
