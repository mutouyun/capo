
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
#if defined(_MSC_VER) && (_MSC_VER <= 1900)
    /*
        <MSVC 2013> Has not support constexpr yet.
        See: http://msdn.microsoft.com/en-us/library/hh567368.aspx
    */
    EXPECT_EQ(capo::countof(p), 0);
#else /*!_MSC_VER*/
    int arr[capo::countof(p)];
    EXPECT_EQ(capo::countof(p), capo::countof(arr));
#endif/*!_MSC_VER*/
}
