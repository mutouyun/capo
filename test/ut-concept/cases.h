#pragma once

TEST_METHOD(has)
{
    using namespace ut_concept_;

    EXPECT_EQ(true , has_foo_type<Foo>::value);
    EXPECT_EQ(false, has_foo_type<Bar>::value);
    EXPECT_EQ(false, has_bar_func<Foo>::value);
    EXPECT_EQ(true , has_bar_func<Bar>::value);
}

TEST_METHOD(is_concept)
{
    using namespace ut_concept_;

    EXPECT_EQ(true , check(Foo{}));
    EXPECT_EQ(true , check(Bar{}));
    EXPECT_EQ(false, check(XXX{}));
    EXPECT_EQ(false, check(int{}));
}
