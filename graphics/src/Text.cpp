#include <graphics/Text.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <utility>

using namespace sr::graphics;

namespace
{
TTF_Direction translateDirection( Text::Direction direction )
{
    switch ( direction )
    {
    case Text::Direction::Invalid:
        return TTF_DIRECTION_INVALID;
    case Text::Direction::LTR:
        return TTF_DIRECTION_LTR;
    case Text::Direction::RTL:
        return TTF_DIRECTION_RTL;
    case Text::Direction::TTB:
        return TTF_DIRECTION_TTB;
    case Text::Direction::BTT:
        return TTF_DIRECTION_BTT;
    }

    return TTF_DIRECTION_INVALID;
}
Text::Direction translateDirection( TTF_Direction direction )
{
    switch ( direction )
    {
    case TTF_DIRECTION_LTR:
        return Text::Direction::LTR;
    case TTF_DIRECTION_RTL:
        return Text::Direction::RTL;
    case TTF_DIRECTION_TTB:
        return Text::Direction::TTB;
    case TTF_DIRECTION_BTT:
        return Text::Direction::BTT;
    case TTF_DIRECTION_INVALID:
        return Text::Direction::Invalid;
    }

    return Text::Direction::Invalid;
}
}  // namespace

Text::Text( Text&& other ) noexcept
{
    m_Text = std::exchange( other.m_Text, nullptr );
}

Text::~Text()
{
    TTF_DestroyText( m_Text );
}

Text& Text::operator=( Text&& other ) noexcept
{
    if ( &other == this )
        return *this;

    m_Text = std::exchange( other.m_Text, nullptr );

    return *this;
}

Text& Text::operator=( std::string_view string )
{
    return setString( string );  // NOLINT(misc-unconventional-assign-operator)
}

Text& Text::operator+=( std::string_view string )
{
    return appendString( string );
}

Text& Text::setString( std::string_view string )
{
    if ( !TTF_SetTextString( m_Text, string.data(), string.length() ) )
    {
        std::cerr << "Failed to set text string: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Text& Text::appendString( std::string_view string )
{
    if ( !TTF_AppendTextString( m_Text, string.data(), string.length() ) )
    {
        std::cerr << "Failed to append string to text: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Color Text::getColor() const
{
    Uint8 r = 0, g = 0, b = 0, a = 0;
    if ( !TTF_GetTextColor( m_Text, &r, &g, &b, &a ) )
    {
        std::cerr << "Failed to get text color: " << SDL_GetError() << std::endl;
    }

    return { r, g, b, a };
}

Text& Text::setColor( const Color& color )
{
    if ( !TTF_SetTextColor( m_Text, color.r, color.g, color.b, color.a ) )
    {
        std::cerr << "Failed to set text color: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Text& Text::setDirection( Direction direction )
{
    if ( !TTF_SetTextDirection( m_Text, translateDirection( direction ) ) )
    {
        std::cerr << "Failed to set text direction: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Text::Direction Text::getDirection() const
{
    return translateDirection( TTF_GetTextDirection( m_Text ) );
}

Text& Text::setFont( const Font& font )
{
    if ( !TTF_SetTextFont( m_Text, font.m_Font ) )
    {
        std::cerr << "Failed to set text font: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Font Text::getFont() const
{
    TTF_Font* font = TTF_GetTextFont( m_Text );
    return { TTF_CopyFont( font ) };
}

Text& Text::setPosition( const glm::ivec2& pos )
{
    if ( !TTF_SetTextPosition( m_Text, pos.x, pos.y ) )
    {
        std::cerr << "Failed to set text position: " << SDL_GetError() << std::endl;
    }

    return *this;
}

glm::ivec2 Text::getPosition() const
{
    glm::ivec2 pos { -1 };
    if ( !TTF_GetTextPosition( m_Text, &pos.x, &pos.y ) )
    {
        std::cerr << "Failed to get text position: " << SDL_GetError() << std::endl;
    }

    return pos;
}

glm::ivec2 Text::getSize() const
{
    glm::ivec2 size { -1 };
    if ( !TTF_GetTextSize( m_Text, &size.x, &size.y ) )
    {
        std::cerr << "Failed to get text size: " << SDL_GetError() << std::endl;
    }

    return size;
}

Text& Text::setWrapWidth( int width )
{
    if (!TTF_SetTextWrapWidth( m_Text, width  ))
    {
        std::cerr << "Failed to set text wrap width: " << SDL_GetError() << std::endl;
    }

    return *this;
}

int Text::getWrapWidth() const
{
    int wrapWidth = -1;
    if (!TTF_GetTextWrapWidth( m_Text, &wrapWidth ))
    {
        std::cerr << "Failed to get text wrap width: " << SDL_GetError() << std::endl;
    }

    return wrapWidth;
}

Text::Text( TTF_Text* text )
: m_Text { text }
{}