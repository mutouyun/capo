
#include "gtest/gtest.h"

#include "capo/operator.hpp"

////////////////////////////////////////////////////////////////

struct Foo : capo::inherit_chain<Foo, capo::addable
                                    , capo::subtractable
                                    , capo::multipliable
                                    , capo::dividable>
{
    int x_;

    Foo(void)  : x_(0) {}
    Foo(int x) : x_(x) {}

    Foo& operator+=(const Foo& rhs) { x_ += rhs.x_; return (*this); }
    Foo& operator-=(const Foo& rhs) { x_ -= rhs.x_; return (*this); }

    Foo& operator*=(const Foo& rhs) { x_ *= rhs.x_; return (*this); }
    Foo& operator/=(const Foo& rhs) { x_ /= rhs.x_; return (*this); }
};

#define TEST_METHOD(TEST_NAME) TEST(operator, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(operator)
{
    Foo foo(5);
    foo = foo + foo + 1 - 2 + foo + 3 / foo * foo;
    EXPECT_EQ(14, foo.x_);
}
