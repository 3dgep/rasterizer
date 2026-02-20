#include <graphics/Renderer.hpp>

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

using namespace sr::graphics;

Renderer::Renderer( SDL_Renderer* renderer )
: m_Renderer( renderer )
{}

Texture Renderer::createTexture( const Image& image ) const
{
    if ( !m_Renderer )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Cannot create texture: Renderer is not set." );
        return {};
    }

    SDL_Texture* sdlTexture = SDL_CreateTexture( m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, image.getWidth(), image.getHeight() );
    if ( !sdlTexture )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", SDL_GetError() );
        return {};
    }

    if ( !SDL_UpdateTexture( sdlTexture, nullptr, image.data(), image.getPitch() ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to update texture: %s", SDL_GetError() );
        SDL_DestroyTexture( sdlTexture );
        return {};
    }

    return Texture { sdlTexture };
}
