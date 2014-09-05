/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_ALGORITHM_MAX_MIN_HPP___
#define CAPO_ALGORITHM_MAX_MIN_HPP___

#include <algorithm>    // std::max, std::min

namespace capo {

////////////////////////////////////////////////////////////////

/*
    Return the largest
*/

template <typename T>
inline const T& max(const T& x, const T& y)
{
    return std::max(x, y);
}

template <typename T, typename... P>
inline const T& max(const T& x, const T& y, const P&... args)
{
    return capo::max(x, capo::max(y, args...));
}

/*
    Return the smallest
*/

template <typename T>
inline const T& min(const T& x, const T& y)
{
    return std::min(x, y);
}

template <typename T, typename... P>
inline const T& min(const T& x, const T& y, const P&... args)
{
    return capo::min(x, capo::min(y, args...));
}

////////////////////////////////////////////////////////////////

} // namespace capo

#endif // CAPO_ALGORITHM_MAX_MIN_HPP___
