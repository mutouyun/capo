/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/allocator.hpp"

#include <vector> // std::vector

namespace capo
{
    template <typename T, class AllocatorT = CAPO_ALLOCATOR_<T>>
    using vector = std::vector<T, AllocatorT>;
}
