#pragma once

namespace sr
{
class WindowImpl
{
public:
    virtual int  getWidth() const noexcept = 0;
    virtual int  getHeight() const noexcept = 0;
    virtual void present() = 0;

protected:
    WindowImpl()                                   = default;
    virtual ~WindowImpl()                          = default;
    WindowImpl( const WindowImpl& )                = default;
    WindowImpl( WindowImpl&& )                     = default;
    WindowImpl& operator=( const WindowImpl& )     = default;
    WindowImpl& operator=( WindowImpl&& ) noexcept = default;
};
}  // namespace sr