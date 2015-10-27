#pragma once

TEST_METHOD(ensure)
{
    using namespace ut_assert_;
    EXPECT_THROW(func_1(), capo::assert::context);
    EXPECT_THROW(func_2(), bool);
}
