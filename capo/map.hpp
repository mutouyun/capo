/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/allocator.hpp"

#include <map> // std::map

namespace capo
{
    template <typename KeyT, typename TypeT, class CompT = std::less<KeyT>
                                           , class AllocatorT = CAPO_ALLOCATOR_<std::pair<const KeyT, TypeT>>>
    using map = std::map<KeyT, TypeT, CompT, AllocatorT>;

    template <typename KeyT, typename TypeT, class CompT = std::less<KeyT>
                                           , class AllocatorT = CAPO_ALLOCATOR_<std::pair<const KeyT, TypeT>>>
    using multimap = std::multimap<KeyT, TypeT, CompT, AllocatorT>;
}
