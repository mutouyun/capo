/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/inherit.hpp"
#include "capo/preprocessor.hpp"

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

template <typename T, typename U, 
          template <typename...> class TypesT, typename BaseT>
struct unequal<TypesT<T, U>, BaseT> : capo::inherit<BaseT>
{
    // Need operator==
    friend bool operator!=(const T& x, const U& y) { return (!static_cast<bool>(x == y)); }
    friend bool operator!=(const U& x, const T& y) { return (!static_cast<bool>(x == y)); }
};

template <class T, typename BaseT = void>
struct alike : capo::unequal<T, BaseT>
{
    // Need operator<
    friend bool operator==(const T& x, const T& y) { return (!static_cast<bool>(x < y) && !static_cast<bool>(y < x)); }
};

template <typename T, typename U, 
          template <typename...> class TypesT, typename BaseT>
struct alike<TypesT<T, U>, BaseT> : capo::unequal<T, BaseT>
{
    // Need operator<
    friend bool operator==(const T& x, const U& y) { return (!static_cast<bool>(x < y) && !static_cast<bool>(y < x)); }
    friend bool operator==(const U& x, const T& y) { return (!static_cast<bool>(x < y) && !static_cast<bool>(y < x)); }
};

template <class T, typename BaseT = void>
struct comparable : capo::inherit<BaseT>
{
    // Need operator<
    friend bool operator> (const T& x, const T& y) { return ( static_cast<bool>(y < x)); }
    friend bool operator<=(const T& x, const T& y) { return (!static_cast<bool>(x > y)); }
    friend bool operator>=(const T& x, const T& y) { return (!static_cast<bool>(x < y)); }
};

template <typename T, typename U, 
          template <typename...> class TypesT, typename BaseT>
struct comparable<TypesT<T, U>, BaseT> : capo::inherit<BaseT>
{
    // Need operator<
    friend bool operator> (const T& x, const U& y) { return ( static_cast<bool>(y < x)); }
    friend bool operator> (const U& x, const T& y) { return ( static_cast<bool>(y < x)); }
    friend bool operator<=(const T& x, const U& y) { return (!static_cast<bool>(x > y)); }
    friend bool operator<=(const U& x, const T& y) { return (!static_cast<bool>(x > y)); }
    friend bool operator>=(const T& x, const U& y) { return (!static_cast<bool>(x < y)); }
    friend bool operator>=(const U& x, const T& y) { return (!static_cast<bool>(x < y)); }
};

////////////////////////////////////////////////////////////////
/// Make operable
////////////////////////////////////////////////////////////////

template <class T, typename BaseT = void>
struct incrementable : capo::inherit<BaseT>
{
    // Need operator+=
    friend T& operator++(T& x)                     { return x += 1; }
    friend T  operator++(T& x, int)                { T nrv(x); ++x; return std::move(nrv); }
};

template <typename T, typename U, 
          template <typename...> class TypesT, typename BaseT>
struct incrementable<TypesT<T, U>, BaseT> : capo::incrementable<T, BaseT> {};

template <class T, typename BaseT = void>
struct decrementable : capo::inherit<BaseT>
{
    // Need operator-=
    friend T& operator--(T& x)                     { return x -= 1; }
    friend T  operator--(T& x, int)                { T nrv(x); --x; return std::move(nrv); }
};

template <typename T, typename U, 
          template <typename...> class TypesT, typename BaseT>
struct decrementable<TypesT<T, U>, BaseT> : capo::decrementable<T, BaseT> {};

#pragma push_macro("CAPO_MAKE_OP_SAME__")
#pragma push_macro("CAPO_MAKE_OP_DIFF_COMM__Y__")
#pragma push_macro("CAPO_MAKE_OP_DIFF_COMM__N__")
#pragma push_macro("CAPO_MAKE_OP_DIFF__")
#pragma push_macro("CAPO_MAKE_OPERABLE__")

#undef CAPO_MAKE_OP_SAME__
#undef CAPO_MAKE_OP_DIFF_COMM__Y__
#undef CAPO_MAKE_OP_DIFF_COMM__N__
#undef CAPO_MAKE_OP_DIFF__
#undef CAPO_MAKE_OPERABLE__

#define CAPO_MAKE_OP_SAME__(OP) \
    friend T operator OP(const T & x, const T & y) { return std::move(T(x) OP##= y); } \
    friend T operator OP(      T&& x,       T&& y) { return std::move(  x  OP##= y); } \
    friend T operator OP(      T&& x, const T & y) { return std::move(  x  OP##= y); } \
    friend T operator OP(const T & x,       T&& y) { return std::move(  y  OP##= x); }

#define CAPO_MAKE_OP_DIFF_COMM__Y__(OP) /* commutative */ \
    friend T operator OP(const U & x, const T & y) { return std::move(T(y) OP##= x); } \
    friend T operator OP(      U&& x,       T&& y) { return std::move(  y  OP##= std::forward<U>(x)); } \
    friend T operator OP(      U&& x, const T & y) { return std::move(T(y) OP##= std::forward<U>(x)); } \
    friend T operator OP(const U & x,       T&& y) { return std::move(  y  OP##= x); }

#define CAPO_MAKE_OP_DIFF_COMM__N__(OP) /* non-commutative */ \
    friend T operator OP(const U & x, const T & y) { return std::move(T(x) OP##= y); } \
    friend T operator OP(      U&& x, const T & y) { return std::move(T(std::forward<U>(x)) OP##= y); }

#define CAPO_MAKE_OP_DIFF__(OP, COMM) \
    friend T operator OP(const T & x, const U & y) { return std::move(T(x) OP##= y); } \
    friend T operator OP(      T&& x,       U&& y) { return std::move(  x  OP##= std::forward<U>(y)); } \
    friend T operator OP(      T&& x, const U & y) { return std::move(  x  OP##= y); } \
    friend T operator OP(const T & x,       U&& y) { return std::move(T(x) OP##= std::forward<U>(y)); } \
    CAPO_PP_JOIN_(CAPO_MAKE_OP_DIFF_COMM__, COMM)(OP)

#define CAPO_MAKE_OPERABLE__(NAME, OP, COMM)                       \
    template <class T, typename BaseT = void>                      \
    struct NAME : capo::inherit<BaseT>                             \
    {                                                              \
        CAPO_MAKE_OP_SAME__(OP)                                    \
    };                                                             \
    template <typename T, typename U,                              \
              template <typename...> class TypesT, typename BaseT> \
    struct NAME<TypesT<T, U>, BaseT> : capo::inherit<BaseT>        \
    {                                                              \
        CAPO_MAKE_OP_DIFF__(OP, CAPO_PP_JOIN_(COMM, __))           \
    };

CAPO_MAKE_OPERABLE__(addable     , +, Y)
CAPO_MAKE_OPERABLE__(subtractable, -, N)
CAPO_MAKE_OPERABLE__(multipliable, *, Y)
CAPO_MAKE_OPERABLE__(dividable   , /, N)
CAPO_MAKE_OPERABLE__(modable     , %, N)
CAPO_MAKE_OPERABLE__(xorable     , ^, Y)
CAPO_MAKE_OPERABLE__(andable     , &, Y)
CAPO_MAKE_OPERABLE__(orable      , |, Y)

#pragma pop_macro("CAPO_MAKE_OPERABLE__")
#pragma pop_macro("CAPO_MAKE_OP_DIFF__")
#pragma pop_macro("CAPO_MAKE_OP_DIFF_COMM__N__")
#pragma pop_macro("CAPO_MAKE_OP_DIFF_COMM__Y__")
#pragma pop_macro("CAPO_MAKE_OP_SAME__")

////////////////////////////////////////////////////////////////

} // namespace capo
