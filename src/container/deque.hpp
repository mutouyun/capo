/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_DEQUE_HPP___
#define CAPO_CONTAINER_DEQUE_HPP___

#include "../memory/allocator.hpp"

#include <deque> // std::deque

namespace capo
{
    template <typename T, class AllocatorT = capo::allocator<T>>
    using deque = std::deque<T, AllocatorT>;
}

#endif // CAPO_CONTAINER_DEQUE_HPP___
