/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

#include "capo/preprocessor/pp_macros.hpp"

//////////////////////////////////////////////////////////////////////////

#define CAPO_PP_MAX_        20

#define CAPO_PP_FILTER__(   _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10, \
                            _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                            _N, ...) _N

#define CAPO_PP_NUMBER__()  20,  19,  18,  17,  16,  15,  14,  13,  12,  11, \
                            10,   9,   8,   7,   6,   5,   4,   3,   2,   1

/*
    Get count of args from __VA_ARGS__

    CAPO_PP_COUNT_(a, b, c, d)
    -->
    4
*/

#define CAPO_PP_HELPER_(...)   CAPO_PP_VA_(CAPO_PP_FILTER__(__VA_ARGS__))
#define CAPO_PP_COUNT_(...)    CAPO_PP_HELPER_(__VA_ARGS__, CAPO_PP_NUMBER__())
