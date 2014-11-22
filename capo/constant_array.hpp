/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/type_list.hpp"

#include <type_traits> // std::integral_constant

namespace capo {

////////////////////////////////////////////////////////////////
/// Constant Array
////////////////////////////////////////////////////////////////

template <typename T>
class constant_array
{
    template <typename...>
    struct type_list_;

    template <T... N>
    struct type_list_<std::integral_constant<T, N>...>
    {
        static const T value[sizeof...(N)];
    };

    template <template <T> class Do_>
    struct do_wrapper_
    {
        template <typename T_>
        using type = std::integral_constant<T, Do_<T_::value>::value>;
    };

    template <template <T, T> class If_>
    struct if_wrapper_
    {
        template <typename T_, typename U_>
        using type = std::integral_constant<T, If_<T_::value, U_::value>::value>;
    };

    template <T X, T Y>
    struct is_less_ : std::integral_constant<T, (X < Y)>
    {};

    template <T X, T Y>
    struct is_greater_ : std::integral_constant<T, (X > Y)>
    {};

public:
    template <T... N>
    using type = type_list_<std::integral_constant<T, N>...>;

    template <typename Arr>
    using size = types_size<Arr>;

    template <typename Arr>
    using empty = types_empty<Arr>;

    template <typename Arr, int IndexN>
    using at = typename types_at<Arr, IndexN>::type;

    template <typename Arr>
    using front = typename types_front<Arr>::type;

    template <typename Arr>
    using back = typename types_back<Arr>::type;

    template <typename A1, typename A2>
    using link = typename types_link<A1, A2>::type;

    template <int Count, T N>
    using assign = typename types_assign<Count, constant_array<T>::type<N>>::type;

    template <typename Arr, int IndexN, T N>
    using insert = typename types_insert<Arr, IndexN, std::integral_constant<T, N>>::type;

    template <typename Arr, int IndexN, int CountN = 1>
    using erase = typename types_erase<Arr, IndexN, CountN>::type;

    template <typename Arr, T N>
    using find = types_find<Arr, std::integral_constant<T, N>>;

    template <typename Arr, T N>
    using exist = types_exist<Arr, std::integral_constant<T, N>>;

    template <typename Arr, template <T> class Do_>
    using foreach = typename types_foreach<Arr, do_wrapper_<Do_>::template type>::type;

    template <typename Arr, T N1, T N2>
    using replace = typename types_replace<Arr, std::integral_constant<T, N1>
                                              , std::integral_constant<T, N2>>::type;

    template <typename Arr, T N>
    using remove = typename types_remove<Arr, std::integral_constant<T, N>>::type;

    template <typename Arr>
    using compact = typename types_compact<Arr>::type;

    template <typename Arr>
    using reverse = typename types_reverse<Arr>::type;

    template <typename Arr, template <T, T> class If_>
    using sort_if = typename types_sort_if<Arr, if_wrapper_<If_>::template type>::type;

    template <typename Arr>
    using sort_less = sort_if<Arr, is_less_>;

    template <typename Arr>
    using sort_greater = sort_if<Arr, is_greater_>;
};

template <typename T>
template <T... N>
const T constant_array<T>::type_list_<std::integral_constant<T, N>...>::value[sizeof...(N)] = { N... };

////////////////////////////////////////////////////////////////

} // namespace capo
