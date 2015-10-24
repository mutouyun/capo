/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/type_list.hpp"
#include "capo/types_to_seq.hpp"
#include "capo/type_traits.hpp"
#include "capo/tuple.hpp"
#include "capo/constant_array.hpp"
#include "capo/concept.hpp"

#include <utility>  // std::move, std::forward, std::swap

namespace capo {

////////////////////////////////////////////////////////////////

/*
    Define infinity
*/

enum : size_t
{
    Infinity = static_cast<size_t>(-1)
};

namespace detail_iterator_ {

/*
    Concept for checking convertible types.
*/

template <typename T, typename... P>
struct Convertible
    : std::false_type
{};

template <typename T, typename P1>
struct Convertible<T, P1>
    : std::is_convertible<typename std::remove_reference<P1>::type, T>
{};

template <typename T, typename P1, typename... P>
struct Convertible<T, P1, P...>
    : std::integral_constant<bool, Convertible<T, P1>::value && Convertible<T, P...>::value>
{};

/*
    P&&... args => std::tuple<T...>
    Be used by iterator::iterator(P&&... args).
*/

template <size_t ForwardN, typename... A>
auto forward(typename std::enable_if<(ForwardN <= sizeof...(A))>::type*, A&&... args)
{
    return capo::forward_as_tuple<ForwardN>(std::forward<A>(args)...);
}

template <size_t ForwardN, typename... A>
auto forward(typename std::enable_if<(ForwardN > sizeof...(A))>::type*, A&&... args)
{
    // Makes the front default values to 0.
    return std::tuple_cat(capo::numbers_to_tuple(capo::constant_array<int>::assign<ForwardN - sizeof...(A), 0>{}), 
                          std::forward_as_tuple(std::forward<A>(args)...));
}

} // namespace detail_iterator_

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

    template <typename... P, CAPO_REQUIRE_(detail_iterator_::Convertible<T, P...>::value)>
    iterator(P&&... args)
        : x_(detail_iterator_::forward<seq::value>(nullptr, std::forward<P>(args)...))
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
