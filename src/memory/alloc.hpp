/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MEMORY_ALLOC_HPP___
#define CAPO_MEMORY_ALLOC_HPP___

#include "../memory/construct.hpp"
#include "../memory/alloc_malloc.hpp"
#include "../type/type_concept.hpp"

#include <utility>      // std::forward
#include <type_traits>  // std::has_virtual_destructor, std::enable_if, ...
#include <cstddef>      // size_t

namespace capo {

#ifndef CAPO_DEFAULT_ALLOC_
#define CAPO_DEFAULT_ALLOC_ capo::use::alloc_malloc
#endif

////////////////////////////////////////////////////////////////
/// Allocate block of storage
////////////////////////////////////////////////////////////////

template <class AllocT = CAPO_DEFAULT_ALLOC_>
inline void* alloc(size_t size)
{
    return AllocT::alloc(size);
}

template <typename T, class AllocT = CAPO_DEFAULT_ALLOC_, typename... P>
inline T* alloc(P&&... args)
{
    return capo::construct<T>(AllocT::alloc(sizeof(T)), std::forward<P>(args)...);
}

////////////////////////////////////////////////////////////////
/// Release block of storage
////////////////////////////////////////////////////////////////

namespace detail_free
{
    namespace memf_n
    {
        CAPO_CONCEPT_MEMBER_FUNCTION(size_of, size_t(C::*)(void));
    }
    namespace memf_c
    {
        CAPO_CONCEPT_MEMBER_FUNCTION(size_of, size_t(C::*)(void) const);
    }

    /*
        <MSVC 2013> fatal error C1001
        With: namespace detail_free { using has_concept = ...; }
        See : http://connect.microsoft.com/VisualStudio/feedback/details/890917/
    */
    template <typename T>
    struct has_concept : std::integral_constant<bool, std::has_virtual_destructor<T>::value &&
                                                     (memf_n::has_size_of<T>::value || 
                                                      memf_c::has_size_of<T>::value)> {};

    // has virtual-destructor and size_of function

    template <typename T>
    inline auto destruct(T* p)
        -> typename std::enable_if<detail_free::has_concept<T>::value, size_t>::type
    {
        size_t size = 0;
        if (p)
        {
            size = p->size_of();
            capo::destruct<T>(p);
        }
        return size;
    }

    // has no virtual-destructor or size_of function

    template <typename T>
    inline auto destruct(T* p)
        -> typename std::enable_if<!detail_free::has_concept<T>::value, size_t>::type
    {
        size_t size = 0;
        if (p)
        {
            size = sizeof(T);
            capo::destruct<T>(p);
        }
        return size;
    }
}

template <class AllocT = CAPO_DEFAULT_ALLOC_>
inline void free(void* p, size_t size)
{
    AllocT::free(p, size);
}

template <class AllocT = CAPO_DEFAULT_ALLOC_, typename T>
inline void free(T* p)
{
    AllocT::free(p, detail_free::destruct(p));
}

////////////////////////////////////////////////////////////////
/// Reallocate memory block
////////////////////////////////////////////////////////////////

template <class AllocT = CAPO_DEFAULT_ALLOC_>
inline void* realloc(void* p, size_t old_size, size_t new_size)
{
    return AllocT::realloc(p, old_size, new_size);
}

template <class AllocT = CAPO_DEFAULT_ALLOC_, typename U>
inline void* realloc(U* p, size_t new_size)
{
    return AllocT::realloc(p, detail_free::destruct(p), new_size);
}

template <typename T, class AllocT = CAPO_DEFAULT_ALLOC_, typename... P>
inline T* realloc(void* p, size_t old_size, P&&... args)
{
    return capo::construct<T>(AllocT::realloc(p, old_size, sizeof(T)), 
                              std::forward<P>(args)...);
}

template <typename T, class AllocT = CAPO_DEFAULT_ALLOC_, typename U, typename... P>
inline T* realloc(U* p, P&&... args)
{
    return capo::construct<T>(AllocT::realloc(p, detail_free::destruct(p), sizeof(T)), 
                              std::forward<P>(args)...);
}

////////////////////////////////////////////////////////////////
/// Define macros
////////////////////////////////////////////////////////////////

#ifndef capo_alloc
#define capo_alloc    capo::alloc
#endif

#ifndef capo_free
#define capo_free     capo::free
#endif

#ifndef capo_realloc
#define capo_realloc  capo::realloc
#endif

} // namespace capo

#endif // CAPO_MEMORY_ALLOC_HPP___
