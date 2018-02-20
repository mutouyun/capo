#pragma once

TEST_METHOD(double_create)
{
    using namespace ut_singleton_;
    auto* p1 = &capo::singleton<XXX>(123);
    auto* p2 = &capo::singleton<XXX>();
    EXPECT_EQ((size_t)p1, (size_t)p2);
}

TEST_METHOD(multi_thread)
{
    using namespace ut_singleton_;
    std::future<XXX*> p[10];
    for (size_t i = 0; i < capo::countof(p); ++i)
    {
        p[i] = std::async(std::launch::async, [i]
        {
            return &capo::singleton<XXX>(i);
        });
    }
    auto* p0 = p[0].get();
    for (size_t i = 1; i < capo::countof(p); ++i)
    {
        EXPECT_EQ((size_t)p0, (size_t)p[i].get());
    }
}