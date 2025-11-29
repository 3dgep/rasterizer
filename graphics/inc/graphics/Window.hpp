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

    int getWidth() const;

    int getHeight() const;

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

    /// <summary>
    /// Convert coordinates from window client space to image space.
    /// </summary>
    /// <param name="x">The x-coordinate in window client space.</param>
    /// <param name="y">The y-coordinate in window client space.</param>
    /// <param name="image">The target image to convert coordinates for.</param>
    /// <returns>The converted coordinates in image space.</returns>
    glm::vec2 clientToImage( float x, float y, const Image& image ) const noexcept;

    /// <summary>
    /// Convert coordinates from window client space to image space.
    /// </summary>
    /// <param name="clientCoords">The coordinates in window client space.</param>
    /// <param name="image">The target image to convert coordinates for.</param>
    /// <returns>The converted coordinates in image space.</returns>
    glm::vec2 clientToImage( const glm::vec2& clientCoords, const Image& image ) const noexcept
    {
        return clientToImage( clientCoords.x, clientCoords.y, image );
    }

    void present();

    void present( const Image& image );

private:
    void beginFrame();

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