/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

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
        static T* construct(T* p, P&&... args)
        {
            return ::new (p) T(std::forward<P>(args)...);
        }
    };

    template <typename T, size_t N>
    struct impl<T[N]>
    {
        using type = T[N];
        template <typename... P>
        static type* construct(type* p, P&&... args)
        {
            for (size_t i = 0; i < N; ++i)
                impl<T>::construct((*p) + i, std::forward<P>(args)...);
            return p;
        }
    };
}

template <typename T, typename... P>
T* construct(T* p, P&&... args)
{
    return detail_construct::impl<T>::construct(p, std::forward<P>(args)...);
}

template <typename T, typename... P>
T* construct(void* p, P&&... args)
{
    return construct(static_cast<T*>(p), std::forward<P>(args)...);
}

////////////////////////////////////////////////////////////////
/// Destruct an object, but not free the memory
////////////////////////////////////////////////////////////////

namespace detail_destruct
{
    template <typename T>
    struct impl
    {
        static void destruct(T* p)
        {
            reinterpret_cast<T*>(p)->~T();
        }
    };

    template <typename T, size_t N>
    struct impl<T[N]>
    {
        using type = T[N];
        static void destruct(type* p)
        {
            for (size_t i = 0; i < N; ++i)
                impl<T>::destruct((*p) + i);
        }
    };
}

template <typename T>
void destruct(T* p)
{
    return detail_destruct::impl<T>::destruct(p);
}

template <typename T>
void destruct(void* p)
{
    destruct(static_cast<T*>(p));
}

} // namespace capo
