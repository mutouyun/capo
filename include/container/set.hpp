/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_SET_HPP___
#define CAPO_CONTAINER_SET_HPP___

#include "../memory/allocator.hpp"

#include <set> // std::set

namespace capo
{
    template <typename T, class CompT = std::less<T>
                        , class AllocatorT = capo::allocator<T>>
    using set = std::set<T, CompT, AllocatorT>;

    template <typename T, class CompT = std::less<T>
                        , class AllocatorT = capo::allocator<T>>
    using multiset = std::multiset<T, CompT, AllocatorT>;
}

#endif // CAPO_CONTAINER_SET_HPP___
