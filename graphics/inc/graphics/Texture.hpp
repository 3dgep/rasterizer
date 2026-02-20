#pragma once

struct SDL_Texture;

namespace sr
{
inline namespace graphics
{
class Texture
{
public:
    Texture() = default;
    ~Texture();

    int getWidth() const noexcept;

    int getHeight() const noexcept;

    /// <summary>
    /// Explicitly converts the object to a boolean value indicating whether it is valid.
    /// </summary>
    explicit operator bool() const noexcept
    {
        return m_Texture != nullptr;
    }

    Texture( const Texture& copy )                 = delete;
    Texture( Texture&& other ) noexcept            = default;
    Texture& operator=( const Texture& copy )      = delete;
    Texture& operator=( Texture&& other ) noexcept = default;

    SDL_Texture* getSDL_Texture() const
    {
        return m_Texture;
    }

private:
    friend class Renderer;  // Only the Renderer class can create Texture instances.
    explicit Texture( SDL_Texture* texture );
    SDL_Texture* m_Texture = nullptr;
};
}  // namespace graphics

}  // namespace sr
