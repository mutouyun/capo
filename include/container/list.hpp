/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_LIST_HPP___
#define CAPO_CONTAINER_LIST_HPP___

#include "../memory/allocator.hpp"

#include <list> // std::list

namespace capo
{
    template <typename T, class AllocatorT = capo::allocator<T>>
    using list = std::list<T, AllocatorT>;
}

#endif // CAPO_CONTAINER_LIST_HPP___
