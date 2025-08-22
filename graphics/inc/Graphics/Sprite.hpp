#pragma once

#include "BlendMode.hpp"
#include "Image.hpp"

#include <math/Rect.hpp>

#include <memory>

namespace sr
{
inline namespace graphics
{
class Sprite
{
public:
    Sprite() = default;

    /// <summary>
    /// Constructs a Sprite object with the specified image and blend mode.
    /// </summary>
    /// <param name="image">A shared pointer to the Image to be used for the sprite.</param>
    /// <param name="blendMode">(Optional) The blending mode to use for rendering the sprite. Defaults to the default BlendMode.</param>
    explicit Sprite( std::shared_ptr<Image> image, const BlendMode& blendMode = BlendMode {} ) noexcept
    : m_Image { std::move( image ) }
    , m_Rect { 0, 0, static_cast<int32_t>( m_Image->getWidth() ), static_cast<int32_t>( m_Image->getHeight() ) }
    , m_BlendMode { blendMode }
    {}

    /// <summary>
    /// Constructs a Sprite object with the specified image, rectangle, and blend mode.
    /// </summary>
    /// <param name="image">A shared pointer to the Image to be used for the sprite.</param>
    /// <param name="rect">The rectangular region of the image to use.</param>
    /// <param name="blendMode">The blending mode to apply when rendering the sprite. Defaults to the default-constructed BlendMode.</param>
    explicit Sprite( std::shared_ptr<Image> image, const math::RectI& rect, const BlendMode& blendMode = BlendMode {} ) noexcept
    : m_Image { std::move( image )}
    , m_Rect { rect }
    , m_BlendMode { blendMode }
    {}

    /// <summary>
    /// Returns the UV coordinates as a 2D integer vector.
    /// </summary>
    /// <returns>The UV coordinates of the sprite in the image.</returns>
    glm::ivec2 getUV() const noexcept
    {
        return { m_Rect.left, m_Rect.top };
    }

    /// <summary>
    /// Returns the size of the sprite as a 2D integer vector.
    /// </summary>
    /// <returns>The width and height if the sprite in the image.</returns>
    glm::ivec2 getSize() const noexcept
    {
        return { m_Rect.width, m_Rect.height };
    }

    /// <summary>
    /// Returns the width of the sprite.
    /// </summary>
    /// <returns>The width of the sprite.</returns>
    int getWidth() const noexcept
    {
        return m_Rect.width;
    }

    /// <summary>
    /// Returns the height of the sprite.
    /// </summary>
    /// <returns>The height of the sprite in the image.</returns>
    int getHeight() const noexcept
    {
        return m_Rect.height;
    }

    /// <summary>
    /// Returns the sprite's rectangle within the image.
    /// </summary>
    /// <returns>The sprite's rectangle within the image.</returns>
    const math::RectI& getRect() const noexcept
    {
        return m_Rect;
    }

    /// <summary>
    /// Returns the image.
    /// </summary>
    /// <returns>The sprite's image.</returns>
    std::shared_ptr<Image> getImage() const noexcept
    {
        return m_Image;
    }

    /// <summary>
    /// Returns the blend color for the sprite.
    /// </summary>
    /// <returns>The blend color for the sprite.</returns>
    const Color& getColor() const noexcept
    {
        return m_Color;
    }

    /// <summary>
    /// This color is used to modulate the color of the sprite when rendering.
    /// </summary>
    /// <param name="color">The color to modulate the sprite during rendering.</param>
    void setColor(const Color& color ) noexcept
    {
        m_Color = color;
    }

    /// <summary>
    /// Retrieves the current blend mode for the sprite.
    /// </summary>
    /// <returns>The sprite's current blend mode.</returns>
    const BlendMode& getBlendMode() const noexcept
    {
        return m_BlendMode;
    }

    /// <summary>
    /// The blend mode to use for the sprite. Sprites with transparent pixels should use BlendMode::AlphaBlend.
    /// Note: Alpha blending can be computationally expensive. Don't use Alpha blending on opaque sprites.
    /// </summary>
    /// <param name="blendMode">The sprite's blend mode.</param>
    void setBlendMode( const BlendMode& blendMode ) noexcept
    {
        m_BlendMode = blendMode;
    }

    /// <summary>
    /// Checks whether the object contains a valid image.
    /// </summary>
    explicit operator bool() const noexcept
    {
        return m_Image != nullptr;
    }

private:
    // The image that contains the pixels for this sprite.
    std::shared_ptr<Image> m_Image;

    // The source rectangle of the sprite in the image.
    math::RectI m_Rect;

    // Color to apply to the sprite.
    Color m_Color { Color::White };

    // Blend mode to apply when rendering this sprite.
    BlendMode m_BlendMode;
};
}  // namespace graphics
}  // namespace sr