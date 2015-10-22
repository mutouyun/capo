
#include "gtest/gtest.h"

#include "capo/concept.hpp"

////////////////////////////////////////////////////////////////

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

template <typename T, bool B = true>
CAPO_CONCEPT_(FooBarType, B ? (has_foo_type<T>::value || has_bar_func<T>::value) : 
                            ! (has_foo_type<T>::value || has_bar_func<T>::value));

#define TEST_METHOD(TEST_NAME) TEST(concept, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(has)
{
    EXPECT_EQ(true , has_foo_type<Foo>::value);
    EXPECT_EQ(false, has_foo_type<Bar>::value);
    EXPECT_EQ(false, has_bar_func<Foo>::value);
    EXPECT_EQ(true , has_bar_func<Bar>::value);
}

template <typename T> constexpr bool check(T&&, FooBarType<T>*        = nullptr) { return true ; }
template <typename T> constexpr bool check(T&&, FooBarType<T, false>* = nullptr) { return false; }

TEST_METHOD(is_concept)
{
    EXPECT_EQ(true , check(Foo{}));
    EXPECT_EQ(true , check(Bar{}));
    EXPECT_EQ(false, check(XXX{}));
    EXPECT_EQ(false, check(int{}));
}
