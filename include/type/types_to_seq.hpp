/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_TYPES_TO_SEQ_HPP___
#define CAPO_TYPES_TO_SEQ_HPP___

namespace capo {

template <int... N>
struct constant_seq
{
    using next = constant_seq<N..., sizeof...(N)>;
};

namespace detail_types_to_seq
{
    template <typename... T>
    struct impl;

    template <typename T1, typename... T>
    struct impl<T1, T...>
    {
        using type = typename impl<T...>::type::next;
    };

    template <>
    struct impl<>
    {
        using type = constant_seq<>;
    };
} // namespace detail_types_to_seq

template <typename... T>
using types_to_seq = typename detail_types_to_seq::impl<T...>::type;

namespace detail_list_to_seq
{
    template <typename TypesT>
    struct impl;

    template <typename... T,
              template <typename...> class TypesT>
    struct impl<TypesT<T...>>
    {
        using type = types_to_seq<T...>;
    };
} // namespace detail_types_to_seq

template <typename TypesT>
using list_to_seq = typename detail_list_to_seq::impl<TypesT>::type;

} // namespace capo

#endif // CAPO_TYPES_TO_SEQ_HPP___
