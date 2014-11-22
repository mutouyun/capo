/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_UTILITY_COUNTOF_HPP___
#define CAPO_UTILITY_COUNTOF_HPP___

#include <cstddef>  // size_t

namespace capo
{
    template <typename T, size_t N>
    constexpr size_t countof(const T(&)[N]) { return N; }
}

#endif // CAPO_UTILITY_COUNTOF_HPP___
