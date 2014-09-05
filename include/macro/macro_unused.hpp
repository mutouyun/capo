/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#ifndef CAPO_MACRO_UNUSED_HPP___
#define CAPO_MACRO_UNUSED_HPP___

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

#endif // CAPO_MACRO_UNUSED_HPP___
