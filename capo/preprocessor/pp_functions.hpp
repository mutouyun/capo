/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

//////////////////////////////////////////////////////////////////////////

#include "capo/preprocessor/pp_count.hpp"
#include "capo/preprocessor/pp_arg.hpp"
#include "capo/preprocessor/pp_repeat.hpp"
#include "capo/preprocessor/pp_nest.hpp"
#include "capo/preprocessor/pp_mult.hpp"

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_CLONE_(6, c, ,)
    -->
    c , c , c , c , c , c
*/

#define CAPO_PP_CLONE_1_(N, CON, ...)       CON
#define CAPO_PP_CLONE_2_(N, CON, ...)       __VA_ARGS__ CON
#define CAPO_PP_CLONE_(N, CON, ...)         CAPO_PP_REPEATEX_(N, CAPO_PP_CLONE_1_, CAPO_PP_CLONE_2_, CON, __VA_ARGS__)
#define CAPO_PP_CLONE_MAX_(CON, ...)        CAPO_PP_CLONE_(CAPO_PP_MAX_, CON, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_REVERSE_(1, 2, 3, 4, 5, 6, 7, 8, 9)
    -->
    9, 8, 7, 6, 5, 4, 3, 2, 1
*/

#define CAPO_PP_REVERSE_T__(DATA, ...)      __VA_ARGS__, CAPO_PP_A_1_(DATA)
#define CAPO_PP_REVERSE_(...)               CAPO_PP_NEST_(CAPO_PP_COUNT_(__VA_ARGS__), CAPO_PP_REVERSE_T__, CAPO_PP_VA_, CAPO_PP_B_1_, __VA_ARGS__)

/*
    CAPO_PP_LIMIT_(3, a, b, c, d, e, f)
    -->
    a, b, c
*/

#define CAPO_PP_LIMIT_T__(DATA, ...)        CAPO_PP_A_1_(DATA), __VA_ARGS__
#define CAPO_PP_LIMIT_(N, ...)              CAPO_PP_NEST_(N, CAPO_PP_LIMIT_T__, CAPO_PP_A_1_, CAPO_PP_B_1_, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_ADD_(3, 4)
    -->
    7

    CAPO_PP_SUB_(5, 3)
    -->
    2
*/

#define CAPO_PP_INC_(NUM)                   CAPO_PP_COUNT_(CAPO_PP_CLONE_(NUM, -, ,) , -)
#define CAPO_PP_DEC_(NUM)                   CAPO_PP_COUNT_(CAPO_PP_B_1_(CAPO_PP_CLONE_(NUM, -, ,)))

#define CAPO_PP_ADD_(NUM1, NUM2)            CAPO_PP_COUNT_(CAPO_PP_CLONE_(NUM1, -, ,) , CAPO_PP_CLONE_(NUM2, -, ,))
#define CAPO_PP_SUB_(NUM1, NUM2)            CAPO_PP_COUNT_(CAPO_PP_B_(NUM2, CAPO_PP_CLONE_(NUM1, -, ,)))

#define CAPO_PP_REM_(NUM)                   CAPO_PP_SUB_(CAPO_PP_MAX_, NUM)

/*
    CAPO_PP_LESS_(3, 4, Yes, No)
    -->
    Yes

    CAPO_PP_MORE_(3, 4, Yes, No)
    -->
    No

    CAPO_PP_EQUA_(3, 4, Yes, No)
    -->
    No
*/

#define CAPO_PP_LESS_NUMBER__(NUM, Y, N)    CAPO_PP_REVERSE_(CAPO_PP_CLONE_(NUM, N, ,) , CAPO_PP_CLONE_(CAPO_PP_REM_(NUM), Y, ,))
#define CAPO_PP_MORE_NUMBER__(NUM, Y, N)    CAPO_PP_REVERSE_(CAPO_PP_CLONE_(CAPO_PP_DEC_(NUM), Y, ,) , CAPO_PP_CLONE_(CAPO_PP_REM_(NUM), N, ,) , N)

#define CAPO_PP_LESS_(NUM1, NUM2, Y, N)     CAPO_PP_HELPER_(CAPO_PP_CLONE_(NUM2, -, ,) , CAPO_PP_LESS_NUMBER__(NUM1, Y, N))
#define CAPO_PP_MORE_(NUM1, NUM2, Y, N)     CAPO_PP_HELPER_(CAPO_PP_CLONE_(NUM2, -, ,) , CAPO_PP_MORE_NUMBER__(NUM1, Y, N))
#define CAPO_PP_EQUA_(NUM1, NUM2, Y, N)     CAPO_PP_MORE_(NUM1, NUM2, N, CAPO_PP_LESS_(NUM1, NUM2, N, Y))

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_RECUR_(9, f, nul, c)
    -->
    f(c, f(c, f(c, f(c, f(c, f(c, f(c, f(c, f(c, nul)))))))))
*/

#define CAPO_PP_RECUR_1__(DATA, ...)        CAPO_PP_A_1_(DATA)(CAPO_PP_B_2_(DATA), __VA_ARGS__)
#define CAPO_PP_RECUR_2__(DATA)             CAPO_PP_A_1_(DATA)(CAPO_PP_B_2_(DATA), CAPO_PP_A_2_(DATA))
#define CAPO_PP_RECUR_(N, F, NUL, ...)      CAPO_PP_NEST_(N, CAPO_PP_RECUR_1__, CAPO_PP_RECUR_2__, CAPO_PP_VA_, F, NUL, __VA_ARGS__)

/*
    CAPO_PP_ORDER_(f, 1, 2, 3, 4, 5, 6, 7, 8, 9)
    -->
    f(1, f(2, f(3, f(4, f(5, f(6, f(7, f(8, 9))))))))
*/

#define CAPO_PP_ORDER_1__(DATA, ...)        CAPO_PP_A_1_(DATA)(CAPO_PP_A_2_(DATA), __VA_ARGS__)
#define CAPO_PP_ORDER_2__(DATA)             CAPO_PP_A_1_(DATA)(CAPO_PP_B_1_(DATA))
#define CAPO_PP_ORDER_3__(...)              CAPO_PP_A_1_(__VA_ARGS__), CAPO_PP_B_2_(__VA_ARGS__)
#define CAPO_PP_ORDER_N_(N, F, ...)         CAPO_PP_NEST_(N, CAPO_PP_ORDER_1__, CAPO_PP_ORDER_2__, CAPO_PP_ORDER_3__, F, __VA_ARGS__)
#define CAPO_PP_ORDER_(F, ...)              CAPO_PP_ORDER_N_(CAPO_PP_COUNT_(CAPO_PP_B_1_(__VA_ARGS__)), F, __VA_ARGS__)

/*
    CAPO_PP_CALL_(func, 1, 2, 3, 4, 5, 6, 7)
    -->
    func7(1, 2, 3, 4, 5, 6, 7)

    CAPO_PP_CALL_(func)
    -->
    func0()
*/

#define CAPO_PP_CALL_1__(F, ...)            CAPO_PP_VA_(CAPO_PP_JOIN_(F, CAPO_PP_COUNT_(__VA_ARGS__))(__VA_ARGS__))
#define CAPO_PP_CALL_2__(F)                 CAPO_PP_VA_(CAPO_PP_JOIN_(F, 0)())
#define CAPO_PP_CALL_T__(N)                 CAPO_PP_JOIN_(CAPO_PP_CALL_, CAPO_PP_LESS_(1, N, 1__, 2__))
#define CAPO_PP_CALL_N_(N, ...)             CAPO_PP_PROXY_(CAPO_PP_CALL_T__(N), __VA_ARGS__)
#define CAPO_PP_CALL_(...)                  CAPO_PP_CALL_N_(CAPO_PP_COUNT_(__VA_ARGS__), __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_PARAM_(par, int, char, long, double)
    -->
    int par1 , char par2 , long par3 , double par4
*/

#define CAPO_PP_PARAM_1__(N, ...)           CAPO_PP_A_1_(CAPO_PP_B_(N, __VA_ARGS__)) CAPO_PP_JOIN_(CAPO_PP_A_1_(__VA_ARGS__), N)
#define CAPO_PP_PARAM_2__(N, ...)           , CAPO_PP_PARAM_1__(N, __VA_ARGS__)
#define CAPO_PP_PARAM_(...) \
    CAPO_PP_REPEATEX_(CAPO_PP_COUNT_(CAPO_PP_B_1_(__VA_ARGS__)), CAPO_PP_PARAM_1__, CAPO_PP_PARAM_2__, __VA_ARGS__)

/*
    CAPO_PP_TYPE_1_(3, T)
    -->
    T1 , T2 , T3

    CAPO_PP_TYPE_2_(3, P, * par, = NULL)
    -->
    P1 * par1 = NULL, P2 * par2 = NULL, P3 * par3 = NULL
*/

#define CAPO_PP_TYPE_1_1__(N, T1, ...)          CAPO_PP_VA_(CAPO_PP_JOIN_(T1, N) __VA_ARGS__)
#define CAPO_PP_TYPE_2_1__(N, T1, T2, ...)      CAPO_PP_VA_(CAPO_PP_JOIN_(T1, N) CAPO_PP_JOIN_(T2, N) __VA_ARGS__)
#define CAPO_PP_TYPE_3_1__(N, T1, T2, T3, ...)  CAPO_PP_VA_(CAPO_PP_JOIN_(T1, N) CAPO_PP_JOIN_(T2, N) CAPO_PP_JOIN_(T3, N) __VA_ARGS__)

#define CAPO_PP_TYPE_1_2__(N, T1, ...)          CAPO_PP_VA_(, CAPO_PP_JOIN_(T1, N) __VA_ARGS__)
#define CAPO_PP_TYPE_2_2__(N, T1, T2, ...)      CAPO_PP_VA_(, CAPO_PP_JOIN_(T1, N) CAPO_PP_JOIN_(T2, N) __VA_ARGS__)
#define CAPO_PP_TYPE_3_2__(N, T1, T2, T3, ...)  CAPO_PP_VA_(, CAPO_PP_JOIN_(T1, N) CAPO_PP_JOIN_(T2, N) CAPO_PP_JOIN_(T3, N) __VA_ARGS__)

#define CAPO_PP_TYPE_1_(N, T1, ...)             CAPO_PP_REPEATEX_(N, CAPO_PP_TYPE_1_1__, CAPO_PP_TYPE_1_2__, T1, __VA_ARGS__)
#define CAPO_PP_TYPE_2_(N, T1, T2, ...)         CAPO_PP_REPEATEX_(N, CAPO_PP_TYPE_2_1__, CAPO_PP_TYPE_2_2__, T1, T2, __VA_ARGS__)
#define CAPO_PP_TYPE_3_(N, T1, T2, T3, ...)     CAPO_PP_REPEATEX_(N, CAPO_PP_TYPE_3_1__, CAPO_PP_TYPE_3_2__, T1, T2, T3, __VA_ARGS__)

#define CAPO_PP_TYPE_MAX_1_(T1, ...)            CAPO_PP_TYPE_1_(CAPO_PP_MAX_, T1, __VA_ARGS__)
#define CAPO_PP_TYPE_MAX_2_(T1, T2, ...)        CAPO_PP_TYPE_2_(CAPO_PP_MAX_, T1, T2, __VA_ARGS__)
#define CAPO_PP_TYPE_MAX_3_(T1, T2, T3, ...)    CAPO_PP_TYPE_3_(CAPO_PP_MAX_, T1, T2, T3, __VA_ARGS__)
