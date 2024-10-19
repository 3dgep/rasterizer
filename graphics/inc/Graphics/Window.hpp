#pragma once

#include <memory>
#include <string_view>

namespace sr
{
class WindowImpl;

class Window
{
public:
    Window() = default;

    Window( std::string_view title, int width, int height );

    void create( std::string_view title, int width, int height );

private:
    std::unique_ptr<WindowImpl> pImpl;
};
}