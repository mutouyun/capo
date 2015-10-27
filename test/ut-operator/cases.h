#pragma once

TEST_METHOD(operator_case)
{
    using namespace ut_operator_;
    Foo foo(5);
    foo = foo + foo + 1 - 2 + foo + 3 / foo * foo;
    EXPECT_EQ(14, foo.x_);
}
