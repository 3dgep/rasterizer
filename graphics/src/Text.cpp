#include <graphics/Text.hpp>

#include "graphics/Rasterizer.hpp"
#include "graphics/ResourceManager.hpp"

#include <SDL_ttf_context.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
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

void Text::SurfaceDeleter::operator()( SDL_Surface* surface ) const
{
    SDL_DestroySurface( surface );
}

void Text::setColor( const TextPtr& t, const Color& c )
{
    if ( !TTF_SetTextColor( t.get(), c.channels.r, c.channels.g, c.channels.b, c.channels.a ) )
    {
        std::cerr << "Failed to set text color: " << SDL_GetError() << std::endl;
    }
}

Color Text::getColor( const TextPtr& t )
{
    Uint8 r = 0, g = 0, b = 0, a = 0;
    if ( !TTF_GetTextColor( t.get(), &r, &g, &b, &a ) )
    {
        std::cerr << "Failed to get text color: " << SDL_GetError() << std::endl;
    }

    return { r, g, b, a };
}

glm::ivec2 Text::getSize( const TextPtr& t )
{
    glm::ivec2 size { -1 };
    if ( !TTF_GetTextSize( t.get(), &size.x, &size.y ) )
    {
        std::cerr << "Failed to get text size: " << SDL_GetError() << std::endl;
    }

    return size;
}

void Text::setWrapWidth( const TextPtr& t, int w )
{
    if ( !TTF_SetTextWrapWidth( t.get(), w ) )
    {
        std::cerr << "Failed to set text wrap width: " << SDL_GetError() << std::endl;
    }
}

Text::Text( std::string_view text, const Color& fillColor, const Color& outlineColor )
: Text( ResourceManager::loadFont(), text, fillColor, outlineColor )
{}

Text::Text( std::shared_ptr<const Font> font, std::string_view text, const Color& fillColor, const Color& outlineColor )
: m_Font { std::move( font ) }
{
    m_Text[Fill].reset( TTF_CreateText( SDL_ttf_context::textEngine(), m_Font->getTTF_FillFont(), text.data(), text.length() ) );
    m_Text[Outline].reset( TTF_CreateText( SDL_ttf_context::textEngine(), m_Font->getTTF_OutlineFont(), text.data(), text.length() ) );
    m_Text[Shadow].reset( TTF_CreateText( SDL_ttf_context::textEngine(), m_Font->getTTF_FillFont(), text.data(), text.length() ) );
    m_Text[Glow].reset( TTF_CreateText( SDL_ttf_context::textEngine(), m_Font->getTTF_FillFont(), text.data(), text.length() ) );

    for ( const auto& t: m_Text )
    {
        if ( !t )
        {
            std::cerr << "Failed to create Text: " << SDL_GetError();
            return;
        }
    }

    setFillColor( fillColor );
    setOutlineColor( outlineColor );
    setShadowColor( outlineColor );
    setGlowColor( Color::LightYellow );
}

Text::Text( const Text& copy )
: Text( copy.getFont(), copy.getText(), copy.getFillColor(), copy.getOutlineColor() )
{}

Text::Text( Text&& ) noexcept = default;
Text::~Text()                 = default;

Text& Text::operator=( const Text& other )
{
    if ( &other == this )
        return *this;

    auto font = other.getFont();
    auto text = other.getText();

    m_Text[Fill].reset( TTF_CreateText( SDL_ttf_context::textEngine(), font->getTTF_FillFont(), text.data(), text.size() ) );
    m_Text[Outline].reset( TTF_CreateText( SDL_ttf_context::textEngine(), font->getTTF_OutlineFont(), text.data(), text.size() ) );
    m_Text[Shadow].reset( TTF_CreateText( SDL_ttf_context::textEngine(), font->getTTF_FillFont(), text.data(), text.size() ) );
    m_Text[Glow].reset( TTF_CreateText( SDL_ttf_context::textEngine(), font->getTTF_FillFont(), text.data(), text.size() ) );

    for ( const auto& t: m_Text )
    {
        if ( !t )
        {
            std::cerr << "Failed to create Text: " << SDL_GetError();
            return *this;
        }
    }

    setFillColor( other.getFillColor() );
    setOutlineColor( other.getOutlineColor() );
    setShadowColor( other.getShadowColor() );
    setGlowColor( other.getGlowColor() );

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

std::string_view Text::getText() const
{
    if ( m_Text[Fill] && m_Text[Fill]->text )
        return m_Text[Fill]->text;
    return {};
}

Text& Text::setText( std::string_view string )
{
    if ( string != getText() )
    {
        for ( const auto& t: m_Text )
        {
            if ( !TTF_SetTextString( t.get(), string.data(), string.length() ) )
            {
                std::cerr << "Failed to set text string: " << SDL_GetError() << std::endl;
            }
        }
        m_IsDirty = true;
    }

    return *this;
}

Text& Text::appendString( std::string_view string )
{
    for ( const auto& t: m_Text )
    {
        if ( !TTF_AppendTextString( t.get(), string.data(), string.length() ) )
        {
            std::cerr << "Failed to append string to text: " << SDL_GetError() << std::endl;
        }
    }

    m_IsDirty = true;

    return *this;
}

Color Text::getFillColor() const
{
    return getColor( m_Text[Fill] );
}

Text& Text::setFillColor( const Color& color )
{
    if ( getFillColor() != color )
    {
        setColor( m_Text[Fill], color );
        m_IsDirty = true;
    }
    return *this;
}

Color Text::getOutlineColor() const
{
    return getColor( m_Text[Outline] );
}

Text& Text::setOutlineColor( const Color& color )
{
    if ( getOutlineColor() != color )
    {
        setColor( m_Text[Outline], color );
        m_IsDirty = true;
    }
    return *this;
}

Color Text::getShadowColor() const
{
    return getColor( m_Text[Shadow] );
}

Text& Text::setShadowColor( const Color& color )
{
    if ( getShadowColor() != color )
    {
        setColor( m_Text[Shadow], color );
        m_IsDirty = true;
    }
    return *this;
}

const glm::ivec2& Text::getShadowOffset() const
{
    return m_ShadowOffset;
}

Text& Text::setShadowOffset( const glm::ivec2& offset )
{
    if ( m_ShadowOffset != offset )
    {
        m_ShadowOffset = offset;
        m_IsDirty      = true;
    }
    return *this;
}

Color Text::getGlowColor() const
{
    return getColor( m_Text[Glow] );
}

Text& Text::setGlowColor( const Color& color )
{
    if ( getGlowColor() != color )
    {
        setColor( m_Text[Glow], color );
        m_IsDirty = true;
    }
    return *this;
}

int Text::getGlowRadius() const
{
    return m_GlowRadius;
}

Text& Text::setGlowRadius( int radius )
{
    if ( m_GlowRadius != radius )
    {
        m_GlowRadius = radius;
        m_IsDirty    = true;
    }
    return *this;
}

Text::Direction Text::getDirection() const
{
    return translateDirection( TTF_GetTextDirection( m_Text[Fill].get() ) );
}

Text& Text::setDirection( Direction direction )
{
    if ( getDirection() != direction )
    {
        for ( const auto& t: m_Text )
        {
            if ( !TTF_SetTextDirection( t.get(), translateDirection( direction ) ) )
            {
                std::cerr << "Failed to set text direction: " << SDL_GetError() << std::endl;
            }
        }
        m_IsDirty = true;
    }
    return *this;
}

Text& Text::setFont( std::shared_ptr<const Font> font )
{
    if ( m_Font != font )
    {

        m_Font = std::move( font );

        auto setTextFont = []( TTF_Text* t, TTF_Font* f ) {
            if ( !TTF_SetTextFont( t, f ) )
            {
                std::cerr << "Failed to set text font: " << SDL_GetError() << std::endl;
            }
        };

        for ( TextEffect e: { Fill, Shadow, Glow } )
        {
            setTextFont( m_Text[e].get(), m_Font->getTTF_FillFont() );
        }
        setTextFont( m_Text[Outline].get(), m_Font->getTTF_OutlineFont() );

        m_IsDirty = true;
    }

    return *this;
}

std::shared_ptr<const Font> Text::getFont() const
{
    return m_Font;
}

glm::ivec2 Text::getPosition() const
{
    glm::ivec2 pos { -1 };
    if ( !TTF_GetTextPosition( m_Text[Fill].get(), &pos.x, &pos.y ) )
    {
        std::cerr << "Failed to get text position: " << SDL_GetError() << std::endl;
    }

    return pos;
}

Text& Text::setPosition( const glm::ivec2& pos )
{
    if ( getPosition() != pos )
    {
        for ( auto& t: m_Text )
        {
            if ( !TTF_SetTextPosition( t.get(), pos.x, pos.y ) )
            {
                std::cerr << "Failed to set text position: " << SDL_GetError() << std::endl;
            }
        }
        m_IsDirty = true;
    }

    return *this;
}

glm::ivec2 Text::getFillSize() const
{
    return getSize( m_Text[Fill] );
}

glm::ivec2 Text::getOutlineSize() const
{
    return getSize( m_Text[Outline] );
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

int Text::getWrapWidth() const
{
    int wrapWidth = -1;
    if ( !TTF_GetTextWrapWidth( m_Text[Fill].get(), &wrapWidth ) )
    {
        std::cerr << "Failed to get text wrap width: " << SDL_GetError() << std::endl;
    }

    return wrapWidth;
}

Text& Text::setWrapWidth( int width )
{
    if ( getWrapWidth() != width )
    {
        int outline = m_Font ? m_Font->getOutline() : 0;

        setWrapWidth( m_Text[Fill], width );
        setWrapWidth( m_Text[Outline], width + outline * 2 );
        setWrapWidth( m_Text[Shadow], width );
        setWrapWidth( m_Text[Glow], width );

        m_IsDirty = true;
    }

    return *this;
}

Task Text::updateCachedSurface() const
{
    int        padding  = std::max( { m_Font->getOutline(), m_GlowRadius, std::abs( m_ShadowOffset.x ), std::abs( m_ShadowOffset.y ) } );
    glm::ivec2 textSize = glm::clamp( getFillSize() + ( padding * 2 ), { 1, 1 }, { 8192, 8192 } );

    SurfacePtr surface { SDL_CreateSurface( textSize.x, textSize.y, SDL_PIXELFORMAT_RGBA32 ) };
    // Clear the surface with the font's fill color to avoid the "transparent black" halo effect.
    Color fillColor      = getFillColor();
    fillColor.channels.a = 0;  // Start with a fully transparent color to clear the surface.
    SDL_FillSurfaceRect( surface.get(), nullptr, fillColor.rgba );

    co_yield nullptr;

    // Draw the drop shadow.
    if ( std::abs( m_ShadowOffset.x ) > 0 || std::abs( m_ShadowOffset.y ) > 0 )
    {
        if ( !TTF_DrawSurfaceText( m_Text[Shadow].get(), m_ShadowOffset.x + padding, m_ShadowOffset.y + padding, surface.get() ) )
        {
            std::cerr << "Failed to draw text drop shadow to the surface: " << SDL_GetError() << std::endl;
        }
        co_yield nullptr;
    }

    // Draw the glow.
    if ( m_GlowRadius > 0 )
    {
        // Render the glow text once to a temporary surface.
        glm::ivec2 glowSize = getSize( m_Text[Glow] );
        SurfacePtr glowText { SDL_CreateSurface( glowSize.x, glowSize.y, SDL_PIXELFORMAT_RGBA32 ) };
        Color      glowClear = getGlowColor();
        glowClear.channels.a = 0;
        SDL_FillSurfaceRect( glowText.get(), nullptr, glowClear.rgba );
        if ( !TTF_DrawSurfaceText( m_Text[Glow].get(), 0, 0, glowText.get() ) )
        {
            std::cerr << "Failed to draw glow text to temporary surface: " << SDL_GetError() << std::endl;
        }

        // Blit the glow text at each offset onto the surface.
        for ( int dy = -m_GlowRadius; dy <= m_GlowRadius; ++dy )
        {
            for ( int dx = -m_GlowRadius; dx <= m_GlowRadius; ++dx )
            {
                if ( dx == 0 && dy == 0 )
                    continue;

                if ( ( dx * dx + dy * dy ) <= ( m_GlowRadius * m_GlowRadius ) )
                {
                    SDL_Rect dst { dx + padding, dy + padding, glowText->w, glowText->h };
                    if ( !SDL_BlitSurface( glowText.get(), nullptr, surface.get(), &dst ) )
                    {
                        std::cerr << "Failed to blit glow text to surface: " << SDL_GetError() << std::endl;
                    }
                }
            }
            co_yield nullptr;
        }
    }

    // Draw the outline.
    int outline = m_Font->getOutline();
    if ( outline > 0 )
    {
        if ( !TTF_DrawSurfaceText( m_Text[Outline].get(), -outline * 2 + padding, -outline * 2 + padding, surface.get() ) )
        {
            std::cerr << "Failed to draw text outline to the surface: " << SDL_GetError() << std::endl;
        }
        co_yield nullptr;
    }

    // Draw the fill.
    if ( !TTF_DrawSurfaceText( m_Text[Fill].get(), padding, padding, surface.get() ) )
    {
        std::cerr << "Failed to draw text fill to the surface: " << SDL_GetError() << std::endl;
    }

    // Swap the finished surface and padding into the cache.
    m_CachedSurface = std::move( surface );
    m_CachedPadding = padding;
}

void Text::draw( Image& image, int x, int y ) const
{
    if ( !image || !m_Font )
        return;

    static std::hash<Font> fontHasher;
    size_t                 fontHash = fontHasher( *m_Font );
    if ( m_IsDirty || m_FontHash != fontHash )
    {
        m_FontHash = fontHash;
        m_IsDirty  = false;
        // Start the coroutine to update the cached surface over multiple frames.
        m_UpdateTask = updateCachedSurface();
    }

    // Resume the coroutine to do some work this frame.
    if ( m_UpdateTask )
    {
        m_UpdateTask.resume();
    }

    if ( m_CachedSurface )
    {
        // Create a surface from the image.
        SurfacePtr surface = SurfacePtr { SDL_CreateSurfaceFrom( image.getWidth(), image.getHeight(), SDL_PIXELFORMAT_RGBA32, image.data(), image.getPitch() ) };
        if ( !surface )
        {
            std::cerr << "Failed to create surface from image: " << SDL_GetError() << std::endl;
            return;
        }

        // Copy the cached surface to the image's surface.
        SDL_Rect dstRect { x - m_CachedPadding, y - m_CachedPadding, m_CachedSurface->w, m_CachedSurface->h };
        if ( !SDL_BlitSurface( m_CachedSurface.get(), nullptr, surface.get(), &dstRect ) )
        {
            std::cerr << "Failed to blit cached surface to image surface: " << SDL_GetError() << std::endl;
        }
    }
}
