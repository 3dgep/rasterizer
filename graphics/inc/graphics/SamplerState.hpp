#pragma once

#include "Color.hpp"
#include "Enums.hpp"

namespace sr
{
inline namespace graphics
{
struct SamplerState
{
    explicit SamplerState( AddressMode addressMode = AddressMode::Wrap, const Color& borderColor = Color::Black, bool normalizedCoordinates = false ) noexcept
    : addressMode( addressMode )
    , borderColor( borderColor )
    , normalizedCoordinates( normalizedCoordinates )
    {}

    AddressMode addressMode           = AddressMode::Wrap;  ///< Method to use for addressing out-of-bounds texture coordinates.
    Color       borderColor           = Color::Black;       ///< Border color used when AddressMode::Border is selected.
    bool        normalizedCoordinates = false;              ///< Use normalized texture coordinates (0..1) or integer texel coordinates.
};
}  // namespace graphics
}  // namespace sr
