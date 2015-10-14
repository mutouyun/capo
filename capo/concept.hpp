/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include <type_traits>  // std::true_type, std::false_type

////////////////////////////////////////////////////////////////

/*
    Make a concept for checking inner type
*/

#define CAPO_CONCEPT_INNER_TYPE_(INNER_TYPE)                                   \
    struct has_##INNER_TYPE##_inner_type_checker__                             \
    {                                                                          \
        template <typename T>                                                  \
        static std::true_type check(typename T::INNER_TYPE*);                  \
        template <typename T>                                                  \
        static std::false_type check(...);                                     \
    };                                                                         \
    template <typename T>                                                      \
    struct has_##INNER_TYPE                                                    \
        : decltype(has_##INNER_TYPE##_inner_type_checker__::check<T>(nullptr)) \
    {}

/*
    Make a concept for checking member function
*/

#define CAPO_CONCEPT_MEMBER_FUNCTION_(FUNC_NAME, POINTER_TYPE)                 \
    struct has_##FUNC_NAME##_member_func_checker__                             \
    {                                                                          \
        template <class C, POINTER_TYPE>                                       \
        struct func_test;                                                      \
        template <typename T>                                                  \
        static std::true_type check(func_test<T, &T::FUNC_NAME>*);             \
        template <typename T>                                                  \
        static std::false_type check(...);                                     \
    };                                                                         \
    template <typename T>                                                      \
    struct has_##FUNC_NAME                                                     \
        : decltype(has_##FUNC_NAME##_member_func_checker__::check<T>(nullptr)) \
    {}

/*
    Make a concept for checking combined conditions.
*/

#define CAPO_CONCEPT_(CONCEPT_NAME, ...)                                       \
    template <typename T>                                                      \
    struct CONCEPT_NAME : std::integral_constant<bool, (__VA_ARGS__)> {}

namespace capo {

template <template <typename> class ConceptT, typename T>
struct is_concept : ConceptT<T> {};

} // namespace capo

////////////////////////////////////////////////////////////////
