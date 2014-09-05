/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_ALGORITHM_SEQUENCE_HPP___
#define CAPO_ALGORITHM_SEQUENCE_HPP___

#include "../proofing/assert.hpp"

#include <type_traits>  // std::enable_if, std::is_same, std::decay
#include <tuple>        // std::tuple, std::tie
#include <stdexcept>    // std::logic_error
#include <utility>      // std::forward, std::move
#include <cstddef>      // size_t

namespace capo {

////////////////////////////////////////////////////////////////
/// Perpare for sequence policies
////////////////////////////////////////////////////////////////

template <typename T, typename U, U A1, U A2>
struct sequence_state
{
    using value_type = T;

    value_type x_;
    value_type y_;

    template <typename U1 = U, typename U2 = U>
    sequence_state(U1&& a1 = A1, U2&& a2 = A2)
        : x_(std::forward<U1>(a1))
        , y_(std::forward<U2>(a2))
    {}
};

template <typename T, typename U, U A1, U A2,
          template <typename, typename, U, U> class StateT = capo::sequence_state>
struct sequence_policy
{
    using state_type      = StateT<T, U, A1, A2>;
    using value_type      = typename state_type::value_type;
    using const_reference = const value_type&;
    using reference       = const_reference;
    using size_type       = size_t;

protected:
    size_type  t_;
    state_type s_;

public:
    template <typename... U_>
    sequence_policy(size_type t, U_&&... args)
        : t_(t)
        , s_(std::forward<U_>(args)...)
    {}

    template <class S, typename = typename std::enable_if<
                                           std::is_same<
                                  typename std::decay<S>::type, state_type>::value>::type>
    sequence_policy(S&& s, size_type t)
        : t_(t)
        , s_(std::forward<S>(s))
    {}

    void next(void) { ++t_; }
    void prev(void) { --t_; }

    bool operator==(const sequence_policy& rhs) const
    {
        return t_ == rhs.t_;
    }
};

namespace use {

////////////////////////////////////////////////////////////////
/// Sequence policies
////////////////////////////////////////////////////////////////

/*
    Arithmetic sequence
*/

template <typename T>
class arithmetic : public sequence_policy<T, int, 0, 1>
{
    using base_t = sequence_policy<T, int, 0, 1>;

    using base_t::t_;
    using base_t::s_;

public:
    using value_type      = typename base_t::value_type;
    using reference       = typename base_t::reference;
    using const_reference = typename base_t::const_reference;
    using size_type       = typename base_t::size_type;

public:
    using base_t::base_t;

    template <typename... U>
    arithmetic(size_type t, U&&... args)
        : base_t(t, std::forward<U>(args)...)
    {
        s_.x_ += (s_.y_ * t_);
    }

    const_reference next(void) { base_t::next(); return s_.x_ += s_.y_; }
    const_reference prev(void) { base_t::prev(); return s_.x_ -= s_.y_; }

    const_reference value(void) const { return s_.x_; }
};

/*
    Geometric sequence
*/

template <typename T>
class geometric : public sequence_policy<T, int, 1, 2>
{
    using base_t = sequence_policy<T, int, 1, 2>;

    using base_t::t_;
    using base_t::s_;

public:
    using value_type      = typename base_t::value_type;
    using reference       = typename base_t::reference;
    using const_reference = typename base_t::const_reference;
    using size_type       = typename base_t::size_type;

    static value_type power(value_type n, size_type exp)
    {
        if (exp == 0) return 1;
        if (n == 0 || n == 1) return std::move(n);
        value_type k = 1;
        while (exp > 1)
        {
            if (exp & 0x01) k *= n;
            n *= n;
            exp >>= 1;
        }
        return std::move(k *= n);
    }

public:
    using base_t::base_t;

    template <typename... U>
    geometric(size_type t, U&&... args)
        : base_t(t, std::forward<U>(args)...)
    {
        s_.x_ *= power(s_.y_, t_);
    }

    const_reference next(void) { base_t::next(); return s_.x_ *= s_.y_; }
    const_reference prev(void) { base_t::prev(); return s_.x_ /= s_.y_; }

    const_reference value(void) const { return s_.x_; }
};

/*
    Fibonacci sequence
*/

template <typename T>
class fibonacci : public sequence_policy<T, int, 0, 1>
{
    using base_t = sequence_policy<T, int, 0, 1>;

    using base_t::t_;
    using base_t::s_;

public:
    using value_type      = typename base_t::value_type;
    using reference       = typename base_t::reference;
    using const_reference = typename base_t::const_reference;
    using size_type       = typename base_t::size_type;

    using result_t = std::tuple<value_type, value_type>;

    static result_t fib_two(size_type n, const_reference a0, const_reference a1)
    {
        if (n == 1) return result_t(a1, a1 + a0);
        if (n == 0) return result_t(a0, a1);
        value_type i, j;
        if (n & 0x01)   /* F(2n + 1) */
        {
            std::tie(i, j) = fib_two((n - 1) >> 1, a0, a1); // { F(n), F(n + 1) }
            value_type f_2nA2 = j * (i * 2 + j);            // F(2n + 2) = F(n + 1) * (F(n) * 2 + F(n + 1))
            (i *= i) += (j *= j);                           // F(2n + 1) = F(n)^2 + F(n + 1)^2
            return result_t(std::move(i), std::move(f_2nA2));
        }
        else            /* F(2n) */
        {
            std::tie(i, j) = fib_two(n >> 1, a0, a1);       // { F(n), F(n + 1) }
            value_type f_2n = i * (j * 2 - i);              // F(2n)     = F(n) * (F(n + 1) * 2 - F(n))
            (i *= i) += (j *= j);                           // F(2n + 1) = F(n)^2 + F(n + 1)^2
            return result_t(std::move(f_2n), std::move(i));
        }
    }

public:
    using base_t::base_t;

    template <typename... U>
    fibonacci(size_type t, U&&... args)
        : base_t(t, std::forward<U>(args)...)
    {
        if (t == 0) return;
        std::tie(s_.x_, s_.y_) = fib_two(t_, s_.x_, s_.y_);
    }

    const_reference next(void) { base_t::next(); return (s_.x_ < s_.y_) ? s_.x_ += s_.y_ : s_.y_ += s_.x_; }
    const_reference prev(void) { base_t::prev(); return (s_.x_ < s_.y_) ? s_.y_ -= s_.x_ : s_.x_ -= s_.y_; }

    const_reference value(void) const { return (s_.x_ < s_.y_) ? s_.x_ : s_.y_; }
};

} // namespace use

namespace detail_sequence {

////////////////////////////////////////////////////////////////
/// Sequence iterator
////////////////////////////////////////////////////////////////

template <class PolicyT>
class iterator
{
public:
    using sequence_type   = PolicyT;
    using value_type      = typename sequence_type::value_type;
    using reference       = typename sequence_type::reference;
    using const_reference = typename sequence_type::const_reference;
    using size_type       = typename sequence_type::size_type;

private:
    sequence_type seq_;

public:
    iterator(sequence_type&& seq) : seq_(std::move(seq)) {}

    const_reference operator*() const { return seq_.value(); }

    bool operator==(const iterator& rhs) const
    {
        return (seq_ == rhs.seq_);
    }

    bool operator!=(const iterator& rhs) const
    {
        return !operator==(rhs);
    }

    iterator& operator++(void) // prefix ++ operator only
    {
        seq_.next();
        return (*this);
    }
};

////////////////////////////////////////////////////////////////
/// The impl class
////////////////////////////////////////////////////////////////

template <class PolicyT>
class impl
{
public:
    using iterator        = detail_sequence::iterator<PolicyT>;
    using const_iterator  = const iterator;
    using value_type      = typename iterator::value_type;
    using reference       = typename iterator::reference;
    using const_reference = typename iterator::const_reference;
    using size_type       = typename iterator::size_type;
    using sequence_type   = typename iterator::sequence_type;
    using state_type      = typename sequence_type::state_type;

private:
    size_type  cur_begin_;
    size_type  cur_end_;
    state_type state_;

public:
    template <typename... U>
    impl(size_type cur_begin, size_type cur_end, U&&... args)
        : cur_begin_(cur_begin)
        , cur_end_  (cur_end)
        , state_    (std::forward<U>(args)...)
    {
        capo_ensure(cur_begin_ < cur_end_)
            .except(std::logic_error("End index must be greater than begin index."));
    }

    size_type size(void) const { return (cur_end_ - cur_begin_); }

    const_iterator begin(void) const
    {
        return sequence_type(state_, cur_begin_);
    }

    const_iterator end(void) const
    {
        return sequence_type(state_, cur_end_);
    }
};

} // namespace detail_sequence

////////////////////////////////////////////////////////////////
/// Make a sequence of [begin-index, end-index)
////////////////////////////////////////////////////////////////

template <class PolicyT, typename... U>
auto sequence(typename PolicyT::size_type begin,
              typename PolicyT::size_type end, U&&... args)
    -> detail_sequence::impl<PolicyT>
{
    return { begin, end, std::forward<U>(args)... };
}

} // namespace capo

#endif // CAPO_ALGORITHM_SEQUENCE_HPP___
