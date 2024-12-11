#pragma once

#include "Color.hpp"
#include "Image.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <string_view>

namespace sr
{

inline namespace graphics
{

class WindowImpl;

class Window final
{
public:
    Window() = default;
    ~Window();

    Window( std::string_view title, int width, int height, bool fullscreen = false );
    Window( const Window& ) = delete;
    Window( Window&& window ) noexcept;

    Window& operator=( const Window& ) = delete;
    Window& operator=( Window&& window ) noexcept;

    explicit operator bool() const;

    void create( std::string_view title, int width, int height, bool fullScreen = false );

    /// <summary>
    /// Close and destroy the window.
    /// </summary>
    void destroy();

    bool pollEvent( SDL_Event& event );

    void setFullscreen( bool fullscreen );

    void toggleFullscreen();

    bool isFullscreen() const noexcept;

    void setVSync( bool enabled );

    void toggleVSync();

    bool isVSync() const noexcept;

    void clear( const Color& color );

    void resize( int width, int height );

    void present( const Image& image );

private:
    SDL_Window*   m_Window   = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    SDL_Texture*  m_Texture  = nullptr;

    int  m_Width      = -1;
    int  m_Height     = -1;
    bool m_Fullscreen = false;
    bool m_VSync      = true;
};

}  // namespace graphics
}  // namespace sr