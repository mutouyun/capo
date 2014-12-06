#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

#include "capo/assert.hpp"

////////////////////////////////////////////////////////////////

bool b = true;
int i = 1, j = 2;
char what[] = "What's up";

void func_1(void)
{
    CAPO_ENSURE_(b = false)(i)(j)(what).msg("func_1 -> capo::assert::context\n").except();
}

void func_2(void)
{
    CAPO_ENSURE_(b = false)(i)(j)(what).msg("func_2 -> bool\n").except(b);
}

#define TEST_METHOD(TEST_NAME) TEST(assert, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(ensure)
{
    EXPECT_THROW(func_1(), capo::assert::context);
    EXPECT_THROW(func_2(), bool);
}
