/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include <type_traits>

namespace capo {

template <typename T>
struct underlying : std::remove_cv<typename std::remove_reference<T>::type> {};

struct is_functor_
{
    template <typename T> static std::true_type  check(decltype(&T::operator())*);
    template <typename T> static std::false_type check(...);
};

template <typename T>
struct is_functor : decltype(is_functor_::check<T>(nullptr)) {};

template <typename T, bool = std::is_function<typename std::remove_pointer<T>::type>::value || is_functor<T>::value>
struct is_closure_;
template <typename T> struct is_closure_<T, true>  : std::true_type  {};
template <typename T> struct is_closure_<T, false> : std::false_type {};

template <typename T>
struct is_closure : is_closure_<typename underlying<T>::type> {};

} // namespace capo
