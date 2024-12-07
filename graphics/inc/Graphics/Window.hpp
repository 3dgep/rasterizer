#pragma once

#include "Color.hpp"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

#include <string_view>

namespace sr
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

    void present();

private:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;

    int width = -1;
    int height = -1;
    bool fullScreen = false;
};
}  // namespace sr