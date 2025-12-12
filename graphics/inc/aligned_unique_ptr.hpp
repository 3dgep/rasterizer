#pragma once

#include <memory>
#include <new>  // For std::bad_alloc
#include <type_traits>

#ifdef _MSC_VER
    #define aligned_malloc( size, alignment ) _aligned_malloc( ( size ), ( alignment ) )
    #define aligned_free                      _aligned_free
#else
    #include <cstdlib>
    #define aligned_malloc( size, alignment ) std::aligned_alloc( ( alignment ), ( size ) )
    #define aligned_free                      std::free
#endif

template<typename T>
struct aligned_deleter
{
    using T2 = std::remove_extent_t<T>;
    size_t n = 1;

    void operator()( T2* p ) const
    {
        for ( std::size_t i = 0; i < n; ++i )
            p[i].~T2();

        aligned_free( p );
    }
};

template<typename T>
using aligned_unique_ptr = std::unique_ptr<T, aligned_deleter<T>>;

namespace detail
{
template<typename>
constexpr bool is_unbounded_array_v = false;

template<typename T>
constexpr bool is_unbounded_array_v<T[]> = true;

template<typename>
constexpr bool is_bounded_array_v = false;

template<typename T, std::size_t N>
constexpr bool is_bounded_array_v<T[N]> = true;
}  // namespace detail

class bad_aligned_alloc : public std::bad_alloc
{
public:
    const char* what() const noexcept override
    {
        return "aligned memory allocation failed";
    }
};

template<typename T, std::size_t Align, typename... Args>
std::enable_if_t<!std::is_array_v<T>, aligned_unique_ptr<T>>
    make_aligned_unique( Args&&... args )
{
    static_assert( ( Align & ( Align - 1 ) ) == 0 );  // Alignment must be a power of 2.
    static_assert( Align >= alignof( T ) );           // Make sure alignment is at least as big as the alignment requirement of the type.

    aligned_unique_ptr<T> ptr = aligned_unique_ptr<T>( static_cast<T*>( aligned_malloc( sizeof( T ), Align ) ), aligned_deleter<T> {} );

    if ( !ptr )
        throw bad_aligned_alloc();

    try
    {
        new ( ptr.get() ) T( std::forward<Args>( args )... );  // The constructor might throw.
    }
    catch ( ... )
    {
        // Free the memory before propagating the exception.
        aligned_free( ptr );
        // Propagate the exception.
        throw;
    }
    return ptr;
}

template<typename T, std::size_t Align>
std::enable_if_t<detail::is_unbounded_array_v<T> && std::is_default_constructible_v<std::remove_extent_t<T>>, aligned_unique_ptr<T>>
    make_aligned_unique( std::size_t n )
{
    using T2 = std::remove_extent_t<T>;

    static_assert( ( Align & ( Align - 1 ) ) == 0 );  // Alignment must be a power of 2.
    static_assert( Align >= alignof( T2 ) );          // Make sure alignment is at least as big as the alignment requirement of the element type.

    aligned_unique_ptr<T> ptr = aligned_unique_ptr<T>( static_cast<T2*>( aligned_malloc( sizeof( T2 ) * n, Align ) ), aligned_deleter<T> { n } );

    if ( !ptr )
        throw bad_aligned_alloc();

    // Default construct the elements.
    T2* p = ptr.get();
    for ( std::size_t i = 0; i < n; ++i )
    {
        try
        {
            new ( &p[i] ) T2();
        }
        catch ( ... )
        {
            // Deconstruct the already constructed elements.
            for ( std::size_t j = 0; j < i; ++j )
                p[i].~T2();

            // Free the allocation.
            aligned_free( p );
            // Propagate the exception.
            throw;
        }
    }

    return ptr;
}

template<typename T, typename... Args>
std::enable_if_t<detail::is_bounded_array_v<T>> make_aligned_unique( Args&&... ) = delete;