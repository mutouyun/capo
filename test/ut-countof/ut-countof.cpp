
#include "gtest/gtest.h"

#include "capo/countof.hpp"

////////////////////////////////////////////////////////////////

void* p[100];

#define TEST_METHOD(TEST_NAME) TEST(countof, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(countof)
{
    EXPECT_EQ(100, capo::countof(p));
}

TEST_METHOD(constexpr)
{
    int arr[capo::countof(p)];
    EXPECT_EQ(capo::countof(p), capo::countof(arr));
}
