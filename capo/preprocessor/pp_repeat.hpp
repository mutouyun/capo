/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/preprocessor/pp_macros.hpp"
#include "capo/preprocessor/pp_count.hpp"

//////////////////////////////////////////////////////////////////////////

#define CAPO_PP_REPEAT_0_(F1, F2, ...)

#define CAPO_PP_REPEAT_1_(F1, F2, ...)      CAPO_PP_VA_(F1(1, __VA_ARGS__))
#define CAPO_PP_REPEAT_2_(F1, F2, ...)      CAPO_PP_REPEAT_1_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(2, __VA_ARGS__))
#define CAPO_PP_REPEAT_3_(F1, F2, ...)      CAPO_PP_REPEAT_2_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(3, __VA_ARGS__))
#define CAPO_PP_REPEAT_4_(F1, F2, ...)      CAPO_PP_REPEAT_3_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(4, __VA_ARGS__))
#define CAPO_PP_REPEAT_5_(F1, F2, ...)      CAPO_PP_REPEAT_4_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(5, __VA_ARGS__))
#define CAPO_PP_REPEAT_6_(F1, F2, ...)      CAPO_PP_REPEAT_5_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(6, __VA_ARGS__))
#define CAPO_PP_REPEAT_7_(F1, F2, ...)      CAPO_PP_REPEAT_6_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(7, __VA_ARGS__))
#define CAPO_PP_REPEAT_8_(F1, F2, ...)      CAPO_PP_REPEAT_7_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(8, __VA_ARGS__))
#define CAPO_PP_REPEAT_9_(F1, F2, ...)      CAPO_PP_REPEAT_8_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(9, __VA_ARGS__))
#define CAPO_PP_REPEAT_10_(F1, F2, ...)     CAPO_PP_REPEAT_9_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(10, __VA_ARGS__))

#define CAPO_PP_REPEAT_11_(F1, F2, ...)     CAPO_PP_REPEAT_10_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(11, __VA_ARGS__))
#define CAPO_PP_REPEAT_12_(F1, F2, ...)     CAPO_PP_REPEAT_11_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(12, __VA_ARGS__))
#define CAPO_PP_REPEAT_13_(F1, F2, ...)     CAPO_PP_REPEAT_12_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(13, __VA_ARGS__))
#define CAPO_PP_REPEAT_14_(F1, F2, ...)     CAPO_PP_REPEAT_13_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(14, __VA_ARGS__))
#define CAPO_PP_REPEAT_15_(F1, F2, ...)     CAPO_PP_REPEAT_14_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(15, __VA_ARGS__))
#define CAPO_PP_REPEAT_16_(F1, F2, ...)     CAPO_PP_REPEAT_15_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(16, __VA_ARGS__))
#define CAPO_PP_REPEAT_17_(F1, F2, ...)     CAPO_PP_REPEAT_16_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(17, __VA_ARGS__))
#define CAPO_PP_REPEAT_18_(F1, F2, ...)     CAPO_PP_REPEAT_17_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(18, __VA_ARGS__))
#define CAPO_PP_REPEAT_19_(F1, F2, ...)     CAPO_PP_REPEAT_18_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(19, __VA_ARGS__))
#define CAPO_PP_REPEAT_20_(F1, F2, ...)     CAPO_PP_REPEAT_19_(F1, F2, __VA_ARGS__) CAPO_PP_VA_(F2(20, __VA_ARGS__))

/*
    CAPO_PP_REPEAT(5, f, data)
    -->
    f(1, data) f(2, data) f(3, data) f(4, data) f(5, data)
*/

#define CAPO_PP_REPEAT_P_(F, ...)           CAPO_PP_VA_(F(__VA_ARGS__))

#define CAPO_PP_REPEATEX_(N, F1, F2, ...)   CAPO_PP_REPEAT_P_(CAPO_PP_JOIN_(CAPO_PP_REPEAT_, CAPO_PP_JOIN_(N, _)), F1, F2, __VA_ARGS__)
#define CAPO_PP_REPEATEX_MAX_(F1, F2, ...)  CAPO_PP_REPEATEX_(CAPO_PP_MAX_, F1, F2, __VA_ARGS__)

#define CAPO_PP_REPEAT_(N, F, ...)          CAPO_PP_REPEATEX_(N, F, F, __VA_ARGS__)
#define CAPO_PP_REPEAT_MAX_(F, ...)         CAPO_PP_REPEATEX_MAX_(F, F, __VA_ARGS__)
