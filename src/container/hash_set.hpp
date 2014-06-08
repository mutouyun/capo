/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_HASH_SET_HPP___
#define CAPO_CONTAINER_HASH_SET_HPP___

#include "../memory/allocator.hpp"

#include <unordered_set> // std::unordered_set

namespace capo
{
    template <typename T, class HashT = std::hash<T>, class PredT = std::equal_to<T>
                        , class AllocatorT = capo::allocator<T>>
    using hash_set = std::unordered_set<T, HashT, PredT, AllocatorT>;

    template <typename T, class HashT = std::hash<T>, class PredT = std::equal_to<T>
                        , class AllocatorT = capo::allocator<T>>
    using hash_multiset = std::unordered_multiset<T, HashT, PredT, AllocatorT>;
}

#endif // CAPO_CONTAINER_HASH_SET_HPP___
