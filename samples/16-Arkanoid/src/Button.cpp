#include "Button.hpp"

#include "glm/trigonometric.hpp"

#include <iostream>

using namespace sr;

Button::Button( std::string_view text, Font font, const Color& color, const RectF& rect, std::function<void()> onClick )
: onClick { std::move( onClick ) }
, buttonText { std::move( font ), text, color }
{
    setRect( rect );
}

void Button::setSprite( State _state, const Sprite& sprite )
{
    switch ( _state )
    {
    case State::Default:
        defaultSprite = sprite;
        break;
    case State::Hover:
        hoverSprite = sprite;
        break;
    case State::Pressed:
        pressedSprite = sprite;
        break;
    default:;
    }
}

void Button::setRect( const RectF& rect ) noexcept
{
    aabb = AABB { { 0, 0, 0 }, { rect.width, rect.height, 0 } };
    const glm::vec2 anchor { rect.width / 2.0f, rect.height };
    transform.setPosition( rect.topLeft() + anchor );
    // transform.setPosition( rect.topLeft() );
    transform.setAnchor( anchor );
}

void Button::draw( Rasterizer& rasterizer )
{
    if ( !enabled )
        return;

    Sprite* sprite = defaultSprite ? &defaultSprite : nullptr;

    switch ( state )
    {
    case State::Hover:
        sprite = hoverSprite ? &hoverSprite : sprite;
        break;
    case State::Pressed:
        sprite = pressedSprite ? &pressedSprite : sprite;
        break;
    }

    // Draw the sprite for the button (if there is one)
    if ( sprite )
    {
        auto t = transform;
        auto s = sprite->getSize();
        t.setAnchor( { s.x / 2, s.y } );
        rasterizer.drawSprite( *sprite, t );
    }

    // Draw the button text.
    // Center the text on the button.
    const auto size = buttonText.getSize();
    const auto pos  = glm::vec2 { getAABB().center() } - glm::vec2 { size.x, -size.y } / 2.0f;
    rasterizer.drawText( buttonText, static_cast<int>( pos.x ), static_cast<int>( pos.y + ( state == State::Pressed ? 5.0f : 0.0f ) ) );

#if _DEBUG
    auto r = rasterizer;
    r.state.color = Color::Red;
    r.state.fillMode = FillMode::WireFrame;
    r.drawAABB( getAABB() );
#endif
}

void Button::processEvents( const SDL_Event& event )
{
    if ( !enabled )
        return;

    switch ( event.type )
    {
    case SDL_EVENT_MOUSE_MOTION:
        if ( getAABB().contains( { event.motion.x, event.motion.y, 0 } ) )
            setState( State::Hover );
        else
            setState( State::Default );
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if ( state == State::Hover )
            setState( State::Pressed );
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if ( state == State::Pressed )
            if ( onClick )
                onClick();
        break;
    }
}

void Button::setState( State newState )
{
    if ( state != newState )
    {
        endState( state );
        state = newState;
        startState( state );
    }
}

void Button::endState( State oldState ) {}

void Button::startState( State newState )
{
    switch ( newState )
    {
    case State::Default:
        std::cout << buttonText << ": Default" << std::endl;
        break;
    case State::Hover:
        std::cout << buttonText << ": Hovered" << std::endl;
        break;
    case State::Pressed:
        std::cout << buttonText << ": Pressed" << std::endl;
        break;
    }
}
