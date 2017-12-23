/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/types_to_seq.hpp"

#include <tuple>        // std::tuple, std::tuple_element, std::get, ...
#include <utility>      // std::forward, std::move
#include <type_traits>  // std::enable_if, std::integral_constant
#include <cstddef>      // size_t

namespace capo {

////////////////////////////////////////////////////////////////

/*
    capo::forward_as_tuple<3>(1, 2, 3, 4) -> std::tuple<int&&, int&&, int&&>{ 1, 2, 3 }
*/

namespace detail_forward_as_tuple_ {

template <int... M, typename... A>
std::tuple<> impl_(constant_seq<>, constant_seq<M...>, A&&...) { return {}; }

template <int... N, typename... A>
auto impl_(constant_seq<N...>, std::tuple<A...>&& tp, typename std::enable_if<(sizeof...(N) < sizeof...(A))>::type* = nullptr)
{
    return std::forward_as_tuple(std::get<N>(std::move(tp))...);
}

template <int... N, typename... A>
auto impl_(constant_seq<N...>, std::tuple<A...>&& tp, typename std::enable_if<(sizeof...(N) == sizeof...(A))>::type* = nullptr)
{
    return std::move(tp);
}

} // namespace detail_forward_as_tuple_

template <size_t ForwardN, typename... A>
auto forward_as_tuple(A&&... args)
{
    return detail_forward_as_tuple_::impl_(size_to_seq<ForwardN>{}, std::forward_as_tuple(std::forward<A>(args)...));
}

/*
    capo::numbers_to_tuple(capo::constant_array<int>::assign<3, 1>{}) -> std::tuple<int, int, int>{ 1, 1, 1 }
*/

template <typename T, T... N, template <typename...> class Tp>
auto numbers_to_tuple(Tp<std::integral_constant<T, N>...>)
{
    return std::make_tuple(N...);
}

////////////////////////////////////////////////////////////////

} // namespace capo
