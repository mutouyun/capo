/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://orzz.org)
*/

#pragma once

#include "capo/preprocessor/pp_macros.hpp"

//////////////////////////////////////////////////////////////////////////

#define CAPO_PP_NEST_0_(F, L, N, ...)

#define CAPO_PP_NEST_1_(F, L, N, ...)            CAPO_PP_VA_(L(CAPO_PP_VA_(__VA_ARGS__)))
#define CAPO_PP_NEST_2_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_1_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_3_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_2_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_4_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_3_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_5_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_4_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_6_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_5_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_7_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_6_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_8_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_7_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_9_(F, L, N, ...)            CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_8_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_10_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_9_(F, L, N, N(__VA_ARGS__))))
                                                 
#define CAPO_PP_NEST_11_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_10_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_12_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_11_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_13_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_12_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_14_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_13_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_15_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_14_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_16_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_15_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_17_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_16_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_18_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_17_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_19_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_18_(F, L, N, N(__VA_ARGS__))))
#define CAPO_PP_NEST_20_(F, L, N, ...)           CAPO_PP_VA_(F(CAPO_PP_VA_(__VA_ARGS__), CAPO_PP_NEST_19_(F, L, N, N(__VA_ARGS__))))

/*
    CAPO_PP_NEST_(4, func, last, nest, xx)
    -->
    func(xx, func(nest(xx), func(nest(nest(xx)), last(nest(nest(nest(xx)))))))
*/

#define CAPO_PP_NEST_P_(F, ...)                  CAPO_PP_VA_(F(__VA_ARGS__))
#define CAPO_PP_NEST_(N, FUNC, LAST, NEST, ...)  CAPO_PP_NEST_P_(CAPO_PP_JOIN_(CAPO_PP_NEST_, CAPO_PP_JOIN_(N, _)), FUNC, LAST, NEST, __VA_ARGS__)
#define CAPO_PP_NEST_MAX_(FUNC, LAST, NEST, ...) CAPO_PP_NEST_(CAPO_PP_MAX_, FUNC, LAST, NEST, __VA_ARGS__)
