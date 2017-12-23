/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/allocator.hpp"

#include <deque> // std::deque

namespace capo
{
    template <typename T, class AllocatorT = CAPO_ALLOCATOR_<T>>
    using deque = std::deque<T, AllocatorT>;
}
