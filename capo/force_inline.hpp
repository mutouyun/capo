/*
    The Capo Library
    Code covered by the MIT License

    Author: mutouyun (http://darkc.at)
*/

#pragma once

////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#   define CAPO_FORCE_INLINE_ __forceinline
#elif defined(__GNUC__)
#   define CAPO_FORCE_INLINE_ __inline__ __attribute__((always_inline))
#else
#   define CAPO_FORCE_INLINE_ inline
#endif

////////////////////////////////////////////////////////////////
