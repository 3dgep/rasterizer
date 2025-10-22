#pragma once

#include <Timer.hpp>

#include <graphics/Rasterizer.hpp>

#include <glm/vec2.hpp>

#include <filesystem>

/// <summary>
///  This class is used to draw a background sprite on the screen.
/// </summary>
class Background final
{
public:
    Background() = default;

    /// <summary>
    /// Create a scrolling background sprite.
    /// </summary>
    /// <param name="filePath">The path the to the background image.</param>
    /// <param name="scale">Scale the background sprite.</param>
    /// <param name="scrollDirection">The direction to scroll the background.</param>
    /// <param name="scrollSpeed">The speed to scroll the background.</param>
    explicit Background( const std::filesystem::path& filePath, float scale = 1.0f, const glm::vec2& scrollDirection = { 0.0f, 1.0f }, float scrollSpeed = 1.0f );

    /// <summary>
    /// Update the scrolling background.
    /// </summary>
    /// <param name="timer">The timer used to update the texture offset.</param>
    void update( const Timer& timer );

    /// <summary>
    /// Draw this background using the rasterizer.
    /// </summary>
    /// <param name="rasterizer">The rasterizer to draw with.</param>
    void draw( sr::graphics::Rasterizer& rasterizer ) const;

private:
    std::shared_ptr<sr::graphics::Image> backgroundImage;

    glm::vec2 scrollDirection { 0.0f };
    float     scrollSpeed { 0.0f };
    float     scale { 1.0f };
    glm::vec2 textureOffset { 0.0f };
};
