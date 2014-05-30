/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_ALGORITHM_RANGE_HPP___
#define CAPO_ALGORITHM_RANGE_HPP___

#include <type_traits>  // std::remove_const, std::remove_reference, ...
#include <stdexcept>    // std::logic_error
#include <utility>      // std::forward, std::declval

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
    using r_t = value_t<decltype(std::declval<T>() + std::declval<U>())>;
public:
    using type = typename std::conditional<std::is_same<r_t, t_t>::value, T, U>::type;
};

////////////////////////////////////////////////////////////////
/// The range iterator
////////////////////////////////////////////////////////////////

template <typename T>
class iterator
{
public:
    using value_type = T;

private:
    int cursor_;
    const value_type& step_;
    value_type value_;

public:
    iterator(int cur_start, const value_type& begin_val,
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
    using value_type = value_t<T>;
    using iterator   = detail_range::iterator<value_type>;

private:
    const value_type begin_;
    const value_type end_;
    const value_type step_;
    const int max_count_;

    int get_adjusted_count(void) const
    {
        int x = static_cast<int>((end_ - begin_) / step_);
        if (at(x) != end_) ++x;
        return x;
    }

public:
    impl(T begin_val, T end_val, T step_val)
        : begin_(std::forward<T>(begin_val))
        , end_  (std::forward<T>(end_val))
        , step_ (std::forward<T>(step_val))
        , max_count_(get_adjusted_count())
    {
        if (step_ > 0 && begin_ >= end_)
            throw std::logic_error("End value must be greater than begin value.");
        else
        if (step_ < 0 && begin_ <= end_)
            throw std::logic_error("End value must be less than begin value.");
    }

    int size(void) const
    {
        return max_count_;
    }

    value_type at(int n) const
    {
        return begin_ + (step_ * n);
    }

    value_type operator[](int n) const
    {
        return at(n);
    }

    iterator begin(void)
    {
        return { 0, begin_, step_ };
    }

    iterator end(void)
    {
        return { max_count_, begin_, step_ };
    }
};

} // namespace detail_range

////////////////////////////////////////////////////////////////
/// Make a range of [begin, end) with a custom step
////////////////////////////////////////////////////////////////

template <typename T>
detail_range::impl<T&&> range(T&& end)
{
    return { {}, std::forward<T>(end), 1 };
}

template <typename T>
detail_range::impl<T&&> range(T&& begin, T&& end)
{
    return { std::forward<T>(begin), std::forward<T>(end), 1 };
}

template <typename T, typename U>
auto range(T&& begin, T&& end, U&& step)
    -> detail_range::impl<typename detail_range::comfy<T&&, U&&>::type>
{
    using r_t = detail_range::impl<typename detail_range::comfy<T&&, U&&>::type>;
    // may be narrowing
    return r_t(std::forward<T>(begin), std::forward<T>(end), std::forward<U>(step));
}

} // namespace capo

#endif // CAPO_ALGORITHM_RANGE_HPP___
