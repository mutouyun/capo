#pragma once

TEST_METHOD(double_create)
{
    using namespace ut_singleton_;
    auto* p1 = &capo::singleton<XXX>(123);
    auto* p2 = &capo::singleton<XXX>();
    EXPECT_EQ((size_t)p1, (size_t)p2);
}
