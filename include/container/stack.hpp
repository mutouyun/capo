/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_CONTAINER_STACK_HPP___
#define CAPO_CONTAINER_STACK_HPP___

#include "../memory/allocator.hpp"
#include "../container/deque.hpp"

#include <stack> // std::stack

namespace capo
{
    template <typename T, typename SeqT = capo::deque<T> >
    using stack = std::stack<T, SeqT>;
}

#endif // CAPO_CONTAINER_STACK_HPP___
