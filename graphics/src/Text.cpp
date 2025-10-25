#include <graphics/Text.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <codecvt>
#include <iostream>
#include <locale>
#include <utility>

using namespace sr::graphics;

namespace
{
std::string wstringToUTF8( std::wstring_view str )
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes( str.data(), str.data() + str.size() );
}

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

struct SDL_ttf_context
{
    SDL_ttf_context()
    {
        if ( !TTF_Init() )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s", SDL_GetError() );
            throw std::runtime_error( SDL_GetError() );
        }

        textEngine = TTF_CreateSurfaceTextEngine();
    }

    ~SDL_ttf_context()
    {
        TTF_DestroySurfaceTextEngine( textEngine );
        TTF_Quit();
    }

    TTF_TextEngine* textEngine = nullptr;
};

}  // namespace

TTF_TextEngine* Text::TextEngine()
{
    static SDL_ttf_context context;
    return context.textEngine;
}

Text::Text( Font font, std::string_view text, const Color& color )
: m_Font { std::move( font ) }
{
    m_Text = TTF_CreateText( TextEngine(), m_Font.getTTF_Font(), text.data(), text.length() );

    if ( !m_Text )
    {
        std::cerr << "Failed to create Text: " << SDL_GetError();
        return;
    }

    setColor( color );
}

Text::Text( Font font, std::wstring_view text, const Color& color )
: Text { std::move( font ), wstringToUTF8( text ), color }
{}

Text::Text( Text&& other ) noexcept
{
    m_Font = std::move( other.m_Font );
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

    m_Font = std::move( other.m_Font );
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

std::string_view Text::getText() const
{
    if ( m_Text )
        return m_Text->text;
    return {};
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
    m_Font = font;

    if ( !TTF_SetTextFont( m_Text, m_Font.getTTF_Font() ) )
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

int Text::getWidth() const
{
    return getSize().x;
}

int Text::getHeight() const
{
    return getSize().y;
}

Text& Text::setWrapWidth( int width )
{
    if ( !TTF_SetTextWrapWidth( m_Text, width ) )
    {
        std::cerr << "Failed to set text wrap width: " << SDL_GetError() << std::endl;
    }

    return *this;
}

int Text::getWrapWidth() const
{
    int wrapWidth = -1;
    if ( !TTF_GetTextWrapWidth( m_Text, &wrapWidth ) )
    {
        std::cerr << "Failed to get text wrap width: " << SDL_GetError() << std::endl;
    }

    return wrapWidth;
}
