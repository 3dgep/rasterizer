#include <graphics/Window.hpp>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

#include <stdexcept>

using namespace sr;

struct SDL_Context
{
    SDL_Context()
    {
        if ( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMEPAD ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s", SDL_GetError() );
            throw std::runtime_error( SDL_GetError() );
        }
    }

    ~SDL_Context()
    {
        SDL_Quit();
    }
};

Window::~Window()
{
    SDL_DestroyTexture( m_Texture );
    SDL_DestroyRenderer( m_Renderer );
    SDL_DestroyWindow( m_Window );
}

Window::Window( std::string_view title, int width, int height, bool fullscreen )
{
    create( title, width, height, fullscreen );
}

Window::Window( Window&& window ) noexcept
: m_Window( window.m_Window )
, m_Renderer( window.m_Renderer )
, m_Texture( window.m_Texture )
, m_Width( window.m_Width )
, m_Height( window.m_Height )
, m_Fullscreen( window.m_Fullscreen )
, m_VSync( window.m_VSync )
{
    window.m_Window     = nullptr;
    window.m_Renderer   = nullptr;
    window.m_Texture    = nullptr;
    window.m_Width      = -1;
    window.m_Height     = -1;
    window.m_Fullscreen = false;
    window.m_VSync      = true;
}

Window& Window::operator=( Window&& window ) noexcept
{
    if ( this == &window )
        return *this;

    m_Window     = window.m_Window;
    m_Renderer   = window.m_Renderer;
    m_Texture    = window.m_Texture;
    m_Width      = window.m_Width;
    m_Height     = window.m_Height;
    m_Fullscreen = window.m_Fullscreen;
    m_VSync      = window.m_VSync;

    window.m_Window     = nullptr;
    window.m_Renderer   = nullptr;
    window.m_Texture    = nullptr;
    window.m_Width      = -1;
    window.m_Height     = -1;
    window.m_Fullscreen = false;
    window.m_VSync      = true;

    return *this;
}

Window::operator bool() const
{
    return m_Window != nullptr;
}

void Window::create( std::string_view title, int width, int height, bool fullscreen )
{
    static SDL_Context context;  // Ensure a single static context before creating an SDL window.

    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if ( !SDL_CreateWindowAndRenderer( title.data(), width, height, flags, &m_Window, &m_Renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s", SDL_GetError() );
        throw std::exception( SDL_GetError() );
    }

    // Enable vsync.
    SDL_SetRenderVSync( m_Renderer, m_VSync ? 1 : 0 );

    resize( width, height );
}

void Window::destroy()
{
    SDL_DestroyWindow( m_Window );
    m_Window = nullptr;
}

bool Window::pollEvent( SDL_Event& event )
{
    if ( SDL_PollEvent( &event ) )
    {
        switch ( event.type )
        {
        case SDL_EVENT_WINDOW_RESIZED:
            if ( SDL_GetWindowFromEvent( &event ) == m_Window )
            {
                m_Width = event.window.data1;
                m_Height = event.window.data2;
            }
            break;
        case SDL_EVENT_QUIT:
            SDL_DestroyWindow( m_Window );
            m_Window = nullptr;
            break;
        }

        return true;
    }

    return false;
}

void Window::setFullscreen( bool fullscreen )
{
    if ( m_Window )
    {
        SDL_SetWindowFullscreen( m_Window, fullscreen );
        m_Fullscreen = fullscreen;
    }
}

void Window::toggleFullscreen()
{
    setFullscreen( !m_Fullscreen );
}

bool Window::isFullscreen() const noexcept
{
    return m_Fullscreen;
}

void Window::setVSync( bool enabled )
{
    if (m_Renderer)
    {
        SDL_SetRenderVSync( m_Renderer, enabled ? 1 : 0 );
        m_VSync = enabled;
    }
}

void Window::toggleVSync()
{
    setVSync( !m_VSync );
}

bool Window::isVSync() const noexcept
{
    return m_VSync;
}


void Window::resize( int _width, int _height )
{
    if ( m_Width != _width || m_Height != _height )
    {
        _width  = std::max( 1, _width );
        _height = std::max( 1, _height );

        if ( !SDL_SetWindowSize( m_Window, _width, _height ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to resize window: %s", SDL_GetError() );
            return;
        }

        m_Width  = _width;
        m_Height = _height;
    }
}

void Window::clear( const Color& color )
{
    SDL_SetRenderDrawColor( m_Renderer, color.r, color.g, color.b, color.a );
    SDL_RenderClear( m_Renderer );
}

void Window::present( const Image& image )
{
    if ( !m_Window )
        return;

    float w, h;
    if ( !m_Texture || SDL_GetTextureSize( m_Texture, &w, &h ) && ( w != static_cast<float>( image.width() ) || h != static_cast<float>( image.height() ) ) )
    {
        if ( m_Texture )
            SDL_DestroyTexture( m_Texture );

        m_Texture = SDL_CreateTexture( m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, static_cast<int>( image.width() ), static_cast<int>( image.height() ) );
        if ( !m_Texture )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", SDL_GetError() );
            return;
        }
        SDL_SetTextureScaleMode( m_Texture, SDL_SCALEMODE_NEAREST );
    }

    // Copy the image data to the texture.
    if ( !SDL_UpdateTexture( m_Texture, nullptr, image.data(), static_cast<int>( image.pitch() ) ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to update texture: %s", SDL_GetError() );
        return;
    }

    // Center the image on the screen while maintaining the aspect ratio.
    SDL_FRect dstRect {
        0.0f, 0.0f, static_cast<float>( m_Width ), static_cast<float>( m_Height )
    };
    SDL_FRect srcRect {
        0.0f, 0.0f, static_cast<float>( image.width() ), static_cast<float>( image.height() )
    };

    const float aspectRatio = srcRect.w / srcRect.h;
    const float scaleWidth  = dstRect.w / srcRect.w;
    const float scaleHeight = dstRect.h / srcRect.h;

    if ( scaleWidth < scaleHeight )
    {
        dstRect.h = dstRect.w / aspectRatio;
    }
    else
    {
        dstRect.w = dstRect.h * aspectRatio;
    }

    dstRect.x = ( static_cast<float>( m_Width ) - dstRect.w ) / 2;
    dstRect.y = ( static_cast<float>( m_Height ) - dstRect.h ) / 2;

    if ( !SDL_RenderTexture( m_Renderer, m_Texture, &srcRect, &dstRect ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to render texture: %s", SDL_GetError() );
        return;
    }

    if ( !SDL_RenderPresent( m_Renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to present: %s", SDL_GetError() );
        return;
    }
}