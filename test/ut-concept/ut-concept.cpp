
#include "gtest/gtest.h"

#include "capo/concept.hpp"

////////////////////////////////////////////////////////////////

class Foo
{
public:
    using foo_type = void;
};

CAPO_CONCEPT_INNER_TYPE_(foo_type);

class Bar
{
public:
    void bar_func(int) const;
};

CAPO_CONCEPT_MEMBER_FUNCTION_(bar_func, void (C::*)(int) const);

#define TEST_METHOD(TEST_NAME) TEST(concept, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(concept)
{
    EXPECT_EQ(true , has_foo_type<Foo>::value);
    EXPECT_EQ(false, has_foo_type<Bar>::value);
    EXPECT_EQ(false, has_bar_func<Foo>::value);
    EXPECT_EQ(true , has_bar_func<Bar>::value);
}
