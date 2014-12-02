/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include <type_traits> // std::integral_constant

namespace capo {

////////////////////////////////////////////////////////////////
/// Define the type-list
////////////////////////////////////////////////////////////////

template <typename...>
struct type_list {};

template <typename... T>
using types = type_list<T...>;

/*
    Is type-list
*/

template <typename TypesT>
struct is_types
     : std::false_type
{};

template <typename... T,
          template <typename...> class TypesT>
struct is_types<TypesT<T...>>
     : std::true_type
{};

// Check is type-list or not

template <typename TypesT>
struct check_is_types
{
    static_assert(is_types<TypesT>::value, "The template parameter is not a type-list!");
};

////////////////////////////////////////////////////////////////
/// Capacity
////////////////////////////////////////////////////////////////

/*
    Return size
*/

template <typename TypesT>
struct types_size : std::integral_constant<int, 0>
                  , check_is_types<TypesT>
{};

template <typename... T,
          template <typename...> class TypesT>
struct types_size<TypesT<T...>>
     : std::integral_constant<int, sizeof...(T)>
{};

// Check is index valid or not

template <typename TypesT, int IndexN>
struct check_is_index_valid
{
    static_assert(IndexN >= 0,                        "Index is out of range!");
    static_assert(IndexN < types_size<TypesT>::value, "Index is out of range!");
};

// Check is count valid or not

template <typename TypesT, int CountN>
struct check_is_count_valid
{
    static_assert(CountN > 0,                          "Count is too small!");
    static_assert(CountN <= types_size<TypesT>::value, "Count is too large!");
};

/*
    Test whether types is empty
*/

template <typename TypesT>
struct types_empty : std::true_type
                   , check_is_types<TypesT>
{};

template <typename... T,
          template <typename...> class TypesT>
struct types_empty<TypesT<T...>>
     : std::false_type
{};

template <template <typename...> class TypesT>
struct types_empty<TypesT<>>
     : std::true_type
{};

////////////////////////////////////////////////////////////////
/// Element access
////////////////////////////////////////////////////////////////

/*
    Access element
*/

template <typename TypesT, int IndexN>
struct types_at : check_is_index_valid<TypesT, IndexN>
{
    using type = TypesT;
};

template <typename T1, typename... T, int N,
          template <typename...> class TypesT>
struct types_at<TypesT<T1, T...>, N>
     : types_at<TypesT<T...>, N - 1>
{};

template <typename T1, typename... T,
          template <typename...> class TypesT>
struct types_at<TypesT<T1, T...>, 0>
{
    using type = T1;
};

template <typename TypesT, int IndexN>
using types_at_t = typename types_at<TypesT, IndexN>::type;

/*
    Access first element
*/

template <typename TypesT>
using types_front = types_at<TypesT, 0>;

template <typename TypesT>
using types_front_t = typename types_front<TypesT>::type;

/*
    Access last element
*/

template <typename TypesT>
using types_back = types_at<TypesT, types_size<TypesT>::value - 1>;

template <typename TypesT>
using types_back_t = typename types_back<TypesT>::type;

////////////////////////////////////////////////////////////////
/// Assign
////////////////////////////////////////////////////////////////

/*
    Link 2 types together
*/

template <typename T, typename U>
struct types_link
{
    using type = types<T, U>;
};

template <typename... T, typename... U,
          template <typename...> class TypesT,
          template <typename...> class TypesU>
struct types_link<TypesT<T...>, TypesU<U...>>
{
    using type = TypesT<T..., U...>;
};

template <typename... T, typename U,
          template <typename...> class TypesT>
struct types_link<TypesT<T...>, U>
{
    using type = TypesT<T..., U>;
};

template <typename T, typename... U,
          template <typename...> class TypesT>
struct types_link<T, TypesT<U...>>
{
    using type = TypesT<T, U...>;
};

template <typename... T, template <typename...> class TypesT>
struct types_link<TypesT<T...>, types<>>
{
    using type = TypesT<T...>;
};

template <typename... U, template <typename...> class TypesT>
struct types_link<types<>, TypesT<U...>>
{
    using type = TypesT<U...>;
};

template <>
struct types_link<types<>, types<>>
{
    using type = types<>;
};

template <typename T, typename U>
using types_link_t = typename types_link<T, U>::type;

/*
    Assign types content
*/

template <int N, typename T>
struct types_assign
{
    static_assert(N >= 0, "N cannot be less than 0!");
private:
    using tail = typename types_assign<N - 1, T>::type;
public:
    using type = typename types_link<T, tail>::type;
};

template <typename T>
struct types_assign<0, T>
{
    using type = types<>;
};

template <typename... T,
          template <typename...> class TypesT>
struct types_assign<0, TypesT<T...>>
{
    using type = TypesT<>;
};

template <int N, typename T>
using types_assign_t = typename types_assign<N, T>::type;

////////////////////////////////////////////////////////////////
/// Modifiers
////////////////////////////////////////////////////////////////

/*
    Insert elements
*/

template <typename TypesT, int IndexN, typename T>
struct types_insert : check_is_types<TypesT>
                    , check_is_index_valid<TypesT, IndexN>
{
    using type = TypesT;
};

template <typename T1, typename... T, int N, typename U,
          template <typename...> class TypesT>
struct types_insert<TypesT<T1, T...>, N, U>
{
private:
    using tail = typename types_insert<TypesT<T...>, N - 1, U>::type;
public:
    using type = typename types_link<T1, tail>::type;
};

template <typename T1, typename... T, typename U,
          template <typename...> class TypesT>
struct types_insert<TypesT<T1, T...>, 0, U>
{
    using type = typename types_link<U, TypesT<T1, T...>>::type;
};

#if defined(_MSC_VER) && (_MSC_VER <= 1900)
/*
    <MSVC 2013> error C2338: Index is out of range!
    Cause:
        'capo::types_insert<capo::type_list<>, 0, U>' will matching
        'capo::types_insert<TypesT, 0, U>'
*/
template <typename U>
struct types_insert<types<>, 0, U>
{
    using type = typename types_link<U, types<>>::type;
};
#else /*!_MSC_VER*/
template <typename U,
          template <typename...> class TypesT>
struct types_insert<TypesT<>, 0, U>
{
    using type = typename types_link<U, TypesT<>>::type;
};
#endif/*!_MSC_VER*/

template <typename TypesT, int IndexN, typename T>
using types_insert_t = typename types_insert<TypesT, IndexN, T>::type;

/*
    Erase elements
*/

template <typename TypesT, int IndexN, int CountN = 1>
struct types_erase : check_is_types<TypesT>
                   , check_is_index_valid<TypesT, IndexN>
{
    using type = TypesT;
};

template <typename T1, typename... T, int N, int C,
          template <typename...> class TypesT>
struct types_erase<TypesT<T1, T...>, N, C>
{
private:
    using tail = typename types_erase<TypesT<T...>, N - 1, C>::type;
public:
    using type = typename types_link<T1, tail>::type;
};

template <typename T1, typename... T, int C,
          template <typename...> class TypesT>
struct types_erase<TypesT<T1, T...>, 0, C>
     : check_is_count_valid<TypesT<T1, T...>, C>
{
    using type = typename types_erase<TypesT<T...>, 0, C - 1>::type;
};

template <typename T1, typename... T,
          template <typename...> class TypesT>
struct types_erase<TypesT<T1, T...>, 0, 1>
{
    using type = TypesT<T...>;
};

template <typename TypesT, int IndexN, int CountN = 1>
using types_erase_t = typename types_erase<TypesT, IndexN, CountN>::type;

////////////////////////////////////////////////////////////////
/// Algorithms
////////////////////////////////////////////////////////////////

/*
    Find a type
*/

template <typename TypesT, typename T>
struct types_find : std::integral_constant<int, -1>
                  , check_is_types<TypesT>
{};

template <typename T1, typename... T, typename U,
          template <typename...> class TypesT>
struct types_find<TypesT<T1, T...>, U>
     : std::integral_constant<int, 
                             (types_find<TypesT<T...>, U>::value == -1 ? -1 :
                              types_find<TypesT<T...>, U>::value + 1)>
{};

template <typename T1, typename... T,
          template <typename...> class TypesT>
struct types_find<TypesT<T1, T...>, T1>
     : std::integral_constant<int, 0>
{};

/*
    Check type is in the types
*/

template <typename TypesT, typename T>
using types_exist = std::integral_constant<bool, (types_find<TypesT, T>::value != -1)>;

/*
    For each one in the types, do something
*/

template <typename TypesT,
          template <typename> class Do_>
struct types_foreach : check_is_types<TypesT>
{
    using type = TypesT;
};

template <typename T1, typename... T,
          template <typename> class Do_,
          template <typename...> class TypesT>
struct types_foreach<TypesT<T1, T...>, Do_>
{
private:
    using tail = typename types_foreach<TypesT<T...>, Do_>::type;
    using done = typename Do_<T1>::type;
public:
    using type = typename types_link<done, tail>::type;
};

/*
    For each one in the types, do something if the conditional is true
*/

template <template <typename, typename> class If_, typename V,
          template <bool, typename, typename> class Do_, typename U>
struct do_if
{
    template <typename T>
    using type = Do_<If_<T, V>::value, U, T>;
};

template <typename TypesT,
          template <typename, typename> class If_, typename V,
          template <bool, typename, typename> class Do_, typename U>
using types_do_if = types_foreach<TypesT, do_if<If_, V, Do_, U>::template type>;

/*
    Completely replace a type from the types to another type
*/

template <typename TypesT, typename T, typename U>
using types_replace = types_do_if<TypesT, std::is_same, T, std::conditional, U>;

template <typename TypesT, typename T, typename U>
using types_replace_t = typename types_replace<TypesT, T, U>::type;

/*
    Completely remove a type from the types
*/

template <typename TypesT, typename T>
struct types_remove
     : types_replace<TypesT, T, types<>>
{};

template <typename TypesT, typename U1, typename... U,
          template <typename...> class TypesU>
struct types_remove<TypesT, TypesU<U1, U...>>
{
private:
    using rm_t = typename types_remove<TypesT, U1>::type;
public:
    using type = typename types_remove<rm_t, TypesU<U...>>::type;
};

template <typename TypesT, typename T>
using types_remove_t = typename types_remove<TypesT, T>::type;

/*
    Remove duplicate types
*/

template <typename TypesT>
struct types_compact : check_is_types<TypesT>
{
    using type = TypesT;
};

template <typename T1, typename... T,
          template <typename...> class TypesT>
struct types_compact<TypesT<T1, T...>>
{
private:
    using rm_t = typename types_remove<TypesT<T...>, T1>::type;
    using tail = typename types_compact<rm_t>::type;
public:
    using type = typename types_link<T1, tail>::type;
};

template <typename TypesT>
using types_compact_t = typename types_compact<TypesT>::type;

/*
    Reverse the types
*/

template <class TypesT>
struct types_reverse : check_is_types<TypesT>
{
    using type = TypesT;
};

template <typename T1, typename... T,
          template <typename...> class TypesT>
struct types_reverse<TypesT<T1, T...>>
{
private:
    using head = typename types_reverse<TypesT<T...>>::type;
public:
    using type = typename types_link<head, T1>::type;
};

template <typename TypesT>
using types_reverse_t = typename types_reverse<TypesT>::type;

/*
    Select the most satisfactory type
*/

template <typename TypesT,
          template <typename, typename> class If_>
struct types_select_if : check_is_types<TypesT>
{
    using type = TypesT;
};

template <typename T1, typename... T,
          template <typename, typename> class If_,
          template <typename...> class TypesT>
struct types_select_if<TypesT<T1, T...>, If_>
{
private:
    using select_t = typename types_select_if<TypesT<T...>, If_>::type;
public:
    using type = typename std::conditional<If_<T1, select_t>::value, T1, select_t>::type;
};

template <typename T1,
          template <typename, typename> class If_,
          template <typename...> class TypesT>
struct types_select_if<TypesT<T1>, If_>
{
    using type = T1;
};

template <typename TypesT,
          template <typename, typename> class If_>
using types_select_if_t = typename types_select_if<TypesT, If_>::type;

/*
    Sort types
*/

template <class TypesT,
          template <typename, typename> class If_>
struct types_sort_if : check_is_types<TypesT>
{
    using type = TypesT;
};

template <typename T1, typename... T,
          template <typename, typename> class If_,
          template <typename...> class TypesT>
struct types_sort_if<TypesT<T1, T...>, If_>
{
private:
    using types_t = TypesT<T1, T...>;
    using sl_t = typename types_select_if<types_t, If_>::type;
    using er_t = typename types_erase<types_t, types_find<types_t, sl_t>::value>::type;
    using tail = typename types_sort_if<er_t, If_>::type;
public:
    using type = typename types_link<sl_t, tail>::type;
};

template <typename T1,
          template <typename, typename> class If_,
          template <typename...> class TypesT>
struct types_sort_if<TypesT<T1>, If_>
{
    using type = TypesT<T1>;
};

template <class TypesT,
          template <typename, typename> class If_>
using types_sort_if_t = typename types_sort_if<TypesT, If_>::type;

////////////////////////////////////////////////////////////////

} // namespace capo