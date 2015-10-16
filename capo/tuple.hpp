/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/types_to_seq.hpp"
#include "capo/constant_array.hpp"

#include <tuple>        // std::tuple, std::tuple_element, std::get, ...
#include <utility>      // std::forward
#include <type_traits>  // std::enable_if
#include <cstddef>      // size_t

namespace capo {

////////////////////////////////////////////////////////////////

/*
    capo::forward_as_tuple<3>(1, 2, 3, 4) -> std::tuple<int&&, int&&, int&&>{ 1, 2, 3 }
*/

namespace detail_forward_as_tuple_ {

template <int... M, typename... A>
std::tuple<> impl_(constant_seq<>, constant_seq<M...>, A&&...) { return {}; }

template <int N1, int... N, int M1, int... M, typename A1, typename... A,
          typename = typename std::enable_if<(sizeof...(N) < sizeof...(M))>::type>
auto impl_(constant_seq<N1, N...>, constant_seq<M1, M...>, A1&& a, A&&... args)
{
    return std::tuple_cat(std::forward_as_tuple(std::forward<A1>(a)), 
                          impl_(constant_seq<N...>{}, constant_seq<M...>{}, std::forward<A>(args)...));
}

template <int... N, int... M, typename... A,
          typename = typename std::enable_if<(sizeof...(N) == sizeof...(M))>::type>
auto impl_(constant_seq<N...>, constant_seq<M...>, A&&... args)
{
    return std::forward_as_tuple(std::forward<A>(args)...);
}

} // namespace detail_forward_as_tuple_

template <size_t ForwardN, typename... A>
auto forward_as_tuple(A&&... args)
{
    return detail_forward_as_tuple_::impl_(size_to_seq<ForwardN>{}, types_to_seq<A...>{}, std::forward<A>(args)...);
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
