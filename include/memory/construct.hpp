/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MEMORY_CONSTRUCT_HPP___
#define CAPO_MEMORY_CONSTRUCT_HPP___

#include <new>      // placement new
#include <utility>  // std::forward
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Construct an object, just like placement new
////////////////////////////////////////////////////////////////

namespace detail_construct
{
    template <typename T>
    struct impl
    {
        template <typename... P>
        static T* construct(void* p, P&&... args)
        {
            return ::new (p) T(std::forward<P>(args)...);
        }
    };

    template <typename T, size_t N>
    struct impl<T[N]>
    {
        using type = T[N];
        template <typename... P>
        static type* construct(void* p, P&&... args)
        {
            type* r = reinterpret_cast<type*>(p);
            for (size_t i = 0; i < N; ++i)
                impl<T>::construct((*r) + i, std::forward<P>(args)...);
            return r;
        }
    };
}

template <typename T, typename... P>
T* construct(void* p, P&&... args)
{
    return detail_construct::impl<T>::construct(p, std::forward<P>(args)...);
}

////////////////////////////////////////////////////////////////
/// Destruct an object, but not free the memory
////////////////////////////////////////////////////////////////

namespace detail_destruct
{
    template <typename T>
    struct impl
    {
        static void destruct(void* p)
        {
            reinterpret_cast<T*>(p)->~T();
        }
    };

    template <typename T, size_t N>
    struct impl<T[N]>
    {
        using type = T[N];
        static void destruct(void* p)
        {
            type* r = reinterpret_cast<type*>(p);
            for (size_t i = 0; i < N; ++i)
                impl<T>::destruct((*r) + i);
        }
    };
}

template <typename T>
void destruct(void* p)
{
    return detail_destruct::impl<T>::destruct(p);
}

} // namespace capo

#endif // CAPO_MEMORY_CONSTRUCT_HPP___
