#pragma once

namespace sr
{

inline namespace graphics
{

/// <summary>
/// Address modes used for texture sampling.
/// </summary>
enum class AddressMode
{
    Wrap,    ///< Tile the texture.
    Mirror,  ///< Flip the texture coordinates at integer boundaries.
    Clamp,   ///< Clamp texture coordinates in the range 0..1.
};

/// <summary>
/// FillMode determines how primitives are rendered.
/// * FillMode::WireFrame: Primitives are rendered as lines.
/// * FillMode::Solid: Primitives are rendered as solid objects.
/// </summary>
/// <remarks>
enum class FillMode
{
    WireFrame,  ///< Polygons are drawn as line segments.
    Solid       ///< Polygons interiors are filled.
};


/// <summary>
/// CullMode determines which triangles are rendered based on their orientation.
/// * CullMode::None: No culling; all triangles are drawn.
/// * CullMode::Front: Front-facing triangles are culled (not drawn).
/// * CullMode::Back: Back-facing triangles are culled (not drawn).
/// </summary>
enum class CullMode
{
    None,   ///< Always draw triangles.
    Front,  ///< Do not draw front-facing triangles.
    Back    ///< Do not draw back-facing triangles.
};

}  // namespace graphics
}  // namespace sr