/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/sequence.hpp"
#include "capo/assert.hpp"

#include <type_traits>  // std::common_type
#include <stdexcept>    // std::logic_error
#include <utility>      // std::forward

namespace capo {
namespace detail_range {

struct arithmetic_range
{
    enum { StateSize = 2 };

    template <typename T> static void next(T& a, T& d) { a += d; }
    template <typename T> static void prev(T& a, T& d) { a -= d; }

    template <typename T> static void at(T& a, T& d, size_t n)
    {
        a += static_cast<T>(d * n);
    }
};

////////////////////////////////////////////////////////////////
/// The impl class
////////////////////////////////////////////////////////////////

template <typename T>
class impl : public decltype(capo::sequence<arithmetic_range, T>(0, 0))
{
    using base_t = decltype(capo::sequence<arithmetic_range, T>(0, 0));

public:
    using size_type = typename base_t::size_type;

private:
    template <typename T_, typename U_>
    size_type get_adjusted_count(T_&& begin_val, T_&& end_val, U_&& step_val) const
    {
        if (step_val > 0)
        {
            CAPO_ENSURE_(begin_val < end_val)
                .except(std::logic_error("End value must be greater than begin value."));
        }
        else
        if (step_val < 0)
        {
            CAPO_ENSURE_(begin_val > end_val)
                .except(std::logic_error("End value must be less than begin value."));
        }

        size_type x = static_cast<size_type>((end_val - begin_val) / step_val);
        if (begin_val + (step_val * x) != end_val) ++x;
        return x;
    }

public:
    template <typename T_, typename U_>
    impl(T_&& begin_val, T_&& end_val, U_&& step_val)
        : base_t(0, get_adjusted_count(std::forward<T_>(begin_val),
                                       std::forward<T_>(end_val),
                                       std::forward<U_>(step_val)),
                 std::forward<T_>(begin_val),
                 std::forward<U_>(step_val))
    {}
};

} // namespace detail_range

////////////////////////////////////////////////////////////////
/// Make a range of [begin, end) with a custom step
////////////////////////////////////////////////////////////////

template <typename T>
auto range(T&& end)
    -> detail_range::impl<typename std::common_type<T>::type>
{
    typename std::common_type<T>::type t {};
    return { static_cast<T&&>(t), std::forward<T>(end), 1 };
}

template <typename T>
auto range(T&& begin, T&& end)
    -> detail_range::impl<typename std::common_type<T>::type>
{
    return { std::forward<T>(begin), std::forward<T>(end), 1 };
}

template <typename T, typename U>
auto range(T&& begin, T&& end, U&& step)
    -> detail_range::impl<typename std::common_type<T, U>::type>
{
    return { std::forward<T>(begin), std::forward<T>(end), std::forward<U>(step) };
}

} // namespace capo
