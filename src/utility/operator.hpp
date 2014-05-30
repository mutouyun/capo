/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_UTILITY_OPERATOR_HPP___
#define CAPO_UTILITY_OPERATOR_HPP___

#include "../macro/macro_pp.hpp"
#include "../utility/inherit.hpp"

#include <type_traits>  // std::enable_if, std::is_same, std::remove_reference
#include <utility>      // std::move, std::forward

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
struct alike : capo::inherit<BaseT>
{
    // Need operator<
    friend bool operator!=(const T& x, const T& y) { return (static_cast<bool>(x < y) && static_cast<bool>(y < x)); }
    friend bool operator==(const T& x, const T& y) { return !(x != y); }
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
    friend T& operator++(T& x)                      { return x += 1; }
    friend T  operator++(T& x, int)                 { T nrv(x); ++x; return std::move(nrv); }
};

template <class T, typename BaseT>
struct decrementable : capo::inherit<BaseT>
{
    // Need operator-=
    friend T& operator--(T& x)                      { return x -= 1; }
    friend T  operator--(T& x, int)                 { T nrv(x); --x; return std::move(nrv); }
};

namespace detail_operator
{
    // Enable if types are different
    template <class T, typename U>
    using enable_if_diff = std::enable_if<!(std::is_same<T, typename std::remove_reference<U>::type>::value)>;
}

#define CAPO_MAKE_OP_SAME__(OP) \
    friend T operator OP(const T & x, const T & y) { return std::move(T(x) OP##= y); } \
    friend T operator OP(      T&& x,       T&& y) { return std::move(  x  OP##= y); } \
    friend T operator OP(      T&& x, const T & y) { return std::move(  x  OP##= y); } \
    friend T operator OP(const T & x,       T&& y) { return std::move(  y  OP##= x); }

#define CAPO_MAKE_OP_DIFF_COMM__Y__(OP) /* commutative */ \
    template <typename U, typename = typename detail_operator::enable_if_diff<T, U>::type> \
    friend T operator OP(      U&& x, const T & y) { return std::move(T(y) OP##= std::forward<U>(x)); } \
    template <typename U, typename = typename detail_operator::enable_if_diff<T, U>::type> \
    friend T operator OP(      U&& x,       T&& y) { return std::move(  y  OP##= std::forward<U>(x)); }

#define CAPO_MAKE_OP_DIFF_COMM__N__(OP) /* non-commutative */ \
    template <typename U, typename = typename detail_operator::enable_if_diff<T, U>::type> \
    friend T operator OP(      U&& x, const T & y) { return std::move(T(std::forward<U>(x)) OP##= y); }

#define CAPO_MAKE_OP_DIFF__(OP, COMM) \
    template <typename U, typename = typename detail_operator::enable_if_diff<T, U>::type> \
    friend T operator OP(const T & x,       U&& y) { return std::move(T(x) OP##= std::forward<U>(y)); } \
    template <typename U, typename = typename detail_operator::enable_if_diff<T, U>::type> \
    friend T operator OP(      T&& x,       U&& y) { return std::move(  x  OP##= std::forward<U>(y)); } \
    CAPO_PP_JOIN(CAPO_MAKE_OP_DIFF_COMM__, COMM)(OP)

#define CAPO_MAKE_OPERABLE__(NAME, OP, COMM) \
    template <class T, typename BaseT = void> \
    struct NAME : capo::inherit<BaseT> \
    { \
        CAPO_MAKE_OP_SAME__(OP) \
        CAPO_MAKE_OP_DIFF__(OP, CAPO_PP_JOIN(COMM, __)) \
    };

CAPO_MAKE_OPERABLE__(addable     , +, Y)
CAPO_MAKE_OPERABLE__(subtractable, -, N)
CAPO_MAKE_OPERABLE__(multipliable, *, Y)
CAPO_MAKE_OPERABLE__(dividable   , /, N)
CAPO_MAKE_OPERABLE__(modable     , %, N)
CAPO_MAKE_OPERABLE__(xorable     , ^, Y)
CAPO_MAKE_OPERABLE__(andable     , &, Y)
CAPO_MAKE_OPERABLE__(orable      , |, Y)

#undef CAPO_MAKE_OPERABLE__
#undef CAPO_MAKE_OP_DIFF__
#undef CAPO_MAKE_OP_DIFF_COMM__N__
#undef CAPO_MAKE_OP_DIFF_COMM__Y__
#undef CAPO_MAKE_OP_SAME__

////////////////////////////////////////////////////////////////

} // namespace capo

#endif // CAPO_UTILITY_OPERATOR_HPP___
