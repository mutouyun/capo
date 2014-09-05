/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MEMORY_ALLOCATOR_HPP___
#define CAPO_MEMORY_ALLOCATOR_HPP___

#include "../memory/alloc.hpp"

#include <new>      // std::bad_alloc
#include <cstddef>  // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// The wrapper class for capo's allocator
////////////////////////////////////////////////////////////////

template <typename T, class AllocT = CAPO_DEFAULT_ALLOC_>
class allocator
{
public:
    // type definitions
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;

public:
    allocator(void) noexcept {}
    allocator(const allocator<T, AllocT>&) noexcept {}
    template <class U>
    allocator(const allocator<U, AllocT>&) noexcept {}

    size_type max_size(void) const noexcept
    { return ((size_type)-1 / sizeof(T)); }

public:
    pointer allocate(size_type count)
    {
        void* p = nullptr;
        if (count == 0) /* Do nothing */ ;
        else
        if ( (count > this->max_size()) || !(p = AllocT::alloc(count * sizeof(T))) )
            throw std::bad_alloc(); // Report no memory
        return static_cast<pointer>(p);
    }

    void deallocate(pointer p, size_type count)
    {
        AllocT::free(p, count * sizeof(T));
    }
};

template <class AllocT>
class allocator<void, AllocT>
{
public:
    typedef void value_type;
};

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
/*
    <MSVC 2013> compile error C3757
    'const capo::allocator<T, AllocT> &': type not allowed for 'constexpr' function.
*/
template <typename T, typename U, class AllocT>
inline bool operator==(const allocator<T, AllocT>&, const allocator<U, AllocT>&) noexcept
{ return true; }
template <typename T, typename U, class AllocT>
inline bool operator!=(const allocator<T, AllocT>&, const allocator<U, AllocT>&) noexcept
{ return false; }
#else /*_MSC_VER*/
template <typename T, typename U, class AllocT>
constexpr bool operator==(const allocator<T, AllocT>&, const allocator<U, AllocT>&) noexcept
{ return true; }
template <typename T, typename U, class AllocT>
constexpr bool operator!=(const allocator<T, AllocT>&, const allocator<U, AllocT>&) noexcept
{ return false; }
#endif/*_MSC_VER*/

} // namespace capo

#endif // CAPO_MEMORY_ALLOCATOR_HPP___
