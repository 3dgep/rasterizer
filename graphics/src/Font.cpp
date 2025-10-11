#include <graphics/Font.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <utility>

using namespace sr::graphics;

#include "PressStart2P.h"

namespace
{
TTF_Direction translateDirection( Font::Direction direction )
{
    switch ( direction )
    {
    case Font::Direction::Invalid:
        return TTF_DIRECTION_INVALID;
    case Font::Direction::LTR:
        return TTF_DIRECTION_LTR;
    case Font::Direction::RTL:
        return TTF_DIRECTION_RTL;
    case Font::Direction::TTB:
        return TTF_DIRECTION_TTB;
    case Font::Direction::BTT:
        return TTF_DIRECTION_BTT;
    }

    return TTF_DIRECTION_INVALID;
}
Font::Direction translateDirection( TTF_Direction direction )
{
    switch ( direction )
    {
    case TTF_DIRECTION_LTR:
        return Font::Direction::LTR;
    case TTF_DIRECTION_RTL:
        return Font::Direction::RTL;
    case TTF_DIRECTION_TTB:
        return Font::Direction::TTB;
    case TTF_DIRECTION_BTT:
        return Font::Direction::BTT;
    case TTF_DIRECTION_INVALID:
        return Font::Direction::Invalid;
    }

    return Font::Direction::Invalid;
}

Font::Hinting translateHinting( TTF_HintingFlags hinting )
{
    switch ( hinting )
    {
    case TTF_HINTING_INVALID:
        return Font::Hinting::Invalid;
    case TTF_HINTING_NORMAL:
        return Font::Hinting::Normal;
    case TTF_HINTING_LIGHT:
        return Font::Hinting::Light;
    case TTF_HINTING_MONO:
        return Font::Hinting::Mono;
    case TTF_HINTING_NONE:
        return Font::Hinting::None;
    case TTF_HINTING_LIGHT_SUBPIXEL:
        return Font::Hinting::LightSubpixel;
    }

    return Font::Hinting::Invalid;
}

TTF_HintingFlags translateHinting( Font::Hinting hinting )
{
    switch ( hinting )
    {
    case Font::Hinting::Invalid:
        return TTF_HINTING_INVALID;
    case Font::Hinting::Normal:
        return TTF_HINTING_NORMAL;
    case Font::Hinting::Light:
        return TTF_HINTING_LIGHT;
    case Font::Hinting::Mono:
        return TTF_HINTING_MONO;
    case Font::Hinting::None:
        return TTF_HINTING_NORMAL;
    case Font::Hinting::LightSubpixel:
        return TTF_HINTING_LIGHT_SUBPIXEL;
    }

    return TTF_HINTING_INVALID;
}

Font::Style translateStyle( TTF_FontStyleFlags style )
{
    Font::Style res = Font::Style::Normal;

    if ( ( style & TTF_STYLE_BOLD ) != 0 )
        res |= Font::Style::Bold;
    if ( ( style & TTF_STYLE_ITALIC ) != 0 )
        res |= Font::Style::Italic;
    if ( ( style & TTF_STYLE_UNDERLINE ) != 0 )
        res |= Font::Style::Underline;
    if ( ( style & TTF_STYLE_STRIKETHROUGH ) != 0 )
        res |= Font::Style::Strikethrough;

    return res;
}

TTF_FontStyleFlags translateStyle( Font::Style style )
{
    TTF_FontStyleFlags res = TTF_STYLE_NORMAL;

    if ( ( style & Font::Style::Bold ) != 0 )
        res |= TTF_STYLE_BOLD;
    if ( ( style & Font::Style::Italic ) != 0 )
        res |= TTF_STYLE_ITALIC;
    if ( ( style & Font::Style::Underline ) != 0 )
        res |= TTF_STYLE_UNDERLINE;
    if ( ( style & Font::Style::Strikethrough ) != 0 )
        res |= TTF_STYLE_STRIKETHROUGH;

    return res;
}

Font::HorizontalAlignment translateAlignment( TTF_HorizontalAlignment alignment )
{
    switch ( alignment )
    {
    case TTF_HORIZONTAL_ALIGN_INVALID:
        return Font::HorizontalAlignment::Invalid;
    case TTF_HORIZONTAL_ALIGN_LEFT:
        return Font::HorizontalAlignment::Left;
    case TTF_HORIZONTAL_ALIGN_CENTER:
        return Font::HorizontalAlignment::Center;
    case TTF_HORIZONTAL_ALIGN_RIGHT:
        return Font::HorizontalAlignment::Right;
    }

    return Font::HorizontalAlignment::Invalid;
}
TTF_HorizontalAlignment translateAlignment( Font::HorizontalAlignment alignment )
{
    switch ( alignment )
    {
    case Font::HorizontalAlignment::Invalid:
        return TTF_HORIZONTAL_ALIGN_INVALID;
    case Font::HorizontalAlignment::Left:
        return TTF_HORIZONTAL_ALIGN_LEFT;
    case Font::HorizontalAlignment::Center:
        return TTF_HORIZONTAL_ALIGN_CENTER;
    case Font::HorizontalAlignment::Right:
        return TTF_HORIZONTAL_ALIGN_RIGHT;
    }

    return TTF_HORIZONTAL_ALIGN_INVALID;
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
    }

    ~SDL_ttf_context()
    {
        TTF_Quit();
    }
};

}  // namespace

const Font Font::DefaultFont {};

Font::Font( float size )
{
    static SDL_ttf_context context; // Ensure the TTF library has been initialized.

    m_Font = TTF_OpenFontIO( SDL_IOFromConstMem( PressStart2P, std::size( PressStart2P ) ), true, size );
    if ( !m_Font )
    {
        std::cerr << "Failed to create default font: " << SDL_GetError() << std::endl;
        return;
    }
}

Font::Font( const std::filesystem::path& fontFile, float size )
{
    static SDL_ttf_context context;  // Ensure the TTF library has been initialized.

    m_Font = TTF_OpenFont( fontFile.string().c_str(), size );
    if ( !m_Font )
    {
        std::cerr << "Failed to load font: " << fontFile << std::endl;
    }
}

Font::Font( const Font& other )
{
    m_Font = TTF_CopyFont( other.m_Font );
}

Font::Font( Font&& other ) noexcept
{
    m_Font = std::exchange( other.m_Font, nullptr );
}

Font& Font::operator=( const Font& other )
{
    if ( &other == this )
        return *this;

    if ( m_Font )
        TTF_CloseFont( m_Font );

    m_Font = TTF_CopyFont( other.m_Font );

    return *this;
}

Font& Font::operator=( Font&& other ) noexcept
{
    if ( &other == this )
        return *this;

    m_Font = std::exchange( other.m_Font, nullptr );

    return *this;
}

Font::~Font()
{
    TTF_CloseFont( m_Font );
}

int Font::getAscent() const
{
    return TTF_GetFontAscent( m_Font );
}

int Font::getDecent() const
{
    return TTF_GetFontDescent( m_Font );
}

Font& Font::setDirection( Direction direction )
{
    if ( !TTF_SetFontDirection( m_Font, translateDirection( direction ) ) )
    {
        std::cerr << "Failed to set font direction: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Font::Direction Font::getDirection() const
{
    return translateDirection( TTF_GetFontDirection( m_Font ) );
}

int Font::getHorizontalDPI() const
{
    int hdpi = 0;
    if ( !TTF_GetFontDPI( m_Font, &hdpi, nullptr ) )
    {
        std::cerr << "Failed to get font DPI: " << SDL_GetError() << std::endl;
    }

    return hdpi;
}

int Font::getVerticalDPI() const
{
    int vdpi = 0;
    if ( !TTF_GetFontDPI( m_Font, nullptr, &vdpi ) )
    {
        std::cerr << "Failed to get font DPI: " << SDL_GetError() << std::endl;
    }

    return vdpi;
}

Font& Font::setSizeDPI( float size, int hdpi, int vdpi )
{
    if ( !TTF_SetFontSizeDPI( m_Font, size, hdpi, vdpi ) )
    {
        std::cerr << "Failed to set font size & DPI: " << SDL_GetError() << std::endl;
    }

    return *this;
}

std::string Font::getFamilyName() const
{
    return TTF_GetFontFamilyName( m_Font );
}

int Font::getHeight() const
{
    return TTF_GetFontHeight( m_Font );
}

float Font::getSize() const
{
    return TTF_GetFontSize( m_Font );
}

Font& Font::setSize( float size )
{
    if ( !TTF_SetFontSize( m_Font, size ) )
    {
        std::cerr << "Failed to set the font size: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Font::Hinting Font::getHinting() const
{
    return translateHinting( TTF_GetFontHinting( m_Font ) );
}

Font& Font::setHinting( Hinting hinting )
{
    TTF_SetFontHinting( m_Font, translateHinting( hinting ) );

    return *this;
}

bool Font::isKerning() const
{
    return TTF_GetFontKerning( m_Font );
}

Font& Font::setKerning( bool enabled )
{
    TTF_SetFontKerning( m_Font, enabled );

    return *this;
}

int Font::getLineSpacing() const
{
    return TTF_GetFontLineSkip( m_Font );
}

Font& Font::setLineSpacing( int spacing )
{
    TTF_SetFontLineSkip( m_Font, spacing );

    return *this;
}

int Font::getCharSpacing() const
{
    return TTF_GetFontCharSpacing( m_Font );
}

Font& Font::setCharSpacing( int spacing )
{
    if ( !TTF_SetFontCharSpacing( m_Font, spacing ) )
    {
        std::cerr << "Failed to set font char spacing: " << SDL_GetError() << std::endl;
    }

    return *this;
}

bool Font::isSDF() const
{
    return TTF_GetFontSDF( m_Font );
}

Font& Font::setSDF( bool enabled )
{
    if ( !TTF_SetFontSDF( m_Font, enabled ) )
    {
        std::cerr << "Failed to set font SDF rendering: " << SDL_GetError() << std::endl;
    }

    return *this;
}

bool Font::isScalable() const
{
    return TTF_FontIsScalable( m_Font );
}

bool Font::isFixedWidth() const
{
    return TTF_FontIsFixedWidth( m_Font );
}

Font::Style Font::getStyle() const
{
    return translateStyle( TTF_GetFontStyle( m_Font ) );
}

Font& Font::setStyle( Style style )
{
    TTF_SetFontStyle( m_Font, translateStyle( style ) );

    return *this;
}

int Font::getOutline() const
{
    return TTF_GetFontOutline( m_Font );
}

Font& Font::setOutline( int outline )
{
    if (!TTF_SetFontOutline( m_Font, outline ))
    {
        std::cerr << "Failed to set font outline: " << SDL_GetError() << std::endl;
    }

    return *this;
}

Font::Weight Font::getWeight() const
{
    return static_cast<Weight>( TTF_GetFontWeight( m_Font ) );
}

Font::HorizontalAlignment Font::getWrapAlignment() const
{
    return translateAlignment( TTF_GetFontWrapAlignment( m_Font ) );
}

Font& Font::setWrapAlignment( HorizontalAlignment alignment )
{
    TTF_SetFontWrapAlignment( m_Font, translateAlignment( alignment ) );

    return *this;
}

glm::ivec2 Font::getStringSize( std::string_view text, int wrapWidth ) const
{
    glm::ivec2 size { 0 };
    if (!TTF_GetStringSizeWrapped( m_Font, text.data(), text.size(), wrapWidth, &size.x, &size.y ))
    {
        std::cerr << "Failed to calculate string size: " << SDL_GetError() << std::endl;
    }

    return size;
}

Font::Font( TTF_Font* font )
: m_Font { font }
{}
