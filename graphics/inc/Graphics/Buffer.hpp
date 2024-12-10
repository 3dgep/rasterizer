#pragma once

#include <aligned_unique_ptr.hpp>

#include <cassert>
#include <cstddef>

namespace sr
{
inline namespace graphics
{
template<typename T, std::size_t Alignment = 64>
struct Buffer
{
    Buffer()  = default;
    ~Buffer() = default;

    Buffer( std::size_t size );

    Buffer( const Buffer& copy );
    Buffer( Buffer&& move ) noexcept;

    Buffer& operator=( const Buffer& rhs );
    Buffer& operator=( Buffer&& rhs ) noexcept;

    const T& operator[]( std::size_t i ) const;
    T&       operator[]( std::size_t i );

    const T* data() const;
    T*       data();

    void resize( std::size_t size );

    std::size_t getSize() const noexcept;

    /// <summary>
    /// Clear the buffer contents to a specific value.
    /// </summary>
    /// <param name="v">The value to clear the buffer to.</param>
    void clear( const T& v );

private:
    std::size_t             m_size = 0u;
    aligned_unique_ptr<T[]> m_data;
};

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>::Buffer( std::size_t size )
{
    resize( size );
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>::Buffer( const Buffer& copy )
{
    resize( copy.m_size );
    std::memcpy( data(), copy.data(), m_size * sizeof( T ) );
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>::Buffer( Buffer&& move ) noexcept
: m_size { move.m_size }
, m_data { std::move( move.m_data ) }
{
    move.m_size = 0u;
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>& Buffer<T, Alignment>::operator=( const Buffer& rhs )
{
    resize( rhs.m_size );
    std::memcpy( data(), rhs.data(), m_size * sizeof( T ) );

    return *this;
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>& Buffer<T, Alignment>::operator=( Buffer&& rhs ) noexcept
{
    m_size = rhs.m_size;
    m_data = std::move( rhs.m_data );

    rhs.m_size = 0;

    return *this;
}

template<typename T, std::size_t Alignment>
const T& Buffer<T, Alignment>::operator[]( std::size_t i ) const
{
    assert( i < m_size );
    return m_data[i];
}

template<typename T, std::size_t Alignment>
T& Buffer<T, Alignment>::operator[]( std::size_t i )
{
    assert( i < m_size );
    return m_data[i];
}

template<typename T, std::size_t Alignment>
const T* Buffer<T, Alignment>::data() const
{
    return m_data.get();
}

template<typename T, std::size_t Alignment>
T* Buffer<T, Alignment>::data()
{
    return m_data.get();
}

template<typename T, std::size_t Alignment>
void Buffer<T, Alignment>::resize( std::size_t size )
{
    if ( m_size != size )
    {
        m_data = make_aligned_unique<T[], Alignment>( size );

        m_size = size;
    }
}

template<typename T, std::size_t Alignment>
std::size_t Buffer<T, Alignment>::getSize() const noexcept
{
    return m_size;
}

template<typename T, std::size_t Alignment>
void Buffer<T, Alignment>::clear( const T& v )
{
    T* p = data();

#pragma omp parallel for
    for ( int i = 0; i < static_cast<int>( m_size ); ++i )
        p[i] = v;
}

}  // namespace graphics
}  // namespace sr
