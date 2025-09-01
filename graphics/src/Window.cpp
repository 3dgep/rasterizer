#include <graphics/Window.hpp>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <stdexcept>
#include <utility>  // for std::exchange

using namespace sr;

struct SDL_Context
{
    SDL_Context()
    {
        if ( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_GAMEPAD )  )
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

struct ImGui_Context
{
    ImGui_Context()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable multiple window/viewports. NOTE: SDL3 backend does not currently support Multi-viewports.

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup initial scaling
        float primaryDisplayScale = SDL_GetDisplayContentScale( SDL_GetPrimaryDisplay() );

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes( primaryDisplayScale );        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi         = primaryDisplayScale;  // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
        io.ConfigDpiScaleViewports = true;                 // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
    }

    ~ImGui_Context()
    {
        // Cleanup
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
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
: m_Window( std::exchange( window.m_Window, nullptr ) )
, m_Renderer( std::exchange( window.m_Renderer, nullptr ) )
, m_Texture( std::exchange( window.m_Texture, nullptr ) )
, m_Width( std::exchange( window.m_Width, -1 ) )
, m_Height( std::exchange( window.m_Height, -1 ) )
, m_Fullscreen( std::exchange( window.m_Fullscreen, false ) )
, m_VSync( std::exchange( window.m_VSync, true ) )
{}

Window& Window::operator=( Window&& window ) noexcept
{
    if ( this == &window )
        return *this;

    m_Window     = std::exchange( window.m_Window, nullptr );
    m_Renderer   = std::exchange( window.m_Renderer, nullptr );
    m_Texture    = std::exchange( window.m_Texture, nullptr );
    m_Width      = std::exchange( window.m_Width, -1 );
    m_Height     = std::exchange( window.m_Height, -1 );
    m_Fullscreen = std::exchange( window.m_Fullscreen, false );
    m_VSync      = std::exchange( window.m_VSync, true );

    return *this;
}

Window::operator bool() const
{
    return m_Window != nullptr;
}

void Window::create( std::string_view title, int width, int height, bool fullscreen )
{
    static SDL_Context   SDL_Context;    // Ensure a single static context before creating an SDL window.
    static ImGui_Context ImGui_Context;  // Ensure a single static context for ImGui.

    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
    flags |= fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if ( !SDL_CreateWindowAndRenderer( title.data(), width, height, flags, &m_Window, &m_Renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s", SDL_GetError() );
        throw std::runtime_error( SDL_GetError() );
    }

    // Enable vsync.
    SDL_SetRenderVSync( m_Renderer, m_VSync ? 1 : 0 );

    resize( width, height );

    // Setup Platform/Renderer backends for ImGui.
    ImGui_ImplSDL3_InitForSDLRenderer( m_Window, m_Renderer );
    ImGui_ImplSDLRenderer3_Init( m_Renderer );

    beginFrame();
}

int Window::getWidth() const
{
    return m_Width;
}

int Window::getHeight() const
{
    return m_Height;
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
        ImGui_ImplSDL3_ProcessEvent( &event );

        switch ( event.type )
        {
        case SDL_EVENT_WINDOW_RESIZED:
            if ( SDL_GetWindowFromEvent( &event ) == m_Window )
            {
                m_Width  = event.window.data1;
                m_Height = event.window.data2;
            }
            break;
        case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
        case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
        {
            SDL_DisplayID id    = SDL_GetDisplayForWindow( m_Window );
            float         scale = SDL_GetDisplayContentScale( id );
            // Change ImGui styles for the new display scale.
            ImGuiStyle style;
            ImGui::StyleColorsDark( &style );
            style.ScaleAllSizes( scale );  // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
            style.FontScaleDpi = scale;    // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
            ImGui::GetStyle()  = style;
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
    if ( m_Renderer )
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

void Window::resize( int width, int height )
{
    if ( m_Width != width || m_Height != height )
    {
        width  = std::max( 1, width );
        height = std::max( 1, height );

        if ( !SDL_SetWindowSize( m_Window, width, height ) )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to resize window: %s", SDL_GetError() );
            return;
        }

        m_Width  = width;
        m_Height = height;
    }
}

glm::vec2 Window::clientToImage( float _x, float _y, const Image& image ) const noexcept
{
    const float imageWidth   = static_cast<float>( image.getWidth() );
    const float imageHeight  = static_cast<float>( image.getHeight() );
    const float windowWidth  = static_cast<float>( m_Width );
    const float windowHeight = static_cast<float>( m_Height );

    const float aspectRatio = imageWidth / imageHeight;
    const float scaleWidth  = windowWidth / imageWidth;
    const float scaleHeight = windowHeight / imageHeight;

    float width, height;

    if ( scaleWidth < scaleHeight )
    {
        width  = windowWidth;
        height = width / aspectRatio;  // Scale height according to aspect ratio.
    }
    else
    {
        height = windowHeight;
        width  = height * aspectRatio;  // Scale width according to aspect ratio.
    }

    // Compute the offset into image space.
    float x = ( windowWidth - width ) / 2.0f;
    float y = ( windowHeight - height ) / 2.0f;
    // Compute scale.
    float sx = imageWidth / width;
    float sy = imageHeight / height;

    return { ( _x - x ) * sx, ( _y - y ) * sy };
}

void Window::clear( const Color& color )
{
    SDL_SetRenderDrawColor( m_Renderer, color.r, color.g, color.b, color.a );
    SDL_RenderClear( m_Renderer );
}

void Window::present()
{
    // ImGui rendering
    ImGui::Render();

    // Update for multiple viewports
    // Note: Multi-viewports are not supported with the SDL3 backend.
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    ImGui_ImplSDLRenderer3_RenderDrawData( ImGui::GetDrawData(), m_Renderer );

    if ( !SDL_RenderPresent( m_Renderer ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to present: %s", SDL_GetError() );
        return;
    }

    beginFrame();
}

void Window::present( const Image& image )
{
    if ( !m_Window )
        return;

    float w, h;
    if ( !m_Texture || SDL_GetTextureSize( m_Texture, &w, &h ) && ( w != static_cast<float>( image.getWidth() ) || h != static_cast<float>( image.getHeight() ) ) )
    {
        if ( m_Texture )
            SDL_DestroyTexture( m_Texture );

        m_Texture = SDL_CreateTexture( m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, static_cast<int>( image.getWidth() ), static_cast<int>( image.getHeight() ) );
        if ( !m_Texture )
        {
            SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture: %s", SDL_GetError() );
            return;
        }
        SDL_SetTextureScaleMode( m_Texture, SDL_SCALEMODE_NEAREST );
        SDL_SetTextureBlendMode( m_Texture, SDL_BLENDMODE_NONE );
    }

    // Copy the image data to the texture.
    if ( !SDL_UpdateTexture( m_Texture, nullptr, image.data(), static_cast<int>( image.getPitch() ) ) )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to update texture: %s", SDL_GetError() );
        return;
    }

    // Center the image on the screen while maintaining the aspect ratio.
    SDL_FRect dstRect {
        0.0f, 0.0f, static_cast<float>( m_Width ), static_cast<float>( m_Height )
    };
    SDL_FRect srcRect {
        0.0f, 0.0f, static_cast<float>( image.getWidth() ), static_cast<float>( image.getHeight() )
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

    present();
}

void Window::beginFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}
