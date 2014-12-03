/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/preprocessor/pp_macros.hpp"

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_B_2_(a, b, c, d, e)
    -->
    c, d, e
*/

#define CAPO_PP_B_H_(a, ...)    __VA_ARGS__

#define CAPO_PP_B_0_(...)       CAPO_PP_VA_(__VA_ARGS__)
#define CAPO_PP_B_1_(...)       CAPO_PP_VA_(CAPO_PP_B_H_(__VA_ARGS__))
#define CAPO_PP_B_2_(...)       CAPO_PP_B_1_(CAPO_PP_B_1_(__VA_ARGS__))
#define CAPO_PP_B_3_(...)       CAPO_PP_B_1_(CAPO_PP_B_2_(__VA_ARGS__))
#define CAPO_PP_B_4_(...)       CAPO_PP_B_1_(CAPO_PP_B_3_(__VA_ARGS__))
#define CAPO_PP_B_5_(...)       CAPO_PP_B_1_(CAPO_PP_B_4_(__VA_ARGS__))
#define CAPO_PP_B_6_(...)       CAPO_PP_B_1_(CAPO_PP_B_5_(__VA_ARGS__))
#define CAPO_PP_B_7_(...)       CAPO_PP_B_1_(CAPO_PP_B_6_(__VA_ARGS__))
#define CAPO_PP_B_8_(...)       CAPO_PP_B_1_(CAPO_PP_B_7_(__VA_ARGS__))
#define CAPO_PP_B_9_(...)       CAPO_PP_B_1_(CAPO_PP_B_8_(__VA_ARGS__))

#define CAPO_PP_B_10_(...)      CAPO_PP_B_1_(CAPO_PP_B_9_(__VA_ARGS__))
#define CAPO_PP_B_11_(...)      CAPO_PP_B_1_(CAPO_PP_B_10_(__VA_ARGS__))
#define CAPO_PP_B_12_(...)      CAPO_PP_B_1_(CAPO_PP_B_11_(__VA_ARGS__))
#define CAPO_PP_B_13_(...)      CAPO_PP_B_1_(CAPO_PP_B_12_(__VA_ARGS__))
#define CAPO_PP_B_14_(...)      CAPO_PP_B_1_(CAPO_PP_B_13_(__VA_ARGS__))
#define CAPO_PP_B_15_(...)      CAPO_PP_B_1_(CAPO_PP_B_14_(__VA_ARGS__))
#define CAPO_PP_B_16_(...)      CAPO_PP_B_1_(CAPO_PP_B_15_(__VA_ARGS__))
#define CAPO_PP_B_17_(...)      CAPO_PP_B_1_(CAPO_PP_B_16_(__VA_ARGS__))
#define CAPO_PP_B_18_(...)      CAPO_PP_B_1_(CAPO_PP_B_17_(__VA_ARGS__))
#define CAPO_PP_B_19_(...)      CAPO_PP_B_1_(CAPO_PP_B_18_(__VA_ARGS__))

#define CAPO_PP_B_20_(...)      CAPO_PP_B_1_(CAPO_PP_B_19_(__VA_ARGS__))

#define CAPO_PP_B_P_(F, ...)    CAPO_PP_VA_(F(__VA_ARGS__))
#define CAPO_PP_B_(N, ...)      CAPO_PP_B_P_(CAPO_PP_JOIN_(CAPO_PP_B_, CAPO_PP_JOIN_(N, _)), __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////

/*
    CAPO_PP_A_3_(a, b, c, d, e)
    -->
    c
*/

#define CAPO_PP_A_H_(a, ...)    a

#define CAPO_PP_A_0_(...)
#define CAPO_PP_A_1_(...)       CAPO_PP_VA_(CAPO_PP_A_H_(__VA_ARGS__))
#define CAPO_PP_A_2_(...)       CAPO_PP_A_1_(CAPO_PP_B_1_(__VA_ARGS__))
#define CAPO_PP_A_3_(...)       CAPO_PP_A_1_(CAPO_PP_B_2_(__VA_ARGS__))
#define CAPO_PP_A_4_(...)       CAPO_PP_A_1_(CAPO_PP_B_3_(__VA_ARGS__))
#define CAPO_PP_A_5_(...)       CAPO_PP_A_1_(CAPO_PP_B_4_(__VA_ARGS__))
#define CAPO_PP_A_6_(...)       CAPO_PP_A_1_(CAPO_PP_B_5_(__VA_ARGS__))
#define CAPO_PP_A_7_(...)       CAPO_PP_A_1_(CAPO_PP_B_6_(__VA_ARGS__))
#define CAPO_PP_A_8_(...)       CAPO_PP_A_1_(CAPO_PP_B_7_(__VA_ARGS__))
#define CAPO_PP_A_9_(...)       CAPO_PP_A_1_(CAPO_PP_B_8_(__VA_ARGS__))

#define CAPO_PP_A_10_(...)      CAPO_PP_A_1_(CAPO_PP_B_9_(__VA_ARGS__))
#define CAPO_PP_A_11_(...)      CAPO_PP_A_1_(CAPO_PP_B_10_(__VA_ARGS__))
#define CAPO_PP_A_12_(...)      CAPO_PP_A_1_(CAPO_PP_B_11_(__VA_ARGS__))
#define CAPO_PP_A_13_(...)      CAPO_PP_A_1_(CAPO_PP_B_12_(__VA_ARGS__))
#define CAPO_PP_A_14_(...)      CAPO_PP_A_1_(CAPO_PP_B_13_(__VA_ARGS__))
#define CAPO_PP_A_15_(...)      CAPO_PP_A_1_(CAPO_PP_B_14_(__VA_ARGS__))
#define CAPO_PP_A_16_(...)      CAPO_PP_A_1_(CAPO_PP_B_15_(__VA_ARGS__))
#define CAPO_PP_A_17_(...)      CAPO_PP_A_1_(CAPO_PP_B_16_(__VA_ARGS__))
#define CAPO_PP_A_18_(...)      CAPO_PP_A_1_(CAPO_PP_B_17_(__VA_ARGS__))
#define CAPO_PP_A_19_(...)      CAPO_PP_A_1_(CAPO_PP_B_18_(__VA_ARGS__))

#define CAPO_PP_A_20_(...)      CAPO_PP_A_1_(CAPO_PP_B_19_(__VA_ARGS__))

#define CAPO_PP_A_P_(F, ...)    CAPO_PP_VA_(F(__VA_ARGS__))
#define CAPO_PP_A_(N, ...)      CAPO_PP_A_P_(CAPO_PP_JOIN_(CAPO_PP_A_, CAPO_PP_JOIN_(N, _)), __VA_ARGS__)
