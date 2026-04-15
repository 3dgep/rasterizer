#pragma once

#include <coroutine>
#include <utility>

namespace sr
{
inline namespace graphics
{

/// A simple coroutine type that suspends at each co_yield, allowing
/// work to be spread across multiple frames.
class Task
{
public:
    struct promise_type
    {
        Task get_return_object()
        {
            return Task { std::coroutine_handle<promise_type>::from_promise( *this ) };
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value( std::nullptr_t ) { return {}; }
        void                return_void() {}
        void                unhandled_exception() { std::terminate(); }
    };

    Task() = default;

    explicit Task( std::coroutine_handle<promise_type> h )
    : m_Handle { h }
    {}

    ~Task()
    {
        if ( m_Handle )
            m_Handle.destroy();
    }

    Task( const Task& )            = delete;
    Task& operator=( const Task& ) = delete;

    Task( Task&& other ) noexcept
    : m_Handle { std::exchange( other.m_Handle, nullptr ) }
    {}

    Task& operator=( Task&& other ) noexcept
    {
        if ( this != &other )
        {
            if ( m_Handle )
                m_Handle.destroy();
            m_Handle = std::exchange( other.m_Handle, nullptr );
        }
        return *this;
    }

    /// Returns true if the coroutine has not yet finished.
    explicit operator bool() const { return m_Handle && !m_Handle.done(); }

    /// Resume the coroutine. Does nothing if already finished.
    void resume()
    {
        if ( m_Handle && !m_Handle.done() )
            m_Handle.resume();
    }

private:
    std::coroutine_handle<promise_type> m_Handle;
};

}  // namespace graphics
}  // namespace sr
