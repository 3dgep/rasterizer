#pragma once

#include <filesystem>

struct TTF_Font;

namespace sr
{
inline namespace graphics
{
class Font
{
public:
    Font( float size = 12.0f );
    Font( const std::filesystem::path& fontFile, float size = 12.0f );
    Font( const Font& ) = delete;
    Font( Font&& ) noexcept;

    Font& operator=( const Font& ) = delete;
    Font& operator=( Font&& ) noexcept;

    ~Font();

private:
    TTF_Font* m_Font = nullptr;
    float     m_Size = 0.0f;
};
}  // namespace graphics
}  // namespace sr