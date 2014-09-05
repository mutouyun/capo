/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_ALGORITHM_RANGE_HPP___
#define CAPO_ALGORITHM_RANGE_HPP___

#include "../algorithm/sequence.hpp"
#include "../proofing/assert.hpp"

#include <type_traits>  // std::decay
#include <stdexcept>    // std::logic_error
#include <utility>      // std::forward

namespace capo {
namespace detail_range {

////////////////////////////////////////////////////////////////
/// The impl class
////////////////////////////////////////////////////////////////

template <typename T>
class impl : public decltype(capo::sequence<capo::use::arithmetic<T>>(0, 0))
{
    using base_t = decltype(capo::sequence<capo::use::arithmetic<T>>(0, 0));

public:
    using size_type = typename base_t::size_type;

private:
    template <typename U1, typename U2>
    size_type get_adjusted_count(U1&& begin_val, U1&& end_val, U2&& step_val) const
    {
        if (step_val > 0)
        {
            capo_ensure(begin_val < end_val)
                .except(std::logic_error("End value must be greater than begin value."));
        }
        else
        if (step_val < 0)
        {
            capo_ensure(begin_val > end_val)
                .except(std::logic_error("End value must be less than begin value."));
        }

        size_type x = static_cast<size_type>((end_val - begin_val) / step_val);
        if (begin_val + (step_val * x) != end_val) ++x;
        return x;
    }

public:
    template <typename U1, typename U2>
    impl(U1&& begin_val, U1&& end_val, U2&& step_val)
        : base_t(0, get_adjusted_count(std::forward<U1>(begin_val),
                                       std::forward<U1>(end_val),
                                       std::forward<U2>(step_val)),
                 std::forward<U1>(begin_val),
                 std::forward<U2>(step_val))
    {}
};

} // namespace detail_range

////////////////////////////////////////////////////////////////
/// Make a range of [begin, end) with a custom step
////////////////////////////////////////////////////////////////

template <typename T>
auto range(T&& end)
    -> detail_range::impl<typename std::decay<T>::type>
{
    typename std::decay<T>::type t {};
    return { static_cast<T&&>(t), std::forward<T>(end), 1 };
}

template <typename T>
auto range(T&& begin, T&& end)
    -> detail_range::impl<typename std::decay<T>::type>
{
    return { std::forward<T>(begin), std::forward<T>(end), 1 };
}

template <typename T, typename U>
auto range(T&& begin, T&& end, U&& step)
    -> detail_range::impl<typename std::decay<decltype(begin + step)>::type>
{
    return { std::forward<T>(begin), std::forward<T>(end), std::forward<U>(step) };
}

} // namespace capo

#endif // CAPO_ALGORITHM_RANGE_HPP___
