/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/assert.hpp"
#include "capo/iterator.hpp"

#include <type_traits>  // std::add_const, std::add_lvalue_reference
#include <tuple>        // std::tuple, std::tie
#include <stdexcept>    // std::logic_error
#include <utility>      // std::forward, std::move, std::swap
#include <cstddef>      // size_t

namespace capo {
namespace use {

////////////////////////////////////////////////////////////////
/// Sequence policies
////////////////////////////////////////////////////////////////

/*
    Arithmetic sequence
*/

template <int D = 1>
struct arithmetic
{
    enum { StateSize = 1 };

    template <typename T> static void next(T& a) { a += D; }
    template <typename T> static void prev(T& a) { a -= D; }

    template <typename T> static void at(T& a1, size_t n)
    {
        capo_assert(n);
        a1 += D * (n - 1);
    }
};

/*
    Geometric sequence
*/

template <int Q = 2>
struct geometric
{
    static_assert(Q != 0, "Q cannot be equal to 0!");

    enum { StateSize = 1 };

    template <typename T> static void next(T& a) { a *= Q; }
    template <typename T> static void prev(T& a) { a /= Q; }

    template <typename T> static void at(T& a1, size_t n)
    {
        capo_assert(n);
        a1 *= power(Q, n - 1);
    }

private:
    static int power(int x, size_t n)
    {
        if (n == 0) return 1;
        if (x == 0 || x == 1) return x;
        T k = 1;
        while (n > 1)
        {
            if (n & 0x01) k *= x;
            x *= x;
            n >>= 1;
        }
        return k *= x;
    }
};

/*
    Fibonacci sequence
*/

struct fibonacci
{
    enum { StateSize = 2 };

    template <typename T> static void next(T& a1, T& a2)
    {
        std::swap(a1, a2);
        a2 += a1;
    }

    template <typename T> static void prev(T& a1, T& a2)
    {
        std::swap(a1, a2);
        a1 -= a2;
    }

    template <typename T> static void at(T& a0, T& a1, size_t n)
    {
        std::tie(a0, a1) = fib_two(a0, a1, n);
    }

private:
    template <typename T> static std::tuple<T, T>
        fib_two(const T& a0, const T& a1, size_t n)
    {
        if (n == 1) return std::tuple<T, T>{ a1, a1 + a0 };
        if (n == 0) return std::tuple<T, T>{ a0, a1 };
        T i, j;
        if (n & 0x01)   /* F(2n + 1) */
        {
            std::tie(i, j) = fib_two(a0, a1, (n - 1) >> 1); // {F(n), F(n + 1)}
            return std::tuple<T, T>
            {
                i * i + j * j,      // F(2n + 1) = F(n) ^ 2 + F(n + 1) ^ 2
                j * (i * 2 + j)     // F(2n + 2) = F(n + 1) * (F(n) * 2 + F(n + 1))
            };
        }
        else            /* F(2n) */
        {
            std::tie(i, j) = fib_two(a0, a1, n >> 1);   // {F(n), F(n + 1)}
            return std::tuple<T, T>
            {
                i * (j * 2 - i),    // F(2n)     = F(n) * (F(n + 1) * 2 - F(n))
                i * i + j * j       // F(2n + 1) = F(n) ^ 2 + F(n + 1) ^ 2
            };
        }
    }
};

} // namespace use

namespace detail_sequence {

////////////////////////////////////////////////////////////////
/// The impl class
////////////////////////////////////////////////////////////////

template <class PolicyT, typename T, class IterT = capo::iterator<PolicyT, T>>
class impl
{
public:
    using iterator        = IterT;
    using const_iterator  = const iterator;
    using value_type      = T;
    using reference       = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<typename std::add_const<T>::type>::type;
    using size_type       = typename iterator::size_type;

private:
    size_type cur_begin_, cur_end_;
    typename iterator::tp_t state_;

public:
    template <typename... U>
    impl(size_type begin, size_type end, U&&... args)
        : cur_begin_(begin)
        , cur_end_  (end)
        , state_    (std::forward<U>(args)...)
    {
        capo_ensure(cur_begin_ < cur_end_)(cur_begin_)(cur_end_)
            .except(std::logic_error("End index must be greater than begin index."));
    }

    size_type size(void) const { return (cur_end_ - cur_begin_); }

    const_iterator begin(void) const
    {
        return { state_, cur_begin_ };
    }

    const_iterator end(void) const
    {
        return { state_, cur_end_ };
    }
};

} // namespace detail_sequence

////////////////////////////////////////////////////////////////
/// Make a sequence of [begin-index, end-index)
////////////////////////////////////////////////////////////////

template <class PolicyT, typename T, typename... U>
auto sequence(size_t begin, size_t end, U&&... args)
    -> detail_sequence::impl<PolicyT, T>
{
    return { begin, end, std::forward<U>(args)... };
}

} // namespace capo
