#pragma once

#include "capo/concept.hpp"

namespace ut_concept_ {

class Foo
{
public:
    using foo_type = void;
};

class Bar
{
public:
    void bar_func(int) const;
};

class XXX
{
};

CAPO_CONCEPT_INNER_TYPE_(foo_type);
CAPO_CONCEPT_MEMBER_(bar_func, void (C::*)(int) const);

template <typename T>
CAPO_CONCEPT_(FooBarType, has_foo_type<typename std::remove_reference<T>::type>::value || 
                          has_bar_func<typename std::remove_reference<T>::type>::value);

template <typename T, CAPO_REQUIRE_(FooBarType<T>::value)>
constexpr auto check(T&&) -> bool { return true ; }
template <typename T, CAPO_REQUIRE_(!FooBarType<T>::value)>
constexpr auto check(T&&) -> bool { return false; }

} // namespace ut_concept_
