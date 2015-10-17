/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/allocator.hpp"

#include <unordered_map> // std::unordered_map

namespace capo
{
    template <typename KeyT, typename TypeT, class HashT = std::hash<KeyT>, class PredT = std::equal_to<KeyT>
                                           , class AllocatorT = CAPO_ALLOCATOR_<std::pair<const KeyT, TypeT>>>
    using hash_map = std::unordered_map<KeyT, TypeT, HashT, PredT, AllocatorT>;

    template <typename KeyT, typename TypeT, class HashT = std::hash<KeyT>, class PredT = std::equal_to<KeyT>
                                           , class AllocatorT = CAPO_ALLOCATOR_<std::pair<const KeyT, TypeT>>>
    using hash_multimap = std::unordered_multimap<KeyT, TypeT, HashT, PredT, AllocatorT>;
}
