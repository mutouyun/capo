/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/type_list.hpp"

#include <type_traits>  // std::integral_constant
#include <cstddef>      // size_t

namespace capo {

////////////////////////////////////////////////////////////////

template <int... N>
struct constant_seq : std::integral_constant<size_t, sizeof...(N)>
{
    using next = constant_seq<N..., sizeof...(N)>;
};

namespace detail_size_to_seq_ {

template <size_t N>
struct impl_
{
    using type = typename impl_<N - 1>::type::next;
};

template <>
struct impl_<0>
{
    using type = constant_seq<>;
};

} // namespace detail_size_to_seq_

template <size_t N>
using size_to_seq = typename detail_size_to_seq_::impl_<N>::type;

template <typename... T>
using types_to_seq = size_to_seq<sizeof...(T)>;

template <typename TypesT>
using list_to_seq = size_to_seq<capo::types_size<TypesT>::value>;

////////////////////////////////////////////////////////////////

} // namespace capo
