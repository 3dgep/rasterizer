#pragma once

#include "Image.hpp"
#include "Texture.hpp"

struct SDL_Renderer;

namespace sr
{
inline namespace graphics
{
class Renderer
{
public:
    Renderer() = default;

    /// <summary>
    /// Create a texture from the given image.
    /// </summary>
    Texture createTexture( const Image& image ) const;

private:
    friend class Window;  // Only the Window class can create Renderer instances.
    Renderer( SDL_Renderer* renderer );
    SDL_Renderer* m_Renderer = nullptr;
};
}  // namespace graphics
}  // namespace sr