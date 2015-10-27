#if defined(_MSC_VER)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "gtest/gtest.h"

////////////////////////////////////////////////////////////////

#define TEST_METHOD(TEST_NAME) TEST(assert, TEST_NAME)

////////////////////////////////////////////////////////////////

#include "preparing.h"
#include "cases.h"
