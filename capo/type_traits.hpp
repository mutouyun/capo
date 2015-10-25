/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/concept.hpp"

#include <type_traits>  // std::remove_cv, std::remove_reference, std::decay, ...
#include <tuple>        // std::tuple
#include <functional>   // std::function
#include <cstddef>      // size_t

namespace capo {

////////////////////////////////////////////////////////////////

/*
    Type qualifiers & reference removing
*/

template <typename T>
using underlying = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

/*
    Functor & closure checking
*/

CAPO_CONCEPT_TYPING_(is_functor_object, &T::operator());

template <typename T>
using is_functor = is_functor_object<underlying<T>>;

template <typename T>
using is_closure = std::integral_constant<bool, 
     (is_functor<T>::value || std::is_function<typename std::remove_pointer<underlying<T>>::type>::value)>;

/*
    Function type traits
*/

namespace detail_function_traits_ {

template <typename F, bool = is_closure<F>::value>
struct impl_
{
    enum : size_t { arity = 0 };
    typedef void type;
    typedef void return_type;
    typedef std::tuple<> parameters;
};

// check functor

template <typename F>
struct impl_<F, true>
     : impl_<decltype(&F::operator())>
{};

// check pointer

template <typename T, bool B>
struct impl_<T*, B>
     : impl_<T>
{};

// check function pointer

template <typename R, typename... P>
struct impl_<R(*)(P...), true>
     : impl_<R(P...)>
{};

template <typename R, typename... P>
struct impl_<R(*)(P..., ...), true>
     : impl_<R(P..., ...)>
{};

// check member function pointer

#pragma push_macro("CAPO_FUNCTION_TRAITS_IMPL__")
#undef  CAPO_FUNCTION_TRAITS_IMPL__
#define CAPO_FUNCTION_TRAITS_IMPL__(...)                \
    template <typename R, typename C, typename... P>    \
    struct impl_<R(C::*)(P...) __VA_ARGS__, false>      \
         : impl_<R(P...)>                               \
    {};                                                 \
    template <typename R, typename C, typename... P>    \
    struct impl_<R(C::*)(P..., ...) __VA_ARGS__, false> \
         : impl_<R(P..., ...)>                          \
    {};

CAPO_FUNCTION_TRAITS_IMPL__()
CAPO_FUNCTION_TRAITS_IMPL__(const)
CAPO_FUNCTION_TRAITS_IMPL__(volatile)
CAPO_FUNCTION_TRAITS_IMPL__(const volatile)

#pragma pop_macro("CAPO_FUNCTION_TRAITS_IMPL__")

// check std::function

template <typename R, typename... P>
struct impl_<std::function<R(P...)>, true>
     : impl_<R(P...)>
{};

template <typename R, typename... P>
struct impl_<std::function<R(P..., ...)>, true>
     : impl_<R(P..., ...)>
{};

// check function type

template <typename R, typename... P>
struct impl_<R(P...), true>
{
    enum : size_t
    {
        arity = sizeof...(P)
    };
    typedef R type(P...);
    typedef R return_type;
    typedef std::tuple<P...> parameters;
};

template <typename R, typename... P>
struct impl_<R(P..., ...), true>
{
    enum : size_t
    {
        arity = sizeof...(P)
    };
    typedef R type(P..., ...);
    typedef R return_type;
    typedef std::tuple<P...> parameters;
};

} // namespace detail_function_traits_

template <typename F>
struct function_traits
     : detail_function_traits_::impl_<typename std::decay<F>::type>
{};

/*
    The concept for checking different underlying types
*/

template <typename T, typename U>
CAPO_CONCEPT_(Different, !std::is_same<capo::underlying<T>, capo::underlying<U>>::value);

////////////////////////////////////////////////////////////////

} // namespace capo
