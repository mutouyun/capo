/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_ALGORITHM_RANGE_HPP___
#define CAPO_ALGORITHM_RANGE_HPP___

#include "../proofing/assert.hpp"

#include <type_traits>      // std::remove_const, std::remove_reference, ...
#include <stdexcept>        // std::logic_error
#include <utility>          // std::forward, std::declval
#include <cstddef>          // size_t

namespace capo {
namespace detail_range {

////////////////////////////////////////////////////////////////
/// Type helpers
////////////////////////////////////////////////////////////////

template <typename T>
using value_t = typename std::remove_const<
                typename std::remove_reference<T>::type>::type;

template <typename T, typename U>
struct comfy
{
private:
    using t_t = value_t<T>;
    using u_t = value_t<U>;
    using r_t = value_t<decltype(std::declval<T>() + std::declval<U>())>;
public:
    using type = typename std::conditional<std::is_same<r_t, t_t>::value, t_t, u_t>::type;
};

template <typename T, typename U>
using comfy_t = typename comfy<T, U>::type;

////////////////////////////////////////////////////////////////
/// The range iterator
////////////////////////////////////////////////////////////////

template <typename T>
class iterator
{
public:
    using value_type = T;
    using size_type  = size_t;

private:
    size_type cursor_;
    const value_type& step_;
    value_type value_;

public:
    iterator(size_type cur_start, const value_type& begin_val,
                                  const value_type& step_val)
        : cursor_(cur_start)
        , step_  (step_val)
        , value_ (begin_val)
    {
        value_ += (step_ * cursor_);
    }

    value_type operator*() const { return value_; }

    bool operator==(const iterator& rhs) const
    {
        return (cursor_ == rhs.cursor_);
    }

    bool operator!=(const iterator& rhs) const
    {
        return !operator==(rhs);
    }

    iterator& operator++(void) // prefix ++ operator only
    {
        value_ += step_;
        ++ cursor_;
        return (*this);
    }
};

////////////////////////////////////////////////////////////////
/// The impl class
////////////////////////////////////////////////////////////////

template <typename T>
class impl
{
public:
    using value_type      = T;
    using reference       = const value_type&;
    using const_reference = const value_type&;
    using iterator        = const detail_range::iterator<value_type>;
    using const_iterator  = const detail_range::iterator<value_type>;
    using size_type       = typename iterator::size_type;

private:
    value_type begin_;
    value_type end_;
    value_type step_;
    size_type  max_count_;

    size_type get_adjusted_count(void) const
    {
        if (step_ > 0)
        {
            capo_ensure(begin_ < end_)
                .except(std::logic_error("End value must be greater than begin value."));
        }
        else
        if (step_ < 0)
        {
            capo_ensure(begin_ > end_)
                .except(std::logic_error("End value must be less than begin value."));
        }

        size_type x = static_cast<size_type>((end_ - begin_) / step_);
        if (begin_ + (step_ * x) != end_) ++x;
        return x;
    }

public:
    template <typename U1, typename U2, typename U3>
    impl(U1&& begin_val, U2&& end_val, U3&& step_val)
        : begin_(std::forward<U1>(begin_val))
        , end_  (std::forward<U2>(end_val))
        , step_ (std::forward<U3>(step_val))
        , max_count_(get_adjusted_count())
    {}

    impl(const impl& rhs) = default;
    impl(impl&& rhs)
        : begin_(std::move(rhs.begin_))
        , end_  (std::move(rhs.end_))
        , step_ (std::move(rhs.step_))
        , max_count_(rhs.max_count_)
    {}

    impl& operator=(const impl& rhs) = default;
    impl& operator=(impl&& rhs)
    {
        begin_ = std::move(rhs.begin_);
        end_   = std::move(rhs.end_);
        step_  = std::move(rhs.step_);
        max_count_ = rhs.max_count_;
        return (*this);
    }

    size_type size(void) const
    {
        return max_count_;
    }

    const_iterator begin(void) const
    {
        return { 0, begin_, step_ };
    }

    const_iterator end(void) const
    {
        return { max_count_, begin_, step_ };
    }
};

} // namespace detail_range

////////////////////////////////////////////////////////////////
/// Make a range of [begin, end) with a custom step
////////////////////////////////////////////////////////////////

template <typename T>
auto range(T&& end)
    -> detail_range::impl<detail_range::value_t<T>>
{
    return { detail_range::value_t<T>{}, std::forward<T>(end), 1 };
}

template <typename T>
auto range(T&& begin, T&& end)
    -> detail_range::impl<detail_range::value_t<T>>
{
    return { std::forward<T>(begin), std::forward<T>(end), 1 };
}

template <typename T, typename U>
auto range(T&& begin, T&& end, U&& step)
    -> detail_range::impl<detail_range::comfy_t<T, U>>
{
    return { std::forward<T>(begin), std::forward<T>(end), std::forward<U>(step) };
}

} // namespace capo

#endif // CAPO_ALGORITHM_RANGE_HPP___
