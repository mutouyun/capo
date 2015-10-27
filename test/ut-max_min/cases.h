#pragma once

TEST_METHOD(max)
{
    EXPECT_EQ(3, capo::max(1, 3, 2));
}

TEST_METHOD(min)
{
    EXPECT_EQ(1, capo::min(1, 3, 2));
}
