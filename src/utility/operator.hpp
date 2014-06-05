/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_UTILITY_OPERATOR_HPP___
#define CAPO_UTILITY_OPERATOR_HPP___

#include "../utility/inherit.hpp"

#include <utility>  // std::move, std::forward

namespace capo {

////////////////////////////////////////////////////////////////
/// Make comparable
////////////////////////////////////////////////////////////////

template <class T, typename BaseT = void>
struct unequal : capo::inherit<BaseT>
{
    // Need operator==
    friend bool operator!=(const T& x, const T& y) { return (!static_cast<bool>(x == y)); }
};

template <class T, typename BaseT = void>
struct alike : capo::unequal<T, BaseT>
{
    // Need operator<
    friend bool operator==(const T& x, const T& y) { return (!static_cast<bool>(x < y) && !static_cast<bool>(y < x)); }
};

template <class T, typename BaseT = void>
struct comparable : capo::inherit<BaseT>
{
    // Need operator<
    friend bool operator> (const T& x, const T& y) { return ( static_cast<bool>(y < x)); }
    friend bool operator<=(const T& x, const T& y) { return (!static_cast<bool>(x > y)); }
    friend bool operator>=(const T& x, const T& y) { return (!static_cast<bool>(x < y)); }
};

////////////////////////////////////////////////////////////////
/// Make operable
////////////////////////////////////////////////////////////////

template <class T, typename BaseT>
struct incrementable : capo::inherit<BaseT>
{
    // Need operator+=
    friend T& operator++(T& x)                     { return x += 1; }
    friend T  operator++(T& x, int)                { T nrv(x); ++x; return std::move(nrv); }
};

template <class T, typename BaseT>
struct decrementable : capo::inherit<BaseT>
{
    // Need operator-=
    friend T& operator--(T& x)                     { return x -= 1; }
    friend T  operator--(T& x, int)                { T nrv(x); --x; return std::move(nrv); }
};

#define CAPO_MAKE_OP_SAME__(OP) \
    friend T operator OP(const T & x, const T & y) { return std::move(T(x) OP##= y); } \
    friend T operator OP(      T&& x,       T&& y) { return std::move(  x  OP##= y); } \
    friend T operator OP(      T&& x, const T & y) { return std::move(  x  OP##= y); } \
    friend T operator OP(const T & x,       T&& y) { return std::move(  y  OP##= x); }

#define CAPO_MAKE_OPERABLE__(NAME, OP) \
    template <class T, typename BaseT = void> \
    struct NAME : capo::inherit<BaseT> { CAPO_MAKE_OP_SAME__(OP) };

CAPO_MAKE_OPERABLE__(addable     , +)
CAPO_MAKE_OPERABLE__(subtractable, -)
CAPO_MAKE_OPERABLE__(multipliable, *)
CAPO_MAKE_OPERABLE__(dividable   , /)
CAPO_MAKE_OPERABLE__(modable     , %)
CAPO_MAKE_OPERABLE__(xorable     , ^)
CAPO_MAKE_OPERABLE__(andable     , &)
CAPO_MAKE_OPERABLE__(orable      , |)

#undef CAPO_MAKE_OPERABLE__
#undef CAPO_MAKE_OP_SAME__

////////////////////////////////////////////////////////////////

} // namespace capo

#endif // CAPO_UTILITY_OPERATOR_HPP___
