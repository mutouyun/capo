/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

////////////////////////////////////////////////////////////////

/*
    Get a string of macro
*/

#define CAPO_PP_SOL_(...)               #__VA_ARGS__
#define CAPO_PP_STR_T__(...)            CAPO_PP_SOL_(__VA_ARGS__)
#define CAPO_PP_STR_(...)               CAPO_PP_STR_T__(__VA_ARGS__)

/*
    Connect two args together
*/

#define CAPO_PP_CAT_(X, ...)            X##__VA_ARGS__
#define CAPO_PP_JOIN_T__(X, ...)        CAPO_PP_CAT_(X, __VA_ARGS__)
#define CAPO_PP_JOIN_(X, ...)           CAPO_PP_JOIN_T__(X, __VA_ARGS__)

/*
    Circumvent MSVC __VA_ARGS__ BUG
*/

#define CAPO_PP_VA_(...)                __VA_ARGS__ /* Try to expand __VA_ARGS__ */
#define CAPO_PP_PROXY_(F, ...)          CAPO_PP_VA_(F(__VA_ARGS__))

////////////////////////////////////////////////////////////////
