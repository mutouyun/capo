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
#define CAPO_PP_STR__(...)              CAPO_PP_SOL_(__VA_ARGS__)
#define CAPO_PP_STR_(...)               CAPO_PP_STR__(__VA_ARGS__)

/*
    Connect two args together
*/

#define CAPO_PP_CAT_(X, ...)            X##__VA_ARGS__
#define CAPO_PP_JOIN__(X, ...)          CAPO_PP_CAT_(X, __VA_ARGS__)
#define CAPO_PP_JOIN_(X, ...)           CAPO_PP_JOIN__(X, __VA_ARGS__)

////////////////////////////////////////////////////////////////
