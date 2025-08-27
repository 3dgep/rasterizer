#pragma once

#include "Image.hpp"
#include "SpriteSheet.hpp"

#include <filesystem>
#include <memory>

namespace sr
{
inline namespace graphics
{
namespace ResourceManager
{

/// <summary>
/// Load an image from a file path.
/// </summary>
/// <param name="filePath">The path to the image file to load.</param>
/// <returns>The loaded image as a shared pointer, or null if the image couldn't be loaded.</returns>
std::shared_ptr<Image> loadImage( const std::filesystem::path& filePath );

/// <summary>
/// Load a sprite sheet from a file path.
/// </summary>
/// <param name="filePath">The path to the image file to load.</param>
/// <param name="columns">The number of sprite columns in the sprite sheet. Default is 1.</param>
/// <param name="rows">The number of sprite rows in the sprite sheet. Default is 1.</param>
/// <param name="padding">The space between sprites in pixels. Default is 0.</param>
/// <param name="margin">The space around the border of the sprite sheet in pixels. Default is 0.</param>
/// <param name="blendMode">The blend mode to apply to the sprites. Default is no blending.</param>
/// <returns>The loaded sprite sheet, or null if the sprite sheet couldn't be loaded.</returns>
std::shared_ptr<SpriteSheet> loadSpriteSheet( const std::filesystem::path& filePath, uint32_t columns = 1, uint32_t rows = 1, uint32_t padding = 0, uint32_t margin = 0, const BlendMode& blendMode = BlendMode {} );

}
}  // namespace graphics
}  // namespace sr