#include <graphics/Texture.hpp>

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

using namespace sr::graphics;

Texture::Texture( SDL_Texture* texture )
: m_Texture( texture )
{}

Texture::~Texture()
{
    if ( m_Texture )
    {
        SDL_DestroyTexture( m_Texture );
        m_Texture = nullptr;
    }
}

int Texture::getWidth() const noexcept
{
    float width, height;
    if ( m_Texture )
    {
        if ( SDL_GetTextureSize( m_Texture, &width, &height ) )
        {
            return static_cast<int>( width );
        }
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to query texture size: %s", SDL_GetError() );
    }
    return -1;
}

int Texture::getHeight() const noexcept
{
    float width, height;
    if ( m_Texture )
    {
        if ( SDL_GetTextureSize( m_Texture, &width, &height ) )
        {
            return static_cast<int>( height );
        }
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to query texture size: %s", SDL_GetError() );
    }
    return -1;
}