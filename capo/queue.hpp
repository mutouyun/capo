/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/deque.hpp"
#include "capo/vector.hpp"

#include <queue> // std::queue

namespace capo
{
    template <typename T, typename SeqT = capo::deque<T> >
    using queue = std::queue<T, SeqT>;

    template <typename T, typename SeqT = capo::vector<T>,
              typename CompT = std::less<typename SeqT::value_type>>
    using priority_queue = std::priority_queue<T, SeqT, CompT>;
}
