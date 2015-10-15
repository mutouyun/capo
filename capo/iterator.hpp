/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/type_list.hpp"
#include "capo/types_to_seq.hpp"
#include "capo/type_traits.hpp"

#include <tuple>        // std::tuple, std::get
#include <utility>      // std::move, std::forward, std::swap

namespace capo {

enum : size_t
{
    Infinity = static_cast<size_t>(-1)
};

namespace detail_iterator {

template <typename T, typename... P>
struct check;

template <typename T, typename P1>
struct check<T, P1>
    : std::is_convertible<typename std::remove_reference<P1>::type, T>
{};

template <typename T, typename P1, typename... P>
struct check<T, P1, P...>
    : std::integral_constant<bool, std::is_convertible<typename std::remove_reference<P1>::type, T>::value && check<P...>::value>
{};

template <typename A, typename P1, typename... P, int... N>
std::tuple<A, P...> join(P1&& a, std::tuple<P...>& tp, constant_seq<N...>)
{
    return std::tuple<A, P...>(
        std::forward<P1>(a),
        static_cast<typename std::tuple_element<N, std::tuple<P...>>::type>(std::get<N>(tp))...
    );
}

template <int... M, typename... P>
std::tuple<> forward(constant_seq<>, constant_seq<M...>, P&&... args)
{
    return {};
}

template <int N1, int... N, int M1, int... M, typename P1, typename... P,
          typename = typename std::enable_if<(sizeof...(N) <= sizeof...(M))>::type>
auto forward(constant_seq<N1, N...>, constant_seq<M1, M...>, P1&& a, P&&... args)
{
    auto fr_tp = forward(constant_seq<N...>{}, constant_seq<M...>{}, std::forward<P>(args)...);
    return join<typename capo::underlying<P1>::type>(std::forward<P1>(a), fr_tp, list_to_seq<decltype(fr_tp)>{});
}

template <int N1, int... N, int... M, typename... P,
          typename = typename std::enable_if<(sizeof...(N) >= sizeof...(M))>::type>
auto forward(constant_seq<N1, N...>, constant_seq<M...>, P&&... args)
{
    auto fr_tp = forward(constant_seq<N...>{}, constant_seq<M...>{}, std::forward<P>(args)...);
    return join<int>(0 /* Makes the default value to 0. */, fr_tp, list_to_seq<decltype(fr_tp)>{});
}

} // namespace detail_iterator

////////////////////////////////////////////////////////////////
/// Iterator pattern
////////////////////////////////////////////////////////////////

template <class A, typename T, int StateSize = A::StateSize>
class iterator
{
public:
    using tp_t = types_assign_t<StateSize, std::tuple<T>>;
    using size_type = size_t;

private:
    tp_t   x_;
    size_t i_ = 0;

    using seq = list_to_seq<tp_t>;

    template <int... N> void next(constant_seq<N...>) { A::next(std::get<N>(x_)...); ++i_; }
    template <int... N> void prev(constant_seq<N...>) { A::prev(std::get<N>(x_)...); --i_; }
    template <int... N> void at(size_t n, constant_seq<N...>)
    {
        if (n != capo::Infinity)
            A::at(std::get<N>(x_)..., n);
        i_ = n;
    }

public:
    iterator(void) {}

    template <typename... P,
              typename = typename std::enable_if<detail_iterator::check<T, P...>::value>::type>
    iterator(P&&... args)
        : x_(detail_iterator::forward(seq{}, types_to_seq<P...>{}, std::forward<P>(args)...))
    {}

    iterator(const tp_t& x)           : x_(x) {}
    iterator(const tp_t& x, size_t n) : x_(x) { at(n, seq{}); }

    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;

    iterator& operator=(iterator rhs)
    {
        x_.swap(rhs.x_);
        i_ = rhs.i_;
        return (*this);
    }

public:
    T&       operator*(void)       { return std::get<0>(x_); }
    const T& operator*(void) const { return std::get<0>(x_); }

    T&       operator->(void)       { return this->operator*(); }
    const T& operator->(void) const { return this->operator*(); }

    bool operator==(const iterator& rhs) const { return (this->i_ == rhs.i_); }
    bool operator!=(const iterator& rhs) const { return !operator==(rhs); }

    tp_t& get_tuple(void) { return x_; }

    iterator& operator++(void)
    {
        next(seq());
        return (*this);
    }

    iterator& operator--(void)
    {
        prev(seq());
        return (*this);
    }

    iterator operator++(int)
    {
        iterator old(*this);
        ++(*this);
        return old;
    }

    iterator operator--(int)
    {
        iterator old(*this);
        --(*this);
        return old;
    }
};

////////////////////////////////////////////////////////////////

} // namespace capo
