#include "SDL_ttf_context.hpp"

#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdexcept>

SDL_ttf_context::SDL_ttf_context()
{
    if ( !TTF_Init() )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s", SDL_GetError() );
        throw std::runtime_error( SDL_GetError() );
    }

    textEngine = TTF_CreateSurfaceTextEngine();
}

SDL_ttf_context::~SDL_ttf_context()
{
    std::cout << "~SDL_ttf_context" << std::endl;
    TTF_DestroySurfaceTextEngine( textEngine );
    TTF_Quit();
}

std::shared_ptr<SDL_ttf_context> SDL_ttf_context::get()
{
    static std::shared_ptr<SDL_ttf_context> context = std::make_shared<SDL_ttf_context>();
    return context;
}
