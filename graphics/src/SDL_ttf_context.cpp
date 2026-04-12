#include "SDL_ttf_context.hpp"

#include <graphics/ResourceManager.hpp>

#include <SDL3_ttf/SDL_ttf.h>

#include <stdexcept>

SDL_ttf_context::SDL_ttf_context()
{
    if ( !TTF_Init() )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s", SDL_GetError() );
        throw std::runtime_error( SDL_GetError() );
    }

    m_TextEngine = TTF_CreateSurfaceTextEngine();
}

SDL_ttf_context::~SDL_ttf_context()
{
    // Make sure the fonts in the resource manager are cleared before
    // shutting down the TTF font library.
    sr::ResourceManager::clearFonts();

    TTF_DestroySurfaceTextEngine( m_TextEngine );
    TTF_Quit();
}

SDL_ttf_context& SDL_ttf_context::get()
{
    static SDL_ttf_context context;
    return context;
}

TTF_TextEngine* SDL_ttf_context::textEngine()
{
    return get().m_TextEngine;
}
