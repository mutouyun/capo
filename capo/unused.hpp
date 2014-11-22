/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

////////////////////////////////////////////////////////////////

#ifdef CAPO_UNUSED_
#   error "CAPO_UNUSED_ has been defined."
#endif

#if defined(_MSC_VER)
#   define CAPO_UNUSED_ __pragma(warning(suppress:4100))
#elif defined(__GNUC__)
#   define CAPO_UNUSED_ __attribute__((__unused__))
#else
#   define CAPO_UNUSED_
#endif

////////////////////////////////////////////////////////////////
