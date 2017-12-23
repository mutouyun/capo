/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/deque.hpp"

#include <stack> // std::stack

namespace capo
{
    template <typename T, typename SeqT = capo::deque<T> >
    using stack = std::stack<T, SeqT>;
}
