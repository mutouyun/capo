/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_TYPE_CONCEPT_HPP___
#define CAPO_TYPE_CONCEPT_HPP___

#include <type_traits>  // std::true_type, std::false_type

////////////////////////////////////////////////////////////////

/*
    Make a concept for checking inner type
*/

#define CAPO_CONCEPT_INNER_TYPE(INNER_TYPE) \
    struct has_##INNER_TYPE##_inner_type_checker___ \
    { \
        template <typename T> \
        static std::true_type check(typename T::INNER_TYPE*); \
        template <typename T> \
        static std::false_type check(...); \
    }; \
    template <typename T> \
    struct has_##INNER_TYPE \
        : decltype(has_##INNER_TYPE##_inner_type_checker___::check<T>(nullptr)) \
    {}

/*
    Make a concept for checking member function
*/

#define CAPO_CONCEPT_MEMBER_FUNCTION(FUNC_NAME, POINTER_TYPE) \
    struct has_##FUNC_NAME##_member_func_checker___ \
    { \
        template <class C, POINTER_TYPE> \
        struct func_test; \
        template <typename T> \
        static std::true_type check(func_test<T, &T::FUNC_NAME>*); \
        template <typename T> \
        static std::false_type check(...); \
    }; \
    template <typename T> \
    struct has_##FUNC_NAME \
        : decltype(has_##FUNC_NAME##_member_func_checker___::check<T>(nullptr)) \
    {}

////////////////////////////////////////////////////////////////

#endif // CAPO_TYPE_LIST_HPP___
