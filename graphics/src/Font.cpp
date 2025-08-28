#include <graphics/Font.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

using namespace sr::graphics;

#include "tiny.inl"

Font::Font( float size )
    : m_Size( size )
{
    m_Font = TTF_OpenFontIO( SDL_IOFromConstMem( tiny_ttf, std::size( tiny_ttf ) ), true, m_Size );
    if (!m_Font)
    {
        std::cerr << "Failed to create default font." << std::endl;
        return;
    }
}

Font::Font( const std::filesystem::path& fontFile, float size )
    : m_Size( size )
{
    m_Font = TTF_OpenFont( fontFile.string().c_str(), m_Size );
    if (!m_Font)
    {
        std::cerr << "Failed to load font: " << fontFile << std::endl;
    }
}

Font::Font( Font&& other ) noexcept
{
    m_Font = std::exchange( other.m_Font, nullptr );
    m_Size = std::exchange( other.m_Size, 0.0f );
}

Font& Font::operator=( Font&& other ) noexcept
{
    m_Font = std::exchange( other.m_Font, nullptr );
    m_Size = std::exchange( other.m_Size, 0.0f );

    return *this;
}

Font::~Font()
{
    TTF_CloseFont( m_Font );
}
