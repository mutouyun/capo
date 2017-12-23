/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/allocator.hpp"

#include <unordered_set> // std::unordered_set

namespace capo
{
    template <typename T, class HashT = std::hash<T>, class PredT = std::equal_to<T>
                        , class AllocatorT = CAPO_ALLOCATOR_<T>>
    using hash_set = std::unordered_set<T, HashT, PredT, AllocatorT>;

    template <typename T, class HashT = std::hash<T>, class PredT = std::equal_to<T>
                        , class AllocatorT = CAPO_ALLOCATOR_<T>>
    using hash_multiset = std::unordered_multiset<T, HashT, PredT, AllocatorT>;
}
