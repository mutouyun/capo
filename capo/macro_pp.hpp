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

#define CAPO_PP_SOL(...)                  #__VA_ARGS__
#define CAPO_PP_STR___(...)               CAPO_PP_SOL(__VA_ARGS__)
#define CAPO_PP_STR(...)                  CAPO_PP_STR___(__VA_ARGS__)

/*
    Connect two args together
*/

#define CAPO_PP_CAT(X, ...)               X##__VA_ARGS__
#define CAPO_PP_JOIN___(X, ...)           CAPO_PP_CAT(X, __VA_ARGS__)
#define CAPO_PP_JOIN(X, ...)              CAPO_PP_JOIN___(X, __VA_ARGS__)

////////////////////////////////////////////////////////////////
