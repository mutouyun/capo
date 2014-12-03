
#include <iostream>

#include "gtest/gtest.h"

#include "capo/preprocessor.hpp"
#include "capo/func_decl.hpp"

////////////////////////////////////////////////////////////////

int foo(int a, int b)
{
    return a + b;
}

const char* bar_0(void)
{
    return __FUNCTION__;
}

const char* bar_3(int, int, int)
{
    return __FUNCTION__;
}

#define TEST_METHOD(TEST_NAME) TEST(preprocessor, TEST_NAME)

////////////////////////////////////////////////////////////////

TEST_METHOD(repeat_nest)
{
    EXPECT_EQ("f(1, d1, d2) f(2, d1, d2) f(3, d1, d2) f(4, d1, d2) f(5, d1, d2)", 
        CAPO_PP_STR_(CAPO_PP_REPEAT_(5, f, d1, d2)));
    EXPECT_EQ("func(xx, func(nest(xx), func(nest(nest(xx)), last(nest(nest(nest(xx)))))))", 
        CAPO_PP_STR_(CAPO_PP_NEST_(4, func, last, nest, xx)));
}

TEST_METHOD(clone_reverse_limit)
{
    EXPECT_EQ("c , c , c , c , c , c"    , CAPO_PP_STR_(CAPO_PP_CLONE_(6, c, ,)));
    EXPECT_EQ("9, 8, 7, 6, 5, 4, 3, 2, 1", CAPO_PP_STR_(CAPO_PP_REVERSE_(1, 2, 3, 4, 5, 6, 7, 8, 9)));
    EXPECT_EQ("a, b, c"                  , CAPO_PP_STR_(CAPO_PP_LIMIT_(3, a, b, c, d, e, f)));
}

TEST_METHOD(calculation)
{
    EXPECT_EQ(6, CAPO_PP_INC_(5));
    EXPECT_EQ(4, CAPO_PP_DEC_(5));
    EXPECT_EQ(7, CAPO_PP_ADD_(3, 4));
    EXPECT_EQ(2, CAPO_PP_SUB_(5, 3));
}

TEST_METHOD(comparison)
{
    EXPECT_EQ(true , CAPO_PP_LESS_(3, 4, true, false));
    EXPECT_EQ(false, CAPO_PP_MORE_(3, 4, true, false));
    EXPECT_EQ(true , CAPO_PP_EQUA_(4, 4, true, false));
}

TEST_METHOD(function)
{
    EXPECT_EQ(9 , CAPO_PP_RECUR_(9, foo, 0, 1));
    EXPECT_EQ(45, CAPO_PP_ORDER_(foo, 1, 2, 3, 4, 5, 6, 7, 8, 9));

    EXPECT_EQ(bar_0()       , CAPO_PP_CALL_(bar_));
    EXPECT_EQ(bar_3(1, 2, 3), CAPO_PP_CALL_(bar_, 1, 2, 3));

    EXPECT_EQ("int par1 , char par2 , long par3 , double par4", 
        CAPO_PP_STR_(CAPO_PP_PARAM_(par, int, char, long, double)));

    EXPECT_EQ("T1 , T2 , T3"             , CAPO_PP_STR_(CAPO_PP_TYPE_1_(3, T)));
    EXPECT_EQ("typename P1"              , CAPO_PP_STR_(CAPO_PP_TYPE_1_(1, typename P)));
    EXPECT_EQ("typename T1 , typename T2", CAPO_PP_STR_(CAPO_PP_TYPE_1_(2, typename T)));
    EXPECT_EQ("P1 * par1 = nullptr , P2 * par2 = nullptr , P3 * par3 = nullptr", 
        CAPO_PP_STR_(CAPO_PP_TYPE_2_(3, P, * par, = nullptr)));
}
