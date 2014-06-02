/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_TYPE_CONCEPT_HPP___
#define CAPO_TYPE_CONCEPT_HPP___

#include <type_traits> // std::is_same

namespace capo {

namespace detail_concept
{
    using not_t = char;
    using yes_t = struct { not_t dummy_[2]; };

    template <typename T>
    struct check : std::is_same<T, yes_t> {};
}

#define CAPO_CONCEPT_INNER_TYPE(INNER_TYPE) \
    struct has_##INNER_TYPE##_inner_type_checker___ \
    { \
        template <typename T> \
        static capo::detail_concept::yes_t check(typename T::INNER_TYPE*); \
        template <typename T> \
        static capo::detail_concept::not_t check(...); \
    }; \
    template <typename T> \
    struct has_##INNER_TYPE : capo::detail_concept::check \
    <decltype(has_##INNER_TYPE##_inner_type_checker___::check<T>(nullptr))> {}

#define CAPO_CONCEPT_MEMBER_FUNCTION(FUNC_NAME, POINTER_TYPE) \
    struct has_##FUNC_NAME##_member_func_checker___ \
    { \
        template <class C, POINTER_TYPE> \
        struct func_test; \
        template <typename T> \
        static capo::detail_concept::yes_t check(func_test<T, &T::FUNC_NAME>*); \
        template <typename T> \
        static capo::detail_concept::not_t check(...); \
    }; \
    template <typename T> \
    struct has_##FUNC_NAME : capo::detail_concept::check \
    <decltype(has_##FUNC_NAME##_member_func_checker___::check<T>(nullptr))> {}

} // namespace capo

#endif // CAPO_TYPE_LIST_HPP___
