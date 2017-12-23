/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/preprocessor/pp_macros.hpp"

//////////////////////////////////////////////////////////////////////////

#define CAPO_PP_MULT_0_(F)

#define CAPO_PP_MULT_1_(F)          F(1)
#define CAPO_PP_MULT_2_(F)          CAPO_PP_MULT_1_(F) F(2)
#define CAPO_PP_MULT_3_(F)          CAPO_PP_MULT_2_(F) F(3)
#define CAPO_PP_MULT_4_(F)          CAPO_PP_MULT_3_(F) F(4)
#define CAPO_PP_MULT_5_(F)          CAPO_PP_MULT_4_(F) F(5)
#define CAPO_PP_MULT_6_(F)          CAPO_PP_MULT_5_(F) F(6)
#define CAPO_PP_MULT_7_(F)          CAPO_PP_MULT_6_(F) F(7)
#define CAPO_PP_MULT_8_(F)          CAPO_PP_MULT_7_(F) F(8)
#define CAPO_PP_MULT_9_(F)          CAPO_PP_MULT_8_(F) F(9)
#define CAPO_PP_MULT_10_(F)         CAPO_PP_MULT_9_(F) F(10)

#define CAPO_PP_MULT_11_(F)         CAPO_PP_MULT_10_(F) F(11)
#define CAPO_PP_MULT_12_(F)         CAPO_PP_MULT_11_(F) F(12)
#define CAPO_PP_MULT_13_(F)         CAPO_PP_MULT_12_(F) F(13)
#define CAPO_PP_MULT_14_(F)         CAPO_PP_MULT_13_(F) F(14)
#define CAPO_PP_MULT_15_(F)         CAPO_PP_MULT_14_(F) F(15)
#define CAPO_PP_MULT_16_(F)         CAPO_PP_MULT_15_(F) F(16)
#define CAPO_PP_MULT_17_(F)         CAPO_PP_MULT_16_(F) F(17)
#define CAPO_PP_MULT_18_(F)         CAPO_PP_MULT_17_(F) F(18)
#define CAPO_PP_MULT_19_(F)         CAPO_PP_MULT_18_(F) F(19)
#define CAPO_PP_MULT_20_(F)         CAPO_PP_MULT_19_(F) F(20)

/*
    CAPO_PP_MULT_(10, f)
    -->
    f(1) f(2) f(3) f(4) f(5) f(6) f(7) f(8) f(9) f(10)
*/

#define CAPO_PP_MULT_P_(F, ...)     CAPO_PP_VA_(F(__VA_ARGS__))
#define CAPO_PP_MULT_(N, F)         CAPO_PP_MULT_P_(CAPO_PP_JOIN_(CAPO_PP_MULT_, CAPO_PP_JOIN_(N, _)), F)
#define CAPO_PP_MULT_MAX_(F)        CAPO_PP_MULT_(CAPO_PP_MAX_, F)
