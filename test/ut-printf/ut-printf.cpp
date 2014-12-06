#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

#include "capo/printf.hpp"

#include <string>
#include <string.h>

////////////////////////////////////////////////////////////////

#define TEST_METHOD(TEST_NAME) TEST(printf, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(std_cout)
{
    char c = 'A';
    capo::printf(capo::use::std_cout, "1234567%s%c\n", " ", c);
}

TEST_METHOD(sprintf)
{
    char c = 'A';
    char buf[100];

    capo::printf([&buf](const std::string& str)
    {
        strcpy(buf, str.c_str());
    }, "1234567%s%c\n", " ", c);

    EXPECT_STREQ("1234567 A\n", buf);
}
