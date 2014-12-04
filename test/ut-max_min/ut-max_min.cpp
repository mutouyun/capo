
#include "gtest/gtest.h"

#include "capo/max_min.hpp"

////////////////////////////////////////////////////////////////

#define TEST_METHOD(TEST_NAME) TEST(max_min, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(max)
{
    EXPECT_EQ(3, capo::max(1, 3, 2));
}

TEST_METHOD(min)
{
    EXPECT_EQ(1, capo::min(1, 3, 2));
}
