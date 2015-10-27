#pragma once

TEST_METHOD(countof)
{
    using namespace ut_countof_;
    EXPECT_EQ((size_t)100, capo::countof(p));
}

TEST_METHOD(constexpr_countof)
{
    using namespace ut_countof_;
    int arr[capo::countof(p)];
    EXPECT_EQ(capo::countof(p), capo::countof(arr));
}
