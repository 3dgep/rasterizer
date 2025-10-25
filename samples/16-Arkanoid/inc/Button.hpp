#pragma once

#include <graphics/Rasterizer.hpp>
#include <graphics/SpriteSheet.hpp>
#include <graphics/Text.hpp>

#include <SDL3/SDL_events.h>

#include <functional>

class Button
{
public:
    /// <summary>
    /// The current button state.
    /// </summary>
    enum class State
    {
        Default,  ///< The button is in it's default state.
        Hover,    ///< The mouse is over the button.
        Pressed,  ///< The mouse button is pressed over the button.
    };

    Button() = default;

    /// <summary>
    /// Construct a button.
    /// </summary>
    /// <param name="text">The button text.</param>
    /// <param name="font">The font to use to render the button text.</param>
    /// <param name="color">The color of the button text. Default: White.</param>
    /// <param name="rect">The rectangle that represents the button's position on screen. Default: 0</param>
    /// <param name="onClick">The callback function to invoke when the button is clicked. Default: none</param>
    Button( std::string_view text, sr::graphics::Font font, const sr::graphics::Color& color = sr::graphics::Color::White, const sr::math::RectF& rect = sr::math::RectF {}, std::function<void()> onClick = {} );

    void setSprite( State state, const sr::graphics::Sprite& sprite );

    void setRect( const sr::math::RectF& rect ) noexcept;

    sr::math::AABB getAABB() const noexcept
    {
        return transform * aabb;
    }

    void setCallback( std::function<void()> _onClick ) noexcept
    {
        onClick = std::move( _onClick );
    }

    void setDefaultSprite( const sr::graphics::Sprite& sprite ) noexcept
    {
        defaultSprite = sprite;
    }

    void setHoverSprite( const sr::graphics::Sprite& sprite ) noexcept
    {
        hoverSprite = sprite;
    }

    void setPressedSprite( const sr::graphics::Sprite& sprite ) noexcept
    {
        pressedSprite = sprite;
    }

    /// <summary>
    /// Check if this button is enabled.
    /// </summary>
    /// <returns>`true` if the button is enabled, otherwise, false.</returns>
    bool isEnabled() const noexcept
    {
        return enabled;
    }

    /// <summary>
    /// Enable/disable the button.
    /// </summary>
    /// <param name="enable">`true` to enable the button, `false` to disable it.</param>
    void enable( bool enable ) noexcept
    {
        enabled = enable;
    }

    /// <summary>
    /// Forward any window events to the button. This allows the button to handle mouse over/mouse clicked events.
    /// </summary>
    /// <param name="event">The event to process.</param>
    void processEvents( const SDL_Event& event );

    /// <summary>
    /// Draw this button.
    /// </summary>
    /// <param name="rasterizer">The rasterizer to draw this button to.</param>
    void draw( sr::graphics::Rasterizer& rasterizer );

private:
    void setState( State newState );
    void endState( State oldState );
    void startState( State newState );

    std::function<void()> onClick;

    sr::graphics::Text buttonText;

    sr::math::Transform2D transform;
    sr::math::AABB        aabb;

    sr::graphics::Sprite defaultSprite;
    sr::graphics::Sprite hoverSprite;
    sr::graphics::Sprite pressedSprite;

    State state   = State::Default;
    bool  enabled = true;
};
