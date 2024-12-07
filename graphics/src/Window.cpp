#include <Graphics/Window.hpp>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

#include <exception>

using namespace sr;

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

Window::~Window()
{
    SDL_DestroyTexture( texture );
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
}

Window::Window( std::string_view title, int width, int height, bool fullscreen )
{
    create( title, width, height, fullscreen );
}

Window::Window( Window&& _window ) noexcept
: window( _window.window )
, renderer( _window.renderer )
, texture( _window.texture )
, width( _window.width )
, height( _window.height )
, fullScreen( _window.fullScreen )
{
    _window.window     = nullptr;
    _window.renderer   = nullptr;
    _window.texture    = nullptr;
    _window.width      = -1;
    _window.height     = -1;
    _window.fullScreen = false;
}

Window& Window::operator=( Window&& _window ) noexcept
{
    if ( this == &_window )
        return *this;

    window     = _window.window;
    renderer   = _window.renderer;
    texture    = _window.texture;
    width      = _window.width;
    height     = _window.height;
    fullScreen = _window.fullScreen;

    _window.window     = nullptr;
    _window.renderer   = nullptr;
    _window.texture    = nullptr;
    _window.width      = -1;
    _window.height     = -1;
    _window.fullScreen = false;

    return *this;
}

Window::operator bool() const
{
    return window != nullptr;
}

void Window::create( std::string_view title, int width, int height, bool fullscreen )
{
    static SDL_Context context;  // Ensure a single static context before creating an SDL window.

    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if ( !SDL_CreateWindowAndRenderer( title.data(), width, height, flags, &window, &renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s", SDL_GetError() );
        throw std::exception( SDL_GetError() );
    }

    resize( width, height );
}

void Window::destroy()
{
    SDL_DestroyWindow( window );
    window = nullptr;
}

bool Window::pollEvent( SDL_Event& event )
{
    if ( SDL_PollEvent( &event ) )
    {
        switch ( event.type )
        {
        case SDL_EVENT_WINDOW_RESIZED:
            if ( SDL_GetWindowFromEvent( &event ) == window )
            {
                int w, h;
                if ( SDL_GetWindowSizeInPixels( window, &w, &h ) )
                {
                    resize( w, h );
                }
            }
            break;
        case SDL_EVENT_QUIT:
            SDL_DestroyWindow( window );
            window = nullptr;
            break;
        }

        return true;
    }

    return false;
}

void Window::resize( int _width, int _height )
{
    if ( width != _width || height != _height )
    {
        _width  = std::max( 1, _width );
        _height = std::max( 1, _height );

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
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", SDL_GetError() );
            return;
        }

        width  = _width;
        height = _height;
    }
}

void Window::clear( const Color& color )
{
    SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
    SDL_RenderClear( renderer );
}

void Window::present()
{
    if ( !window )
        return;

    if ( !SDL_RenderPresent( renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to present: %s", SDL_GetError() );
        return;
    }
}