/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include <cstddef>  // std::size_t

namespace capo {

////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
constexpr std::size_t countof(const T(&)[N]) { return N; }

////////////////////////////////////////////////////////////////

} // namespace capo
