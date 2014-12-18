/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/concept.hpp"
#include "capo/force_inline.hpp"
#include "capo/allocator.hpp"

#include <type_traits>  // std::enable_if
#include <cstddef>      // std::size_t

namespace capo {

#if !defined(CAPO_ALLOCATOR_)
#   define CAPO_ALLOCATOR_ capo::allocator
#endif/*!CAPO_ALLOCATOR_*/

namespace detail_alloc {

////////////////////////////////////////////////////////////////

CAPO_CONCEPT_MEMBER_FUNCTION_(size_of, std::size_t(C::*)(void) const);

////////////////////////////////////////////////////////////////

template <template <typename> class AllocT, typename T>
struct impl
{
    using alloc_type = AllocT<T>;

    template <typename... P>
    static typename alloc_type::pointer alloc(P&&... args)
    {
        alloc_type aa;
        try
        {
            typename alloc_type::pointer p = aa.allocate(1);
            aa.construct(p, std::forward<P>(args)...);
            return p;
        }
        catch(const std::bad_alloc&)
        {
            return nullptr;
        }
    }

    template <typename U>
    static typename std::enable_if<!std::has_virtual_destructor<U>::value
                                || !has_size_of<U>::value
    >::type free(U* p)
    {
        if (p == nullptr) return;
        typename alloc_type::template rebind<U>::other aa;
        aa.destroy(p);
        aa.deallocate(p, 1);
    }

    template <typename U>
    static typename std::enable_if<std::has_virtual_destructor<U>::value
                                && has_size_of<U>::value
    >::type free(U* p)
    {
        if (p == nullptr) return;
        typename alloc_type::template rebind<char>::other aa;
        std::size_t s = p->size_of();
        aa.destroy(p);
        aa.deallocate(p, s);
    }
};

/* Make array to an array pointer */
template <template <typename> class AllocT, typename T, std::size_t N>
struct impl<AllocT, T[N]>
{
    using U = T[N];
    using alloc_type = AllocT<U>;

    template <typename... P>
    static typename alloc_type::pointer alloc(P&&... args)
    {
        alloc_type aa;
        try
        {
            typename alloc_type::pointer p = aa.allocate(1);
            for (std::size_t i = 0; i < N; ++i)
                aa.construct((*p) + i, std::forward<P>(args)...);
            return p;
        }
        catch(const std::bad_alloc&) return nullptr;
    }

    static void free(typename alloc_type::pointer p)
    {
        if (p == nullptr) return;
        alloc_type aa;
        for (std::size_t i = 0; i < N; ++i)
            aa.destroy((*p) + i);
        aa.deallocate(p, 1);
    }
};

template <template <typename> class AllocT>
struct impl<AllocT, void>
{
    using alloc_type = AllocT<char>;

    static void* alloc(std::size_t size)
    {
        if (size)
        {
            alloc_type aa;
            try
            {
                return aa.allocate(size);
            }
            catch(const std::bad_alloc&)
            {
                return nullptr;
            }
        }
        else return nullptr;
    }

    static void free(void* p, std::size_t size)
    {
        if (p == nullptr || !size) return;
        alloc_type aa;
        aa.deallocate((typename alloc_type::pointer)p, size);
    }
};

} // namespace detail_alloc

////////////////////////////////////////////////////////////////
/// Construct alloc
////////////////////////////////////////////////////////////////

template <template <typename> class AllocT>
CAPO_FORCE_INLINE_ void* alloc(std::size_t size)
{
    return detail_alloc::impl<AllocT, void>::alloc(size);
}

template <template <typename> class AllocT, typename T, typename... P>
CAPO_FORCE_INLINE_ T* alloc(P&&... args)
{
    return detail_alloc::impl<AllocT, T>::alloc(std::forward<P>(args)...);
}

CAPO_FORCE_INLINE_ void* alloc(std::size_t size)
{
    return alloc<CAPO_ALLOCATOR_>(size);
}

template <typename T, typename... P>
CAPO_FORCE_INLINE_ T* alloc(P&&... args)
{
    return alloc<CAPO_ALLOCATOR_, T>(std::forward<P>(args)...);
}

////////////////////////////////////////////////////////////////
/// Destruct free
////////////////////////////////////////////////////////////////

template <template <typename> class AllocT>
CAPO_FORCE_INLINE_ void free(void* p, size_t size)
{
    detail_alloc::impl<AllocT, void>::free(p, size);
}

template <template <typename> class AllocT, typename T>
CAPO_FORCE_INLINE_ void free(T* p)
{
    detail_alloc::impl<AllocT, T>::free(p);
}

CAPO_FORCE_INLINE_ void free(void* p, size_t size)
{
    free<CAPO_ALLOCATOR_>(p, size);
}

template <typename T>
CAPO_FORCE_INLINE_ void free(T* p)
{
    free<CAPO_ALLOCATOR_, T>(p);
}

////////////////////////////////////////////////////////////////

} // namespace capo
