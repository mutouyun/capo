/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_VECTOR_HPP___
#define CAPO_CONTAINER_VECTOR_HPP___

#include "../memory/allocator.hpp"

#include <vector> // std::vector

namespace capo
{
    template <typename T, class AllocatorT = capo::allocator<T>>
    using vector = std::vector<T, AllocatorT>;
}

#endif // CAPO_CONTAINER_VECTOR_HPP___
