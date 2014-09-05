/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_MAP_HPP___
#define CAPO_CONTAINER_MAP_HPP___

#include "../memory/allocator.hpp"

#include <map> // std::map

namespace capo
{
    template <typename KeyT, typename TypeT, class CompT = std::less<KeyT>
                                           , class AllocatorT = capo::allocator<std::pair<const KeyT, TypeT>>>
    using map = std::map<KeyT, TypeT, CompT, AllocatorT>;

    template <typename KeyT, typename TypeT, class CompT = std::less<KeyT>
                                           , class AllocatorT = capo::allocator<std::pair<const KeyT, TypeT>>>
    using multimap = std::multimap<KeyT, TypeT, CompT, AllocatorT>;
}

#endif // CAPO_CONTAINER_MAP_HPP___
