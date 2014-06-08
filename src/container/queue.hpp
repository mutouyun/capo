/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_QUEUE_HPP___
#define CAPO_CONTAINER_QUEUE_HPP___

#include "../memory/allocator.hpp"
#include "../container/deque.hpp"
#include "../container/vector.hpp"

#include <queue> // std::queue

namespace capo
{
    template <typename T, typename SeqT = capo::deque<T> >
    using queue = std::queue<T, SeqT>;

    template <typename T, typename SeqT = capo::vector<T>,
              typename CompT = std::less<typename SeqT::value_type>>
    using priority_queue = std::priority_queue<T, SeqT, CompT>;
}

#endif // CAPO_CONTAINER_QUEUE_HPP___
