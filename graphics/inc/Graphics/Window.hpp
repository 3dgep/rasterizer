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

    Window( std::string_view title, int width, int height, bool fullScreen = false );
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

    void resize( int width, int height );

    void clear( const Color& color );

    void present( const Image& image );

private:
    SDL_Window*   m_Window   = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
    SDL_Texture*  m_Texture  = nullptr;

    int  m_Width      = -1;
    int  m_Height     = -1;
    bool m_FullScreen = false;
};

}  // namespace graphics
}  // namespace sr