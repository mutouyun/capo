/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include <algorithm>    // std::max, std::min
#include <type_traits>  // std::integral_constant

namespace capo {

////////////////////////////////////////////////////////////////

/*
    Return the largest
*/

template <typename T>
constexpr const T& max(const T& x, const T& y)
{
    return std::max(x, y);
}

template <typename T, typename... P>
constexpr const T& max(const T& x, const T& y, const P&... args)
{
    return capo::max(x, capo::max(y, args...));
}

template <int X, int... Y>
struct max_number
     : max_number<X, max_number<Y...>::value>
{};

template <int X, int Y>
struct max_number<X, Y>
     : std::integral_constant<int, (X < Y) ? Y : X>
{};

/*
    Return the smallest
*/

template <typename T>
constexpr const T& min(const T& x, const T& y)
{
    return std::min(x, y);
}

template <typename T, typename... P>
constexpr const T& min(const T& x, const T& y, const P&... args)
{
    return capo::min(x, capo::min(y, args...));
}

template <int X, int... Y>
struct min_number
     : min_number<X, min_number<Y...>::value>
{};

template <int X, int Y>
struct min_number<X, Y>
     : std::integral_constant<int, (Y < X) ? Y : X>
{};

////////////////////////////////////////////////////////////////

} // namespace capo
