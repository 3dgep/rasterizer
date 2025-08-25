#pragma once

#include "SpriteSheet.hpp"

namespace sr
{
inline namespace graphics
{
class SpriteAnimation
{
public:
    SpriteAnimation() = default;

    /// <summary>
    /// Constructs a sprite animation from a sprite sheet with specified frame rate and frames.
    /// </summary>
    /// <param name="spriteSheet">The sprite sheet containing the animation frames.</param>
    /// <param name="fps">The playback speed in frames per second. Default is 30.0f.</param>
    /// <param name="frames">The indices of frames in the sprite sheet to use for this animation.
    /// If empty, all frames from the sprite sheet will be used in sequential order.</param>
    explicit SpriteAnimation( std::shared_ptr<SpriteSheet> spriteSheet, float fps = 30.0f, std::span<const int> frames = {} );

    /// <summary>
    /// Updates the animation's current time based on the elapsed time since the last frame.
    /// </summary>
    /// <param name="deltaTime">The elapsed time in seconds since the last update.</param>
    void update( float deltaTime ) noexcept;

    /// <summary>
    /// Gets the total duration of the animation in seconds.
    /// </summary>
    /// <returns>The total duration of the animation in seconds.</returns>
    float getDuration() const noexcept;

    /// <summary>
    /// Checks if the animation has finished playing.
    /// </summary>
    /// <returns>True if the animation has reached its end, false otherwise.</returns>
    bool isDone() const noexcept;

    /// <summary>
    /// Resets the animation to its initial state.
    /// </summary>
    void reset() noexcept;

    /// <summary>
    /// Access a specific frame of the animation by index.
    /// </summary>
    /// <param name="frame">The index of the frame to access.</param>
    /// <returns>A const reference to the Sprite at the specified frame index.</returns>
    const Sprite& operator[]( size_t frame ) const noexcept;

    /// <summary>
    /// Gets the sprite at a specific point in time within the animation.
    /// </summary>
    /// <param name="time">The time in seconds within the animation.</param>
    /// <returns>A const reference to the Sprite at the specified time.</returns>
    const Sprite& at( float time ) const noexcept;

    /// <summary>
    /// Implicitly converts the animation to the current frame's sprite.
    /// </summary>
    /// <returns>A const reference to the current frame's Sprite.</returns>
    operator const Sprite&() const noexcept;

private:
    std::shared_ptr<SpriteSheet> m_SpriteSheet;
    std::vector<int>             m_Frames;             // The frames in the spritesheet.
    float                        m_FrameRate = 30.0f;  // Animation frame-rate.
    float                        m_Time      = 0.0f;   // Current animation time.
};
}  // namespace graphics
}  // namespace sr