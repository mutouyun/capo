/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/alloc.hpp"

#include <forward_list> // std::forward_list

namespace capo
{
    template <typename T, class AllocatorT = CAPO_ALLOCATOR_<T>>
    using forward_list = std::forward_list<T, AllocatorT>;
}
