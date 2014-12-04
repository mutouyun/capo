
#include "gtest/gtest.h"

#include "capo/sequence.hpp"
#include "capo/countof.hpp"

////////////////////////////////////////////////////////////////

#define TEST_METHOD(TEST_NAME) TEST(sequence, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(sequence)
{
    static const int arr[] =
    {
         1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
        21, 23, 25, 27, 29, 31, 33, 35, 37, 39
    };

    auto xx = capo::sequence<capo::use::arithmetic<2>, int>(1, 21, 1);
    int i = 0;
    for (auto n : xx)
    {
        EXPECT_EQ(arr[i], n) << "At index: " << i;
        ++i;
    }
}

TEST_METHOD(fibonacci)
{
    static const int fb[] =
    {
        0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 
        55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181
    };

    for (size_t i = 0; i < capo::countof(fb); ++i)
    {
        unsigned long long x = 0, y = 1;
        capo::use::fibonacci::at(x, y, i);
        EXPECT_EQ(fb[i], x) << "At index: " << i;
    }
}
