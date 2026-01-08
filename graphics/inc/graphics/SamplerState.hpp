#pragma once

#include "Color.hpp"
#include "Enums.hpp"

namespace sr
{
inline namespace graphics
{
struct SamplerState
{
    AddressMode addressMode           = AddressMode::Wrap;  ///< Method to use for addressing out-of-bounds texture coordinates.
    Color       borderColor           = Color::Black;       ///< Border color used when AddressMode::Border is used.
    bool        normalizedCoordinates = false;              ///< Use normalized texture coordinates [0, 1] or integer texel coordinates.

    /// <summary>
    /// Constructs a SamplerState with the specified addressing mode, border color, and coordinate normalization setting.
    /// </summary>
    /// <param name="addressMode">The addressing mode to use when texture coordinates fall outside the [0, 1] range. Defaults to AddressMode::Wrap.</param>
    /// <param name="borderColor">The color to use when the address mode is set to border/clamp to border. Defaults to Color::Black.</param>
    /// <param name="normalizedCoordinates">Specifies whether texture coordinates are normalized (true) or in pixel/texel space (false). Defaults to false.</param>
    explicit SamplerState( AddressMode addressMode = AddressMode::Wrap, const Color& borderColor = Color::Black, bool normalizedCoordinates = false ) noexcept
    : addressMode( addressMode )
    , borderColor( borderColor )
    , normalizedCoordinates( normalizedCoordinates )
    {}

    /// <summary>
    /// Creates a copy of this sampler state with the specified border color.
    /// </summary>
    /// <param name="color">The color to use when sampling outside the texture boundaries in border addressing mode.</param>
    /// <returns>A sampler state configured with the specified border color.</returns>
    SamplerState withBorderColor( const Color& color ) const noexcept;

    static const SamplerState WrapUnnormalized;
    static const SamplerState WrapNormalized;
    static const SamplerState MirrorUnnormalized;
    static const SamplerState MirrorNormalized;
    static const SamplerState ClampUnnormalized;
    static const SamplerState ClampNormalized;
    static const SamplerState BorderUnnormalized;
    static const SamplerState BorderNormalized;
};

inline SamplerState SamplerState::withBorderColor( const Color& color ) const noexcept
{
    SamplerState s { *this };
    s.borderColor  = color;
    return s;
}

}  // namespace graphics
}  // namespace sr
