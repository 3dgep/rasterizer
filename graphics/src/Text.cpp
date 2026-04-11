#include <graphics/Text.hpp>
#include <SDL_ttf_context.hpp>

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

void Text::TextDeleter::operator()( TTF_Text* text ) const
{
    TTF_DestroyText( text );
}

Text::Text( std::string_view text, const Color& fillColor, const Color& outlineColor )
: Text( Font::Default, text, fillColor, outlineColor )
{}

Text::Text( const Font& font, std::string_view text, const Color& fillColor, const Color& outlineColor )
: m_Font { font }
{
    context = SDL_ttf_context::get();

    m_FillText.reset( TTF_CreateText( SDL_ttf_context::get()->textEngine, m_Font.getTTF_FillFont(), text.data(), text.length() ) );
    m_OutlineText.reset( TTF_CreateText( SDL_ttf_context::get()->textEngine, m_Font.getTTF_OutlineFont(), text.data(), text.length() ) );

    if ( !m_FillText )
    {
        std::cerr << "Failed to create Text: " << SDL_GetError();
        return;
    }

    setFillColor( fillColor );
    setOutlineColor( outlineColor );
}

Text::Text(const Text& copy)
: Text(copy.getFont(), copy.getText(), copy.getFillColor(), copy.getOutlineColor() )
{}

Text::Text( Text&& ) noexcept            = default;
Text::~Text()                            = default;

Text& Text::operator=( const Text& other )
{
    if ( &other == this )
        return *this;

    context    = other.context;

    auto& font = other.getFont();
    auto  text = other.getText();

    m_FillText.reset( TTF_CreateText( SDL_ttf_context::get()->textEngine, font.getTTF_FillFont(), text.data(), text.size() ) );
    m_OutlineText.reset( TTF_CreateText( SDL_ttf_context::get()->textEngine, font.getTTF_OutlineFont(), text.data(), text.size() ) );

    setFillColor( other.getFillColor() );
    setOutlineColor( other.getOutlineColor() );

    return *this;
}

Text& Text::operator=( Text&& ) noexcept = default;

Text& Text::operator=( std::string_view string )
{
    return setText( string );  // NOLINT(misc-unconventional-assign-operator)
}

Text& Text::operator+=( std::string_view string )
{
    return appendString( string );
}

Text& Text::setText( std::string_view string )
{
    if ( !TTF_SetTextString( m_FillText.get(), string.data(), string.length() ) || !TTF_SetTextString( m_OutlineText.get(), string.data(), string.length() ) )
    {
        std::cerr << "Failed to set text string: " << SDL_GetError() << std::endl;
    }

    return *this;
}

std::string_view Text::getText() const
{
    if ( m_FillText )
        return m_FillText->text;
    return {};
}

Text& Text::appendString( std::string_view string )
{
    if ( !TTF_AppendTextString( m_FillText.get(), string.data(), string.length() ) || !TTF_AppendTextString( m_OutlineText.get(), string.data(), string.length() ) )
    {
        std::cerr << "Failed to append string to text: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Color Text::getFillColor() const
{
    Uint8 r = 0, g = 0, b = 0, a = 0;
    if ( !TTF_GetTextColor( m_FillText.get(), &r, &g, &b, &a ) )
    {
        std::cerr << "Failed to get text color: " << SDL_GetError() << std::endl;
    }

    return { r, g, b, a };
}

Text& Text::setFillColor( const Color& color )
{
    if ( !TTF_SetTextColor( m_FillText.get(), color.channels.r, color.channels.g, color.channels.b, color.channels.a ) )
    {
        std::cerr << "Failed to set text color: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Text& Text::setOutlineColor( const Color& color )
{
    if ( !TTF_SetTextColor( m_OutlineText.get(), color.channels.r, color.channels.g, color.channels.b, color.channels.a ) )
    {
        std::cerr << "Failed to set text color: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Color Text::getOutlineColor() const
{
    Uint8 r = 0, g = 0, b = 0, a = 0;
    if ( !TTF_GetTextColor( m_OutlineText.get(), &r, &g, &b, &a ) )
    {
        std::cerr << "Failed to get text color: " << SDL_GetError() << std::endl;
    }

    return { r, g, b, a };
}

Text& Text::setDirection( Direction direction )
{
    if ( !TTF_SetTextDirection( m_FillText.get(), translateDirection( direction ) ) || !TTF_SetTextDirection( m_OutlineText.get(), translateDirection( direction ) ) )
    {
        std::cerr << "Failed to set text direction: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Text::Direction Text::getDirection() const
{
    return translateDirection( TTF_GetTextDirection( m_FillText.get() ) );
}

Text& Text::setFont( const Font& font )
{
    m_Font = font;

    if ( !TTF_SetTextFont( m_FillText.get(), m_Font.getTTF_FillFont() ) || !TTF_SetTextFont( m_OutlineText.get(), m_Font.getTTF_OutlineFont() ) )
    {
        std::cerr << "Failed to set text font: " << SDL_GetError() << std::endl;
    }

    return *this;
}

const Font& Text::getFont() const
{
    return m_Font;
}

Text& Text::setPosition( const glm::ivec2& pos )
{
    if ( !TTF_SetTextPosition( m_FillText.get(), pos.x, pos.y ) || !TTF_SetTextPosition( m_OutlineText.get(), pos.x, pos.y ) )
    {
        std::cerr << "Failed to set text position: " << SDL_GetError() << std::endl;
    }

    return *this;
}

glm::ivec2 Text::getPosition() const
{
    glm::ivec2 pos { -1 };
    if ( !TTF_GetTextPosition( m_FillText.get(), &pos.x, &pos.y ) )
    {
        std::cerr << "Failed to get text position: " << SDL_GetError() << std::endl;
    }

    return pos;
}

glm::ivec2 Text::getFillSize() const
{
    glm::ivec2 size { -1 };
    if ( !TTF_GetTextSize( m_FillText.get(), &size.x, &size.y ) )
    {
        std::cerr << "Failed to get text size: " << SDL_GetError() << std::endl;
    }

    return size;
}

glm::ivec2 Text::getOutlineSize() const
{
    glm::ivec2 size { -1 };
    if ( !TTF_GetTextSize( m_OutlineText.get(), &size.x, &size.y ) )
    {
        std::cerr << "Failed to get text size: " << SDL_GetError() << std::endl;
    }

    return size;
}

int Text::getFillWidth() const
{
    return getFillSize().x;
}

int Text::getOutlineWidth() const
{
    return getOutlineSize().x;
}

int Text::getFillHeight() const
{
    return getFillSize().y;
}

int Text::getOutlineHeight() const
{
    return getOutlineSize().y;
}

Text& Text::setWrapWidth( int width )
{
    if ( !TTF_SetTextWrapWidth( m_FillText.get(), width ) || !TTF_SetTextWrapWidth( m_OutlineText.get(), width ) )
    {
        std::cerr << "Failed to set text wrap width: " << SDL_GetError() << std::endl;
    }

    return *this;
}

int Text::getWrapWidth() const
{
    int wrapWidth = -1;
    if ( !TTF_GetTextWrapWidth( m_FillText.get(), &wrapWidth ) )
    {
        std::cerr << "Failed to get text wrap width: " << SDL_GetError() << std::endl;
    }

    return wrapWidth;
}
