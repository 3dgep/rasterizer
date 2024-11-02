#include "WindowSDL3.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <iostream>

using namespace sr::graphics;

struct SDL_Context
{
    SDL_Context()
    {
        if ( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMEPAD ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError() );
            throw std::exception( SDL_GetError() );
        }
    }

    ~SDL_Context()
    {
        SDL_Quit();
    }
};

WindowSDL3::WindowSDL3( std::string_view _title, int _width, int _height, bool _fullscreen )
: title { _title }
, width { 0 }
, height { 0 }
, fullscreen { _fullscreen }
{
    static SDL_Context context;  // Ensure the context is created once for the entire application.

    SDL_WindowFlags flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if ( !SDL_CreateWindowAndRenderer( title.data(), _width, _height, flags | SDL_WINDOW_RESIZABLE, &window, &renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s", SDL_GetError() );
        throw std::exception( SDL_GetError() );
    }

    WindowSDL3::resize( width, height );
}

WindowSDL3::~WindowSDL3()
{
    SDL_DestroyTexture( texture );
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
}

int WindowSDL3::getWidth() const noexcept
{
    return width;
}

int WindowSDL3::getHeight() const noexcept
{
    return height;
}

void WindowSDL3::resize( int _width, int _height )
{
    if ( width != _width || height != _height )
    {
        if ( !SDL_SetWindowSize( window, _width, _height ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to resize window: %s", SDL_GetError() );
            return;
        }

        if ( texture )
            SDL_DestroyTexture( texture );

        texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, _width, _height );
        if ( !texture )
        {
            SDL_LogError( SDL_LOG_CATEGORY_RENDER, "Failed to create texture: %s", SDL_GetError() );
            return;
        }

        width  = _width;
        height = _height;
    }
}

void WindowSDL3::present()
{
    if ( !SDL_RenderTexture( renderer, texture, nullptr, nullptr ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_RENDER, "Failed to copy texture to render target: %s", SDL_GetError() );
        return;
    }

    if( !SDL_RenderPresent( renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_RENDER, "Failed to present: %s", SDL_GetError() );
        return;
    }
}
