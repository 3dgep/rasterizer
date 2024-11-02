#pragma once

#include "../WindowImpl.hpp"

#include <string>
#include <string_view>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

namespace sr
{
inline namespace graphics
{
class WindowSDL3 : public WindowImpl
{
public:
    WindowSDL3( std::string_view title, int width, int height, bool fullscreen = false );
    ~WindowSDL3() override;

    int  getWidth() const noexcept override;
    int  getHeight() const noexcept override;
    void resize( int width, int height ) override;

    void present() override;

public:
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;

    std::string title;
    int width = 0;
    int height = 0;
    bool fullscreen = false;
};
}  // namespace graphics
}  // namespace sr