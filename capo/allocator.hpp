/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/construct.hpp"
#include "capo/standard_alloc.hpp"

#include <utility>  // std::move, std::forward
#include <new>      // std::bad_alloc
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// The wrapper class for capo's allocator
////////////////////////////////////////////////////////////////

template <typename T, class AllocP>
class allocator_wrapper
{
    template <typename U, class AllocU>
    friend class allocator_wrapper;

public:
    // type definitions
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;
    typedef AllocP            alloc_policy;

private:
    alloc_policy alloc_;

public:
    allocator_wrapper(void) noexcept {}

    allocator_wrapper(const allocator_wrapper<T, AllocP>& rhs) noexcept
        : alloc_(rhs.alloc_)
    {}
    template <typename U>
    allocator_wrapper(const allocator_wrapper<U, AllocP>& rhs) noexcept
        : alloc_(rhs.alloc_)
    {}

    allocator_wrapper(allocator_wrapper<T, AllocP>&& rhs) noexcept
        : alloc_(std::move(rhs.alloc_))
    {}
    template <typename U>
    allocator_wrapper(allocator_wrapper<U, AllocP>&& rhs) noexcept
        : alloc_(std::move(rhs.alloc_))
    {}

    allocator_wrapper(const AllocP& rhs) noexcept
        : alloc_(rhs)
    {}
    allocator_wrapper(AllocP&& rhs) noexcept
        : alloc_(std::move(rhs))
    {}

public:
    // the other type of std_allocator
    template <typename U>
    struct rebind { typedef allocator_wrapper<U, AllocP> other; };

    size_type max_size(void) const noexcept
    { return (static_cast<size_type>(-1) / sizeof(T)); }

public:
    pointer allocate(size_type count)
    {
        void* p = nullptr;
        if (count == 0) /* Do nothing */ ;
        else
        if ( (count > this->max_size()) || (p = alloc_.alloc(count * sizeof(T))) == nullptr )
            throw std::bad_alloc(); // Report no memory
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type count)
    {
        alloc_.free(p, count * sizeof(T));
    }

    template <typename... P>
    static void construct(pointer p, P&&... args)
    {
        capo::construct<value_type>(p, std::forward<P>(args)...);
    }

    static void destroy(pointer p)
    {
        capo::destruct<value_type>(p);
    }
};

template <class AllocP>
class allocator_wrapper<void, AllocP>
{
public:
    typedef void    value_type;
    typedef AllocP  alloc_policy;
};

template <typename T, typename U, class AllocP>
constexpr bool operator==(const allocator_wrapper<T, AllocP>&, const allocator_wrapper<U, AllocP>&) noexcept
{
    return true;
}

template <typename T, typename U, class AllocP>
constexpr bool operator!=(const allocator_wrapper<T, AllocP>&, const allocator_wrapper<U, AllocP>&) noexcept
{
    return false;
}

////////////////////////////////////////////////////////////////
/// The capo's allocator
////////////////////////////////////////////////////////////////

#if !defined(CAPO_ALLOCATOR_POLICY_)
#   define CAPO_ALLOCATOR_POLICY_ capo::use::alloc_malloc
#endif/*!CAPO_ALLOCATOR_POLICY_*/

template <typename T>
using allocator = allocator_wrapper<T, CAPO_ALLOCATOR_POLICY_>;

#if !defined(CAPO_ALLOCATOR_)
#   define CAPO_ALLOCATOR_ capo::allocator
#endif/*!CAPO_ALLOCATOR_*/

} // namespace capo
