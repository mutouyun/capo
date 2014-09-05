/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MEMORY_ALLOCATOR_WRAPPER_HPP___
#define CAPO_MEMORY_ALLOCATOR_WRAPPER_HPP___

#include "../memory/construct.hpp"

#include <utility>  // std::move, std::forward
#include <new>      // std::bad_alloc
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// The wrapper class for capo's allocator
////////////////////////////////////////////////////////////////

template <typename T, class AllocT>
class allocator_wrapper
{
    template <typename U, class AllocU>
    friend class allocator_wrapper;

    AllocT alloc_;

public:
    // type definitions
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;
    typedef AllocT            alloc_type;

public:
    allocator_wrapper(void) noexcept {}

    allocator_wrapper(const allocator_wrapper<T, AllocT>& rhs) noexcept
        : alloc_(rhs.alloc_)
    {}
    template <typename U>
    allocator_wrapper(const allocator_wrapper<U, AllocT>& rhs) noexcept
        : alloc_(rhs.alloc_)
    {}

    allocator_wrapper(allocator_wrapper<T, AllocT>&& rhs) noexcept
        : alloc_(std::move(rhs.alloc_))
    {}
    template <typename U>
    allocator_wrapper(allocator_wrapper<U, AllocT>&& rhs) noexcept
        : alloc_(std::move(rhs.alloc_))
    {}

    allocator_wrapper(const AllocT& rhs) noexcept
        : alloc_(rhs)
    {}
    allocator_wrapper(AllocT&& rhs) noexcept
        : alloc_(std::move(rhs))
    {}

public:
    // the other type of std_allocator
    template <typename U>
    struct rebind { typedef allocator_wrapper<U, AllocT> other; };

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

template <class AllocT>
class allocator_wrapper<void, AllocT>
{
public:
    typedef void    value_type;
    typedef AllocT  alloc_type;
};

#if defined(_MSC_VER) && (_MSC_VER <= 1900)
/*
    <MSVC 2013> compile error C3757
    'const capo::allocator_wrapper<T, AllocT> &': type not allowed for 'constexpr' function.
*/
template <typename T, typename U, class AllocT>
inline bool operator==(const allocator_wrapper<T, AllocT>&, const allocator_wrapper<U, AllocT>&) noexcept
{ return true; }
template <typename T, typename U, class AllocT>
inline bool operator!=(const allocator_wrapper<T, AllocT>&, const allocator_wrapper<U, AllocT>&) noexcept
{ return false; }
#else /*!_MSC_VER*/
template <typename T, typename U, class AllocT>
constexpr bool operator==(const allocator_wrapper<T, AllocT>&, const allocator_wrapper<U, AllocT>&) noexcept
{ return true; }
template <typename T, typename U, class AllocT>
constexpr bool operator!=(const allocator_wrapper<T, AllocT>&, const allocator_wrapper<U, AllocT>&) noexcept
{ return false; }
#endif/*!_MSC_VER*/

} // namespace capo

#endif // CAPO_MEMORY_ALLOCATOR_WRAPPER_HPP___
